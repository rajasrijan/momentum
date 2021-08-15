#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <arch/efi/efi.h>
#include <arch/mm.h>
#include <arch/video.h>
#include <kernel/logging.h>
#include <kernel/multitask.h>
#include <kernel/vfs.h>
#include <kernel/vnode.h>
#include <memory>
#include <DDI/driver.h>
#include <kernel/binary_loader.h>
#include <kernel/framebuffer.h>

extern "C" uint64_t _tls_index = 0;
extern "C" int _Init_global_epoch = 0;
extern "C" uint64_t _Init_thread_epoch = 0;
EFI_SYSTEM_TABLE *st = nullptr;
int const Uninitialized = 0;
int const BeingInitialized = -1;
bool CallConstructors();

bool operator==(const EFI_GUID &guid1, const EFI_GUID &guid2)
{
    return !memcmp(&guid1, &guid2, sizeof(EFI_GUID));
}

extern "C" void _Init_thread_header(int *const pOnce)
{
    if (*pOnce == Uninitialized)
    {
        *pOnce = BeingInitialized;
    }
    else
    {
        while (*pOnce == BeingInitialized)
        {
            if (*pOnce == Uninitialized)
            {
                *pOnce = BeingInitialized;
                return;
            }
        }
        _Init_thread_epoch = _Init_global_epoch;
    }
}

extern "C" void _Init_thread_footer(int *const pOnce)
{
    ++_Init_global_epoch;
    *pOnce = _Init_global_epoch;
    _Init_thread_epoch = _Init_global_epoch;
}

class efi_interface_vnode : public vnode
{
  private:
    EFI_BLOCK_IO_PROTOCOL *interface;

  public:
    efi_interface_vnode(EFI_HANDLE handle) : vnode(nullptr)
    {
        EFI_STATUS status = 0;
        EFI_GUID guid = EFI_BLOCK_IO_PROTOCOL_GUID;

        v_type = VBLK;
        status = st->BootServices->HandleProtocol(handle, &guid, (VOID **)&interface);
        if (status)
        {
            log_error("Failed to get interface handle!\n");
            return;
        }
        log_info("Created block device interface 0x%x\n", interface);
    }
    ~efi_interface_vnode()
    {
        log_info("Deleating block device interface 0x%x\n", interface);
    }

    int ioctl(uint32_t command, void *data, int fflag)
    {
        int ret = 0;
        switch (command)
        {
        case BLKPBSZGET: {
            *((uint64_t *)data) = interface->Media->BlockSize;
            return 0;
        }
        case BLKGETSIZE64: {
            *((uint64_t *)data) = interface->Media->LastBlock;
            return 0;
        }
        default:
            return -EINVAL;
        }
        return ret;
    }

    int bread(size_t position, size_t size, char *data, size_t *bytesRead)
    {
        size *= interface->Media->BlockSize;
        auto status = interface->ReadBlocks(interface, interface->Media->MediaId, position, size, data);
        if (status)
        {
            log_error("Failed to read data. error [%d]\n", status);
            return -EUNK;
        }
        return 0;
    }
    bool isPartitionable()
    {
        return false;
    }
};

EFI_STATUS ConvertPointer(UINTN DebugDisposition, VOID **Address)
{
    return 0;
}

template <int lvl = 4>
int allocate_vaddr(page_struct *pg, uint64_t vaddr, uint64_t paddr, uint64_t size)
{
    int ret = 0;
    // log_debug("lvl=%d, vaddr=[%llx], paddr=[%llx], size=[%llx]\n", lvl, vaddr, paddr, size);
    auto vaddr_end = vaddr + size, vaddr_it = vaddr;
    // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
    while (vaddr_it < vaddr_end)
    {
        // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
        auto pg_idx = PG_LVL_IDX(vaddr_it, lvl);
        // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);

        // log_debug("lvl=%d, vaddr=[%llx], paddr=[%llx], size=[%llx]\n", lvl, vaddr_it, paddr, size);
        //  if the virtual address is aligned and size is greater than page size of this level, then allocate the whole
        //  page.
        if (((PG_LVL_OFFSET(vaddr_it, lvl) == 0) && ((vaddr_end - vaddr_it) >= PG_LVL_SZ(lvl))) || (lvl == 1))
        {
            // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
            pg[pg_idx].addr = paddr;
            pg[pg_idx].present = 1;
            pg[pg_idx].rw = 1;

            if (lvl > 1)
                pg[pg_idx].ps = 1;
            // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
            vaddr_it += PG_LVL_SZ(lvl);
            paddr += PG_LVL_SZ(lvl);
        }
        else
        {
            if (pg[pg_idx].addr == 0)
            {
                // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
                uint64_t addr = 0;
                // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
                ret = st->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, &addr);
                // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
                if (ret)
                {
                    log_error("Failed to allocate page\n");
                    return ret;
                }
                memset((void *)addr, 0, EFI_PAGE_SIZE);
                // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);

                pg[pg_idx].addr = addr;
                pg[pg_idx].present = 1;
                pg[pg_idx].rw = 1;
            }
            // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
            auto size_for_lower_lvl = std::min(vaddr_end, PG_LVL_ALIGN(vaddr_it + PG_LVL_SZ(lvl), lvl)) - vaddr_it;
            // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);

            ret = allocate_vaddr < (lvl > 1) ? lvl - 1 : lvl > ((page_struct *)(pg[pg_idx].addr & 0xfffffffffffff000), vaddr_it, paddr, size_for_lower_lvl);
            // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
            if (ret)
            {
                log_error("Failed to allocate_vaddr\n");
                return ret;
            }
            // log_info("%s:%d %s()\n", __FILE__, __LINE__, __FUNCTION__);
            vaddr_it += size_for_lower_lvl;
            paddr += size_for_lower_lvl;
        }
    }
    return ret;
}

int find_kernel_and_boot(EFI_HANDLE handle)
{
    int ret = 0;
    log_info("Looking for kernel image...\n");
    uint64_t entry_point = 0;
    vector<MemPage> memory_map;
    vector<symbol_info> symbol_table;
    vector<shared_ptr<class vnode>> file_list;
    find("/mnt/", "momentum.elf", 2, file_list);
    if (file_list.empty())
    {
        log_error("No kernel images found!");
        return -ENOFILE;
    }
    log_info("Found %d files\n", file_list.size());

    log_info("Loading [%s]\n", file_list[0]->getName().c_str());

    if ((ret = binary_loader::load_kernel(file_list[0], memory_map, entry_point, symbol_table)) != 0)
    {
        log_error("Failed to load binary!\n");
        return ret;
    }

    auto mem_available = binary_loader::get_symbol_by_name<physical_memory_map>("mem_available", symbol_table, memory_map);
    auto mem_available_count = binary_loader::get_symbol_by_name<uint64_t>("mem_available_count", symbol_table, memory_map);
    auto mem_reserved = binary_loader::get_symbol_by_name<physical_memory_map>("mem_reserved", symbol_table, memory_map);
    auto mem_reserved_count = binary_loader::get_symbol_by_name<uint64_t>("mem_reserved_count", symbol_table, memory_map);
    auto fb = binary_loader::get_symbol_by_name<framebuffer>("fb", symbol_table, memory_map);
    auto rsdp_table = binary_loader::get_symbol_by_name<void>("rsdp_table", symbol_table, memory_map);
    auto root_uuid = binary_loader::get_symbol_by_name<uuid_t>("root_partition_uuid", symbol_table, memory_map);

    page_struct *pg = nullptr;

    ret = st->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, (EFI_PHYSICAL_ADDRESS *)(&pg));
    if (ret)
    {
        log_error("Failed to allocate page table. error %d\n", ret);
        return ret;
    }
    memset((void *)pg, 0, EFI_PAGE_SIZE);
    //  identity map first 1GB. this is required.
    ret = allocate_vaddr(pg, 0, 0, 0x40000000);
    //  map kernel to new page table
    for (const auto &page : memory_map)
    {
        ret = allocate_vaddr(pg, page.vaddr, page.paddr, page.size);
        if (ret)
        {
            log_error("Failed to map vaddr [%llx] to paddr [%llx] of sizev [%llx]\n", page.vaddr, page.paddr, page.size);
            return ret;
        }
    }

    uint64_t kernel_stack_paddr = 0;
    //  allocate 4 pages of stack
    ret = st->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, 4, &kernel_stack_paddr);
    if (ret)
    {
        log_error("Failed to allocate kernel stack. error %d\n", ret);
        return ret;
    }

    log_info("stack vaddr=%llx, stack paddr=%llx, stack size=%llx\n", kernel_stack_paddr, kernel_stack_paddr, EFI_PAGE_SIZE * 4);
    ret = allocate_vaddr(pg, kernel_stack_paddr, kernel_stack_paddr, EFI_PAGE_SIZE * 4);
    if (ret)
    {
        log_error("Failed to map vaddr [%llx] to paddr [%llx] of sizev [%llx]\n", kernel_stack_paddr, kernel_stack_paddr, EFI_PAGE_SIZE * 4);
        return ret;
    }

    //  exit boot service.
    uint32_t descriptorVer = 0;
    uint64_t memoryMapKey = 0, memoryMapSize = 0, descriptorSize = 0, loop_counter = 5;
    EFI_MEMORY_DESCRIPTOR *memoryMap = nullptr;

    log_info("Exiting boot service\n");

    while (loop_counter--)
    {
        ret = st->BootServices->GetMemoryMap(&memoryMapSize, memoryMap, &memoryMapKey, &descriptorSize, &descriptorVer);
        if (ret == EFI_BUFFER_TOO_SMALL)
        {
            log_warning("Memory Map buffer is too small, (re)allocating new buffer\n");
            if (memoryMap)
                free(memoryMap);
            //  Allocate memory with space for 1024 extra entries.
            memoryMapSize = memoryMapSize + (1024 * descriptorSize);
            memoryMap = (EFI_MEMORY_DESCRIPTOR *)malloc(memoryMapSize);
            continue;
        }
        if (ret != 0)
        {
            log_info("Failed to read memory map\n");
            break;
        }

        log_info("Read %d memory descriptors. Ver %d\n", memoryMapSize / descriptorSize, descriptorVer);

        ret = st->BootServices->ExitBootServices(handle, memoryMapKey);
        if (!ret)
            break;
    }

    if (ret != 0)
    {
        log_error("Failed to exit boot service! loop_counter=%d, error=%d\n", loop_counter, ret);
        return ret;
    }

    log_info("Bootservice left!\n");

    //  Identity map existing memory map
    for (auto memoryMap_cur = memoryMap; memoryMap_cur != NextMemoryDescriptor(memoryMap, memoryMapSize);
         memoryMap_cur = NextMemoryDescriptor(memoryMap_cur, descriptorSize))
    {
        memoryMap_cur->VirtualStart = memoryMap_cur->PhysicalStart;
    }

    auto memoryMap_cur = NextMemoryDescriptor(memoryMap, memoryMapSize);
    for (const auto &page : memory_map)
    {
        memoryMap_cur->Type = EfiRuntimeServicesData;
        memoryMap_cur->Pad = 0;
        memoryMap_cur->PhysicalStart = page.paddr;
        memoryMap_cur->VirtualStart = page.vaddr;
        memoryMap_cur->NumberOfPages = page.size / EFI_PAGE_SIZE;
        memoryMap_cur->Attribute = EFI_MEMORY_WB | EFI_MEMORY_WT | EFI_MEMORY_WC | EFI_MEMORY_UC;
        memoryMap_cur = NextMemoryDescriptor(memoryMap_cur, descriptorSize);
        memoryMapSize += descriptorSize;
    }
    st->RuntimeServices->ConvertPointer = ConvertPointer;

    ret = st->RuntimeServices->SetVirtualAddressMap(memoryMapSize, descriptorSize, descriptorVer, memoryMap);
    if (ret != 0)
    {
        log_error("Failed to set virtual memory map, error=%d\n", ret);
        return ret;
    }

    log_info("Enabled virtual memory!\n");
    //  separate memory map into ordered lists
    vector<physical_memory_map> memavailable;
    vector<physical_memory_map> memreserved;
    for (auto memoryMap_cur = memoryMap; memoryMap_cur != NextMemoryDescriptor(memoryMap, memoryMapSize);
         memoryMap_cur = NextMemoryDescriptor(memoryMap_cur, descriptorSize))
    {
        switch (memoryMap_cur->Type)
        {
        case EfiLoaderCode:
        case EfiLoaderData:
        case EfiBootServicesCode:
        case EfiBootServicesData:
        case EfiConventionalMemory: {
            auto mem_it = find_if(memavailable.begin(), memavailable.end(),
                                  [memoryMap_cur](const physical_memory_map &cur) { return cur.addr > memoryMap_cur->PhysicalStart; });
            mem_it = memavailable.insert(mem_it, {memoryMap_cur->PhysicalStart, memoryMap_cur->NumberOfPages});
        }
        break;
        case EfiMemoryMappedIO:
        case EfiMemoryMappedIOPortSpace:
            // break;
        case EfiReservedMemoryType:
        case EfiUnusableMemory:
        case EfiACPIReclaimMemory:
        case EfiACPIMemoryNVS:
        case EfiPalCode:
        case EfiPersistentMemory:
        case EfiRuntimeServicesCode:
        case EfiRuntimeServicesData:
        default: {
            auto mem_it = find_if(memreserved.begin(), memreserved.end(),
                                  [memoryMap_cur](const physical_memory_map &cur) { return cur.addr > memoryMap_cur->PhysicalStart; });
            mem_it = memreserved.insert(mem_it, {memoryMap_cur->PhysicalStart, memoryMap_cur->NumberOfPages});
        }
        break;
        }
    }
    //  merge overlapping blocks
    auto merge_fn = [](vector<physical_memory_map> &mem_list) {
        auto mem_it_end = mem_list.end();
        auto mem_it1 = mem_list.begin();
        auto mem_it2 = mem_it1 + 1;
        while (mem_it2 != mem_it_end)
        {
            if ((mem_it1->addr + (mem_it1->len * EFI_PAGE_SIZE)) >= mem_it2->addr)
            {
                mem_it1->len = ((mem_it2->addr + (mem_it2->len * EFI_PAGE_SIZE)) - mem_it1->addr) / EFI_PAGE_SIZE;
            }
            else
            {
                ++mem_it1;
                *mem_it1 = *mem_it2;
            }
            ++mem_it2;
        }
        mem_list.resize(mem_it1 - mem_list.begin() + 1);
    };
    merge_fn(memavailable);
    merge_fn(memreserved);
    (*mem_available_count.ptr) = 0;
    (*mem_reserved_count.ptr) = 0;

    log_debug("Free memory:\n");
    for (const auto &mem_it : memavailable)
    {
        log_debug("addr [%llx],npg [%llx]\n", mem_it.addr, mem_it.len);
    }
    copy(memavailable.begin(), memavailable.end(), mem_available.ptr);
    *mem_available_count.ptr = memavailable.size();

    log_debug("Reserved memory:\n");
    for (const auto &mem_it : memreserved)
    {
        log_debug("addr [%llx],npg [%llx]\n", mem_it.addr, mem_it.len);
    }
    copy(memreserved.begin(), memreserved.end(), mem_reserved.ptr);
    *mem_reserved_count.ptr = memreserved.size();

    *(fb.ptr) = get_framebuffer();

    log_info("Looking for ACPI Root Table\n");
    for (size_t i = 0; i < st->NumberOfTableEntries; i++)
    {
        if (st->ConfigurationTable[i].VendorGuid == (EFI_GUID)ACPI_TABLE_GUID)
        {
            log_info("Found ACPI 1.0 table!\n");
            memcpy(rsdp_table.ptr, st->ConfigurationTable[i].VendorTable, 20);
        }
        else if (st->ConfigurationTable[i].VendorGuid == (EFI_GUID)ACPI_20_TABLE_GUID)
        {
            log_info("Found ACPI 2.0 table!\n");
            memcpy(rsdp_table.ptr, st->ConfigurationTable[i].VendorTable, 36);
            //  ver. 2.0 is the highest we support.
            break;
        }
    }

    if (!root_uuid.ptr)
    {
        log_error("failed to get symbol root_partition_uuid\n");
        asm("cli;hlt");
        return -EUNK;
    }
    statfs_t statfs = {};
    ret = file_list[0]->v_vfsp->statfs(file_list[0]->v_vfsp->vfs_vnodecovered, statfs);
    if (ret)
    {
        log_error("failed to get statfs. error %d\n", ret);
        asm("cli;hlt");
        return ret;
    }
    *(root_uuid.ptr) = statfs.fsid;

    entry_point -= KERNEL_BASE_VADDR_PTR;
    entry_point += KERNEL_BASE_PADDR_PTR;

    log_info("Switch page table\n");
    log_info("Calling kernel entry point @ [%llx]\n", entry_point);

    asm volatile(".intel_syntax noprefix\n"
                 "mov rsp,rdx\n"
                 "jmp rax\n"
                 :
                 : "d"(kernel_stack_paddr + (EFI_PAGE_SIZE * 4)), "b"(pg), "a"(entry_point));

    return 0;
}

EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *system_table)
{
    EFI_STATUS status = 0;
    st = system_table;
    EFI_GUID guid = {};
    EFI_HANDLE handles[128] = {};
    UINTN handle_count = sizeof(handles);

    st->ConOut->ClearScreen(st->ConOut);
    st->ConOut->OutputString(st->ConOut, L"Loading Momentum...\r\n");

    if (init_video(st->ConOut))
    {
        st->ConOut->OutputString(st->ConOut, L"Failed to initilize_video()!\n");
        goto hang;
    }

    log_info("Firmware version: %d.%d\n", st->FirmwareRevision >> 16, st->FirmwareRevision & 0xFFFF);

    if (initilize_memorymanager(st))
    {
        log_error("Failed to initilize_memorymanager()!\n");
        goto hang;
    }

    if (init_paging())
    {
        log_error("Failed to initilize_memorymanager()!\n");
        goto hang;
    }

    CallConstructors();

    if (init_multitask())
    {
        log_error("Failed to init_multitask()!\n");
        goto hang;
    }

    if (init_vfs())
    {
        log_error("Failed to init_vfs()!\n");
        goto hang;
    }

    if (init_drivers())
    {
        log_error("Failed to init_drivers()!\n");
        goto hang;
    }

    guid = EFI_BLOCK_IO_PROTOCOL_GUID;
    status = st->BootServices->LocateHandle(ByProtocol, &guid, nullptr, &handle_count, handles);
    if (status)
    {
        log_error("Failed to get a list of block devices!\n");
        goto hang;
    }

    handle_count /= sizeof(EFI_HANDLE);
    log_info("Found %d block devices.\n", handle_count);

    for (size_t i = 0; i < handle_count; i++)
    {
        char drive_name[32] = {};
        snprintf(drive_name, sizeof(drive_name), "/dev/drive_%d", i);
        vnode *tmp = new efi_interface_vnode(handles[i]);
        mknod(drive_name, shared_ptr<vnode>(tmp));
    }

    if (find_kernel_and_boot(handle))
    {
        log_error("Failed to find_kernel_and_boot()!\n");
        goto hang;
    }

hang:
    while (true)
    {
        asm("cli;hlt");
    }
end:
    return 0;
}

extern "C" void _purecall()
{
    log_error("%s()\n", __FUNCTION__);
    asm("cli;hlt");
}
