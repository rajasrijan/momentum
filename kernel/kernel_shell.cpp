#include <kernel/kernel_shell.h>
#include <string>
#include <kernel/logging.h>
#include <kernel/vfs.h>
#include <kernel/vnode.h>
#include <kernel/multitask.h>
#include <kernel/binary_loader.h>
#include <DDI/pci_driver.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include <platform/acenv.h>
#include <actypes.h>
#include <acpi.h>
#ifdef __cplusplus
}
#endif

static int loglevel(char *args)
{
    int log_lvl = atoi(args);
    set_log_lvl(log_lvl);
    return 0;
}

int gui_init = 0;

static int gui(char *args)
{
    int ret = 0;
    // if (!gui_init) {
    //     gui_init = 1;
    //     ret = initilize_graphics();
    //     if (ret < 0) {
    //         printf("failed to init graphics\n");
    //         return ret;
    //     }
    // }
    return ret;
}

static int cat(char *filename)
{
    int ret = 0;
    vfile *file = nullptr;
    ret = open(filename, O_RDONLY, &file);
    if (ret < 0)
        return 1;
    char buffer[256] = {0};
    int bytesRead = 0;
    while ((bytesRead = file->read(buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = 0;
        printf(buffer);
    }
    return 0;
}

static int exec(char *cmd_line)
{
    int errorCode = 0;
    char filename[256] = {};
    process_t process = nullptr;
    shared_ptr<vnode> exec_node;
    vector<MemPage> memory_map;
    uint64_t entry_point = 0;
    for (int i = 0; (i < sizeof(filename)) && (cmd_line[i] != 0) && (cmd_line[i] != ' '); i++)
        filename[i] = cmd_line[i];
    errorCode = lookup(filename, exec_node);
    if (errorCode)
        goto error;
    if (!exec_node->isFile())
        return -ENOFILE;
    errorCode = binary_loader::load(exec_node, memory_map, entry_point);
    if (errorCode)
        goto error;
    //  create process
    errorCode = multitask::getInstance()->createProcess(process, exec_node->getName().c_str(), cmd_line, 3, memory_map, entry_point);
    if (errorCode)
        goto error;
error:
    return errorCode;
}

static int cd(char *args)
{
    return chdir(args);
}

static int ls(char *args)
{
    string path = (!args) ? "." : args;
    vector<string> dirs;
    int fd = openat(FDCWD, path.c_str(), O_RDONLY | O_NONBLOCK | O_DIRECTORY | O_CLOEXEC, 0);
    getdents(fd, dirs);
    close(fd);
    for (const string &entry : dirs)
    {
        printf("%s\n", entry.c_str());
    }
    return 0;
}

static int lspci([[maybe_unused]] char *args)
{
    printf("Connected PCI devices:\n");
    std::vector<pci_device_t> &deviceList = pci_getDevices();
    for (pci_device_t &device : deviceList)
    {
        pci_device_id devId = {0, 0, 0, 0, 0, 0, 0};
        const char *driver_name = nullptr;
        device.getDeviceId(&devId);
        if (device.pDriver && device.pDriver->name)
        {
            driver_name = device.pDriver->name;
        }
        else
        {
            driver_name = "None";
        }
        printf("\tPCI_%x_%x_%x_%x_%x_%x_%x\n", devId.VendorID, devId.DeviceID, devId.SubVendorID, devId.SubSystemID, devId.Class, devId.SubClass, devId.ProgIf);
        printf("\t\tDriver: %s\n", driver_name);
    }
    return 0;
}

static int lsmem([[maybe_unused]] char *args)
{
    int ret = 0;
    vector<MemPage> memMap;
    ret = PageManager::getMemoryMap(memMap);
    if (ret < 0)
    {
        printf("failed to getMemoryMap\n");
        asm("cli;hlt");
    }
    for (const auto &page : memMap)
    {
        printf("vaddr: [0x%lx], paddr: [0x%lx], size: [0x%lx]\n", page.vaddr, page.paddr, page.size);
    }
    return ret;
}

static int echo(char *args)
{
    if (!args)
        return 1;
    printf("%s\n", args);
    return 0;
}

static int top([[maybe_unused]] char *args)
{
    for (auto process : multitask::getInstance()->ListProcesses())
    {
        printf("%s\n", process->getName());
    }
    return 0;
}

static int poweroff([[maybe_unused]] char *args)
{
    AcpiEnterSleepStatePrep(5);
    asm("cli"); // disable interrupts
    AcpiEnterSleepState(5);
    printf("POWEROFF");
    asm("hlt"); // in case it didn't work!
    return 1;
}

int help([[maybe_unused]] char *args);

typedef std::pair<string, function<int(char *)>> cmd_t;

cmd_t cmdMapping[] = {
    {"echo", echo},   {"exec", exec},         {"cat", cat},           {"cd", cd},   {"gui", gui},   {"ls", ls}, {"lspci", lspci},
    {"lsmem", lsmem}, {"loglevel", loglevel}, {"poweroff", poweroff}, {"top", top}, {"help", help},
};

int help([[maybe_unused]] char *args)
{
    printf("Supported commands:\n");
    for (size_t i = 0; i < (sizeof(cmdMapping) / sizeof(cmdMapping[0])); i++)
    {
        printf("\t%s <args>\n", cmdMapping[i].first.c_str());
    }
    return 0;
}

int kernel_shell()
{
    char input[4096];
    log_info("\nKernel Shell:\n");
    while (true)
    {
        std::string curDir = getCurrentPath();
        log_info("(KS)%s", curDir.c_str());
        gets_s(input, sizeof(input) / sizeof(input[0]));
        char *cmd = input;
        char *subCmd = nullptr;
        for (int i = 0; input[i] != 0; i++)
        {
            if (input[i] == ' ')
            {
                input[i] = 0;
                if (input[i + 1] != 0)
                    subCmd = &input[i + 1];
                break;
            }
        }
        bool cmdFound = false;
        for (size_t i = 0; i < (sizeof(cmdMapping) / sizeof(cmdMapping[0])); i++)
        {
            if (cmdMapping[i].first == cmd)
            {
                auto cmd_ret = cmdMapping[i].second(subCmd);
                cmdFound = true;
                if (cmd_ret != 0)
                    log_error("command failed with error code [%d]\n", cmd_ret);
                break;
            }
        }
        if (!cmdFound)
        {
            log_error("Invalid command.\n");
        }
    }
    return 0;
}
