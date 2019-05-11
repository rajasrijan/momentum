#ifdef __cplusplus

#include <arch/x86_64/paging.h>
#include <arch/x86_64/global.h>
#include <arch/x86_64/timer.h>
#include <kernel/multitask.h>
#include <utility>
using namespace std;
extern "C"
{
#endif
#include <acpi.h>
#include <stdlib.h>
#include <stdio.h>

    /* Types for AcpiOsExecute */

    /*
 * OSL Initialization and shutdown primitives
 */
    ACPI_STATUS AcpiOsInitialize(void)
    {
        printf("Initializing ACPI OSL\n");
        return EXIT_SUCCESS;
    }

    ACPI_STATUS
    AcpiOsTerminate(
        void)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    /*
 * ACPI Table interfaces
 */
    ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer(void)
    {
        ACPI_PHYSICAL_ADDRESS Ret = 0;
        AcpiFindRootPointer(&Ret);
        return Ret;
    }

    ACPI_STATUS AcpiOsPredefinedOverride(const ACPI_PREDEFINED_NAMES *InitVal, ACPI_STRING *NewVal)
    {
        NewVal = NULL;
        return AE_NOT_IMPLEMENTED;
    }

    ACPI_STATUS AcpiOsTableOverride(ACPI_TABLE_HEADER *ExistingTable, ACPI_TABLE_HEADER **NewTable)
    {
        *NewTable = NULL;
        return AE_NOT_IMPLEMENTED;
    }

    ACPI_STATUS AcpiOsPhysicalTableOverride(ACPI_TABLE_HEADER *ExistingTable, ACPI_PHYSICAL_ADDRESS *NewAddress, UINT32 *NewTableLength)
    {
        NewAddress = NULL;
        NewTableLength = NULL;
        return AE_NOT_IMPLEMENTED;
    }

    /*
 * Spinlock primitives
 */
    ACPI_STATUS
    AcpiOsCreateLock(
        ACPI_SPINLOCK *OutHandle)
    {
        *OutHandle = (mtx_t *)malloc(sizeof(mtx_t));
        mtx_init(*OutHandle, 0);
        return 0;
    }

    void AcpiOsDeleteLock(
        ACPI_SPINLOCK Handle)
    {
        free(Handle);
    }

    ACPI_CPU_FLAGS AcpiOsAcquireLock(ACPI_SPINLOCK Handle)
    {
        mtx_lock(Handle);
        return 0;
    }

    void AcpiOsReleaseLock(ACPI_SPINLOCK Handle, ACPI_CPU_FLAGS Flags)
    {
        mtx_unlock(Handle);
    }

    /*
 * Semaphore primitives
 */
    ACPI_STATUS AcpiOsCreateSemaphore(UINT32 MaxUnits, UINT32 InitialUnits, ACPI_SEMAPHORE *OutHandle)
    {
        *OutHandle = (ACPI_SEMAPHORE)malloc(sizeof(sem_t));
        sem_init(*OutHandle, MaxUnits, InitialUnits);
        return EXIT_SUCCESS;
    }

    ACPI_STATUS
    AcpiOsDeleteSemaphore(
        ACPI_SEMAPHORE Handle)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    ACPI_STATUS
    AcpiOsWaitSemaphore(
        ACPI_SEMAPHORE Handle,
        UINT32 Units,
        UINT16 Timeout)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    ACPI_STATUS
    AcpiOsSignalSemaphore(
        ACPI_SEMAPHORE Handle,
        UINT32 Units)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    /*
 * Mutex primitives. May be configured to use semaphores instead via
 * ACPI_MUTEX_TYPE (see platform/acenv.h)
 */
    ACPI_STATUS
    AcpiOsCreateMutex(
        ACPI_MUTEX *OutHandle)
    {
        *OutHandle = (mtx_t *)malloc(sizeof(mtx_t));
        mtx_init(*OutHandle, 0);
        return 0;
    }

    void AcpiOsDeleteMutex(
        ACPI_MUTEX Handle)
    {
        free(Handle);
    }

    ACPI_STATUS
    AcpiOsAcquireMutex(ACPI_MUTEX Handle, UINT16 Timeout)
    {
        return mtx_trylock(Handle, Timeout);
    }

    void AcpiOsReleaseMutex(ACPI_MUTEX Handle)
    {
        mtx_unlock(Handle);
    }

    /*
 * Memory allocation and mapping
 */
    void *AcpiOsAllocate(ACPI_SIZE Size)
    {
        return calloc(1, Size);
    }

    void AcpiOsFree(
        void *Memory)
    {
        free(Memory);
    }

    void *AcpiOsMapMemory(ACPI_PHYSICAL_ADDRESS Where, ACPI_SIZE Length)
    {
        uint64_t paddr = ((Where / PageManager::PAGESIZE) * PageManager::PAGESIZE);
        uint64_t length = PageManager::roundToPageSize(Length + Where) - paddr;
        uint64_t offset = Where - paddr;
        uint64_t vaddr = 0;
        if (PageManager::getInstance()->findFreeVirtualMemory(vaddr, length, 0x40000000))
        {
            printf("Virtual Memory Full");
            asm("cli;hlt");
        }
        PageManager::getInstance()->setVirtualToPhysicalMemory(vaddr, paddr, length, PageManager::Supervisor, PageManager::Read_Write);
        return (void *)(vaddr + offset);
    }

    void AcpiOsUnmapMemory(void *LogicalAddress, ACPI_SIZE Size)
    {
        Size = PageManager::roundToPageSize(Size);
        PageManager::getInstance()->freeVirtualMemory((uint64_t)LogicalAddress, Size);
    }

    ACPI_STATUS AcpiOsGetPhysicalAddress(void *LogicalAddress, ACPI_PHYSICAL_ADDRESS *PhysicalAddress)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    /*
 * Interrupt handlers
 */
    pair<ACPI_OSD_HANDLER, void *> ServiceRoutines[256];
    void common_interrupt_handler(retStack_t *stack, general_registers_t *context)
    {
        auto &ServiceRoutine = ServiceRoutines[stack->interruptNumber];
        if (ServiceRoutine.first != nullptr)
        {
            ServiceRoutine.first(ServiceRoutine.second);
        }
    }

    ACPI_STATUS AcpiOsInstallInterruptHandler(UINT32 InterruptNumber, ACPI_OSD_HANDLER ServiceRoutine, void *Context)
    {
        ServiceRoutines[InterruptNumber] = make_pair(ServiceRoutine, Context);
        register_interrupt_handler(InterruptNumber, common_interrupt_handler);
        return 0;
    }

    ACPI_STATUS
    AcpiOsRemoveInterruptHandler(
        UINT32 InterruptNumber,
        ACPI_OSD_HANDLER ServiceRoutine)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    /*
 * Threads and Scheduling
 */
    ACPI_THREAD_ID AcpiOsGetThreadId(void)
    {
        return multitask::getInstance()->getKernelThread()->getThreadID();
    }

    ACPI_STATUS
    AcpiOsExecute(
        ACPI_EXECUTE_TYPE Type,
        ACPI_OSD_EXEC_CALLBACK Function,
        void *Context)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    void AcpiOsWaitEventsComplete(
        void)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
    }

    void AcpiOsSleep(
        UINT64 Milliseconds)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
    }

    void AcpiOsStall(
        UINT32 Microseconds)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
    }

    /*
 * Platform and hardware-independent I/O interfaces
 */
    ACPI_STATUS AcpiOsReadPort(ACPI_IO_ADDRESS Address, UINT32 *Value, UINT32 Width)
    {
        switch (Width)
        {
        case 8:
            *Value = inb(Address);
            break;
        case 16:
            *Value = inw(Address);
            break;
        case 32:
            *Value = inl(Address);
            break;
        default:
            printf("%s: Invalid width [%d]\n", __FUNCTION__, Width);
            __asm__("cli;hlt");
        }
        return 0;
    }

    ACPI_STATUS AcpiOsWritePort(ACPI_IO_ADDRESS Address, UINT32 Value, UINT32 Width)
    {
        switch (Width)
        {
        case 8:
            outb(Address, Value);
            break;
        case 16:
            outw(Address, Value);
            break;
        case 32:
            outl(Address, Value);
            break;
        default:
            printf("%s: Invalid width [%d]\n", __FUNCTION__, Width);
            __asm__("cli;hlt");
        }
        return 0;
    }

    /*
 * Platform and hardware-independent physical memory interfaces
 */
    ACPI_STATUS
    AcpiOsReadMemory(
        ACPI_PHYSICAL_ADDRESS Address,
        UINT64 *Value,
        UINT32 Width)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    ACPI_STATUS
    AcpiOsWriteMemory(
        ACPI_PHYSICAL_ADDRESS Address,
        UINT64 Value,
        UINT32 Width)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    /*
 * Platform and hardware-independent PCI configuration space access
 * Note: Can't use "Register" as a parameter, changed to "Reg" --
 * certain compilers complain.
 */
    ACPI_STATUS
    AcpiOsReadPciConfiguration(
        ACPI_PCI_ID *PciId,
        UINT32 Reg,
        UINT64 *Value,
        UINT32 Width)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    ACPI_STATUS
    AcpiOsWritePciConfiguration(
        ACPI_PCI_ID *PciId,
        UINT32 Reg,
        UINT64 Value,
        UINT32 Width)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    /*
 * Miscellaneous
 */
    BOOLEAN
    AcpiOsReadable(
        void *Pointer,
        ACPI_SIZE Length)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    BOOLEAN
    AcpiOsWritable(
        void *Pointer,
        ACPI_SIZE Length)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    UINT64 AcpiOsGetTimer(void)
    {
        return getSystemTime();
    }

    ACPI_STATUS
    AcpiOsSignal(
        UINT32 Function,
        void *Info)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    ACPI_STATUS AcpiOsEnterSleep(UINT8 SleepState, UINT32 RegaValue, UINT32 RegbValue)
    {
        printf("Entering sleep state.\n");
        return 0;
    }

    /*
 * Debug print routines
 */
    void ACPI_INTERNAL_VAR_XFACE AcpiOsPrintf(const char *Format, ...)
    {
        char buffer[1024];
        va_list arg;
        va_start(arg, Format);
        vsprintf(buffer, Format, arg);
        printf(buffer);
        va_end(arg);
    }

    void AcpiOsVprintf(const char *Format, va_list Args)
    {
        char buffer[1024];
        vsprintf(buffer, Format, Args);
        printf(buffer);
    }

    void AcpiOsRedirectOutput(
        void *Destination)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
    }

    /*
 * Debug IO
 */
    ACPI_STATUS
    AcpiOsGetLine(
        char *Buffer,
        UINT32 BufferLength,
        UINT32 *BytesRead)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    ACPI_STATUS
    AcpiOsInitializeDebugger(
        void)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    void AcpiOsTerminateDebugger(
        void)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
    }

    ACPI_STATUS
    AcpiOsWaitCommandReady(
        void)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    ACPI_STATUS
    AcpiOsNotifyCommandComplete(
        void)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    void AcpiOsTracePoint(
        ACPI_TRACE_EVENT_TYPE Type,
        BOOLEAN Begin,
        UINT8 *Aml,
        char *Pathname)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
    }

    /*
 * Obtain ACPI table(s)
 */
    ACPI_STATUS
    AcpiOsGetTableByName(
        char *Signature,
        UINT32 Instance,
        ACPI_TABLE_HEADER **Table,
        ACPI_PHYSICAL_ADDRESS *Address)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    ACPI_STATUS
    AcpiOsGetTableByIndex(
        UINT32 Index,
        ACPI_TABLE_HEADER **Table,
        UINT32 *Instance,
        ACPI_PHYSICAL_ADDRESS *Address)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    ACPI_STATUS
    AcpiOsGetTableByAddress(
        ACPI_PHYSICAL_ADDRESS Address,
        ACPI_TABLE_HEADER **Table)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return EXIT_FAILURE;
    }

    /*
 * Directory manipulation
 */
    void *
    AcpiOsOpenDirectory(
        char *Pathname,
        char *WildcardSpec,
        char RequestedFileType)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return nullptr;
    }

    char *
    AcpiOsGetNextFilename(
        void *DirHandle)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
        return nullptr;
    }

    void AcpiOsCloseDirectory(
        void *DirHandle)
    {
        printf("%s not impl\n", __FUNCTION__);
        __asm__("cli;hlt");
    }

#ifdef __cplusplus
}
#endif
