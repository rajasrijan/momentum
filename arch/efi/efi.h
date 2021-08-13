#ifndef EFI_H
#define EFI_H
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>

#define EFIAPI __attribute__((ms_abi))

// Bit definitions for EFI_TIME.Daylight
#define EFI_TIME_ADJUST_DAYLIGHT 0x01
#define EFI_TIME_IN_DAYLIGHT 0x02

// Value definition for EFI_TIME.TimeZone
#define EFI_UNSPECIFIED_TIMEZONE 0x07FF

// possible caching types for the memory range
#define EFI_MEMORY_UC 0x0000000000000001
#define EFI_MEMORY_WC 0x0000000000000002
#define EFI_MEMORY_WT 0x0000000000000004
#define EFI_MEMORY_WB 0x0000000000000008
#define EFI_MEMORY_UCE 0x0000000000000010

// physical memory protection on range
#define EFI_MEMORY_WP 0x0000000000001000
#define EFI_MEMORY_RP 0x0000000000002000
#define EFI_MEMORY_XP 0x0000000000004000

// range requires a runtime mapping
#define EFI_MEMORY_RUNTIME 0x8000000000000000

#define EFI_MEMORY_DESCRIPTOR_VERSION 1
#define ISO_639_2_ENTRY_SIZE 3

#define EFI_PAGE_SIZE 4096
#define EFI_PAGE_MASK 0xFFF
#define EFI_PAGE_SHIFT 12

#define EFI_SIZE_TO_PAGES(a) (((a) >> EFI_PAGE_SHIFT) + ((a)&EFI_PAGE_MASK ? 1 : 0))

#define EFI_OS_INDICATIONS_BOOT_TO_FW_UI 0x0000000000000001
#define EFI_OS_INDICATIONS_TIMESTAMP_REVOCATION 0x0000000000000002
#define EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED 0x0000000000000004
#define EFI_OS_INDICATIONS_FMP_CAPSULE_SUPPORTED 0x0000000000000008
#define EFI_OS_INDICATIONS_CAPSULE_RESULT_VAR_SUPPORTED 0x0000000000000010

    //
    // EFI Specification Revision
    //

#define EFI_SPECIFICATION_MAJOR_REVISION 1
#define EFI_SPECIFICATION_MINOR_REVISION 02

#define NextMemoryDescriptor(Ptr, Size) ((EFI_MEMORY_DESCRIPTOR *)(((UINT8 *)Ptr) + Size))

    //
    // EFI Events
    //

#define EVT_TIMER 0x80000000
#define EVT_RUNTIME 0x40000000
#define EVT_RUNTIME_CONTEXT 0x20000000

#define EVT_NOTIFY_WAIT 0x00000100
#define EVT_NOTIFY_SIGNAL 0x00000200

#define EVT_SIGNAL_EXIT_BOOT_SERVICES 0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

#define EVT_EFI_SIGNAL_MASK 0x000000FF
#define EVT_EFI_SIGNAL_MAX 4

#define EFI_EVENT_TIMER EVT_TIMER
#define EFI_EVENT_RUNTIME EVT_RUNTIME
#define EFI_EVENT_RUNTIME_CONTEXT EVT_RUNTIME_CONTEXT
#define EFI_EVENT_NOTIFY_WAIT EVT_NOTIFY_WAIT
#define EFI_EVENT_NOTIFY_SIGNAL EVT_NOTIFY_SIGNAL
#define EFI_EVENT_SIGNAL_EXIT_BOOT_SERVICES EVT_SIGNAL_EXIT_BOOT_SERVICES
#define EFI_EVENT_SIGNAL_VIRTUAL_ADDRESS_CHANGE EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE
#define EFI_EVENT_EFI_SIGNAL_MASK EVT_EFI_SIGNAL_MASK
#define EFI_EVENT_EFI_SIGNAL_MAX EVT_EFI_SIGNAL_MAX

// Variable attributes
#define EFI_VARIABLE_NON_VOLATILE 0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS 0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS 0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD 0x00000008
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS 0x00000010
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS 0x00000020
#define EFI_VARIABLE_APPEND_WRITE 0x00000040

// Variable size limitation
#define EFI_MAXIMUM_VARIABLE_SIZE 1024
    //
    // Image functions
    //

    // PE32+ Subsystem type for EFI images

#if !defined(IMAGE_SUBSYSTEM_EFI_APPLICATION)
#define IMAGE_SUBSYSTEM_EFI_APPLICATION 10
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER 11
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER 12
#endif

    // PE32+ Machine type for EFI images

#if !defined(EFI_IMAGE_MACHINE_IA32)
#define EFI_IMAGE_MACHINE_IA32 0x014c
#endif

#if !defined(EFI_IMAGE_MACHINE_IA64)
#define EFI_IMAGE_MACHINE_IA64 0x0200
#endif

#if !defined(EFI_IMAGE_MACHINE_EBC)
#define EFI_IMAGE_MACHINE_EBC 0x0EBC
#endif

#if !defined(EFI_IMAGE_MACHINE_X64)
#define EFI_IMAGE_MACHINE_X64 0x8664
#endif

#if !defined(EFI_IMAGE_MACHINE_ARMTHUMB_MIXED)
#define EFI_IMAGE_MACHINE_ARMTHUMB_MIXED 0x01C2
#endif

#if !defined(EFI_IMAGE_MACHINE_AARCH64)
#define EFI_IMAGE_MACHINE_AARCH64 0xAA64
#endif

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL 0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL 0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL 0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER 0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE 0x00000020

#define EFI_OPTIONAL_PTR 0x00000001
#define EFI_INTERNAL_FNC 0x00000002 // Pointer to internal runtime fnc
#define EFI_INTERNAL_PTR 0x00000004 // Pointer to internal runtime data

    //
    // Task priority level
    //

#define TPL_APPLICATION 4
#define TPL_CALLBACK 8
#define TPL_NOTIFY 16
#define TPL_HIGH_LEVEL 31
#define EFI_TPL_APPLICATION TPL_APPLICATION
#define EFI_TPL_CALLBACK TPL_CALLBACK
#define EFI_TPL_NOTIFY TPL_NOTIFY
#define EFI_TPL_HIGH_LEVEL TPL_HIGH_LEVEL

#define EFI_GLOBAL_VARIABLE                                                                                                                                    \
    {                                                                                                                                                          \
        0x8BE4DF61, 0x93CA, 0x11d2,                                                                                                                            \
        {                                                                                                                                                      \
            0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C                                                                                                     \
        }                                                                                                                                                      \
    }

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET 0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE 0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET 0x00040000

#define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544e5552
#define EFI_RUNTIME_SERVICES_REVISION (EFI_SPECIFICATION_MAJOR_REVISION << 16) | (EFI_SPECIFICATION_MINOR_REVISION)

#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION (EFI_SPECIFICATION_MAJOR_REVISION << 16) | (EFI_SPECIFICATION_MINOR_REVISION)

    //
    // EFI Configuration Table and GUID definitions
    //

#define MPS_TABLE_GUID                                                                                                                                         \
    {                                                                                                                                                          \
        0xeb9d2d2f, 0x2d88, 0x11d3,                                                                                                                            \
        {                                                                                                                                                      \
            0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d                                                                                                      \
        }                                                                                                                                                      \
    }

#define ACPI_TABLE_GUID                                                                                                                                        \
    {                                                                                                                                                          \
        0xeb9d2d30, 0x2d88, 0x11d3,                                                                                                                            \
        {                                                                                                                                                      \
            0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d                                                                                                      \
        }                                                                                                                                                      \
    }

#define ACPI_20_TABLE_GUID                                                                                                                                     \
    {                                                                                                                                                          \
        0x8868e871, 0xe4f1, 0x11d3,                                                                                                                            \
        {                                                                                                                                                      \
            0xbc, 0x22, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81                                                                                                      \
        }                                                                                                                                                      \
    }

#define SMBIOS_TABLE_GUID                                                                                                                                      \
    {                                                                                                                                                          \
        0xeb9d2d31, 0x2d88, 0x11d3,                                                                                                                            \
        {                                                                                                                                                      \
            0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d                                                                                                      \
        }                                                                                                                                                      \
    }

#define SMBIOS3_TABLE_GUID                                                                                                                                     \
    {                                                                                                                                                          \
        0xf2fd1544, 0x9794, 0x4a2c,                                                                                                                            \
        {                                                                                                                                                      \
            0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94                                                                                                     \
        }                                                                                                                                                      \
    }

#define SAL_SYSTEM_TABLE_GUID                                                                                                                                  \
    {                                                                                                                                                          \
        0xeb9d2d32, 0x2d88, 0x11d3,                                                                                                                            \
        {                                                                                                                                                      \
            0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d                                                                                                      \
        }                                                                                                                                                      \
    }

#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249
#define EFI_SYSTEM_TABLE_REVISION (EFI_SPECIFICATION_MAJOR_REVISION << 16) | (EFI_SPECIFICATION_MINOR_REVISION)

//
//  EFI error codes
//
#define EFI_SUCCESS 0           /*Success*/
#define EFI_LOAD_ERROR 1        /*The image failed to load.*/
#define EFI_INVALID_PARAMETER 2 /*A parameter was incorrect.*/
#define EFI_UNSUPPORTED 3       /*The operation is not supported.*/
#define EFI_BAD_BUFFER_SIZE 4   /*The buffer was not the proper size for the request.*/
#define EFI_BUFFER_TOO_SMALL                                                                                                                                   \
    5 /*The buffer is not large enough to hold the requested data. The required buffer size is returned in the appropriate parameter when this error occurs.*/
#define EFI_NOT_FOUND 14 /*The item was not found.*/

    typedef uint8_t UINT8;
    typedef uint16_t UINT16;
    typedef uint32_t UINT32;
    typedef uint64_t UINT64;
    typedef uint64_t UINTN;
    typedef int8_t INT8;
    typedef int16_t INT16;
    typedef int32_t INT32;
    typedef int64_t INT64;
    typedef int64_t INTN;
    typedef void VOID;
    typedef wchar_t CHAR16;
    typedef UINT8 CHAR8;
    typedef UINT8 BOOLEAN;
    typedef UINTN EFI_STATUS;
    typedef UINT64 EFI_LBA;
    typedef UINTN EFI_TPL;
    typedef VOID *EFI_HANDLE;
    typedef VOID *EFI_EVENT;
    typedef struct _EFI_DEVICE_PATH_PROTOCOL
    {
        UINT8 Type;
        UINT8 SubType;
        UINT8 Length[2];
    } EFI_DEVICE_PATH_PROTOCOL;
    typedef struct _EFI_DEVICE_PATH_PROTOCOL _EFI_DEVICE_PATH;
    typedef EFI_DEVICE_PATH_PROTOCOL EFI_DEVICE_PATH;
    typedef struct
    {
        UINT32 Data1;
        UINT16 Data2;
        UINT16 Data3;
        UINT8 Data4[8];
    } EFI_GUID;
    typedef struct
    {
        UINT16 Year;  // 1998 - 20XX
        UINT8 Month;  // 1 - 12
        UINT8 Day;    // 1 - 31
        UINT8 Hour;   // 0 - 23
        UINT8 Minute; // 0 - 59
        UINT8 Second; // 0 - 59
        UINT8 Pad1;
        UINT32 Nanosecond; // 0 - 999,999,999
        INT16 TimeZone;    // -1440 to 1440 or 2047
        UINT8 Daylight;
        UINT8 Pad2;
    } EFI_TIME;
    //
    // Networking
    //

    typedef struct
    {
        UINT8 Addr[4];
    } EFI_IPv4_ADDRESS;
    typedef struct
    {
        UINT8 Addr[16];
    } EFI_IPv6_ADDRESS;
    typedef struct
    {
        UINT8 Addr[32];
    } EFI_MAC_ADDRESS;
    typedef struct
    {
        UINT32 ReceivedQueueTimeoutValue;
        UINT32 TransmitQueueTimeoutValue;
        UINT16 ProtocolTypeFilter;
        BOOLEAN EnableUnicastReceive;
        BOOLEAN EnableMulticastReceive;
        BOOLEAN EnableBroadcastReceive;
        BOOLEAN EnablePromiscuousReceive;
        BOOLEAN FlushQueuesOnReset;
        BOOLEAN EnableReceiveTimestamps;
        BOOLEAN DisableBackgroundPolling;
    } EFI_MANAGED_NETWORK_CONFIG_DATA;
    //
    // Memory
    //

    typedef UINT64 EFI_PHYSICAL_ADDRESS;
    typedef UINT64 EFI_VIRTUAL_ADDRESS;
    typedef enum
    {
        AllocateAnyPages,
        AllocateMaxAddress,
        AllocateAddress,
        MaxAllocateType
    } EFI_ALLOCATE_TYPE;
    // Preseve the attr on any range supplied.
    // ConventialMemory must have WB,SR,SW when supplied.
    // When allocating from ConventialMemory always make it WB,SR,SW
    // When returning to ConventialMemory always make it WB,SR,SW
    // When getting the memory map, or on RT for runtime types

    typedef enum
    {
        EfiReservedMemoryType,
        EfiLoaderCode,
        EfiLoaderData,
        EfiBootServicesCode,
        EfiBootServicesData,
        EfiRuntimeServicesCode,
        EfiRuntimeServicesData,
        EfiConventionalMemory,
        EfiUnusableMemory,
        EfiACPIReclaimMemory,
        EfiACPIMemoryNVS,
        EfiMemoryMappedIO,
        EfiMemoryMappedIOPortSpace,
        EfiPalCode,
        EfiPersistentMemory,
        EfiMaxMemoryType
    } EFI_MEMORY_TYPE;

    typedef struct
    {
        UINT32 Type; // Field size is 32 bits followed by 32 bit pad
        UINT32 Pad;
        EFI_PHYSICAL_ADDRESS PhysicalStart; // Field size is 64 bits
        EFI_VIRTUAL_ADDRESS VirtualStart;   // Field size is 64 bits
        UINT64 NumberOfPages;               // Field size is 64 bits
        UINT64 Attribute;                   // Field size is 64 bits
    } EFI_MEMORY_DESCRIPTOR;
    //
    // International Language
    //

    typedef UINT8 ISO_639_2;
    //
    // Declare forward referenced data structures
    //

    struct _EFI_SYSTEM_TABLE;
    //
    // EFI Memory
    //

    typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_PAGES)(EFI_ALLOCATE_TYPE Type, EFI_MEMORY_TYPE MemoryType, UINTN NoPages, EFI_PHYSICAL_ADDRESS *Memory);
    typedef EFI_STATUS(EFIAPI *EFI_FREE_PAGES)(EFI_PHYSICAL_ADDRESS Memory, UINTN NoPages);
    typedef EFI_STATUS(EFIAPI *EFI_GET_MEMORY_MAP)(UINTN *MemoryMapSize, EFI_MEMORY_DESCRIPTOR *MemoryMap, UINTN *MapKey, UINTN *DescriptorSize,
                                                   UINT32 *DescriptorVersion);
    typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_POOL)(EFI_MEMORY_TYPE PoolType, UINTN Size, VOID **Buffer);
    typedef EFI_STATUS(EFIAPI *EFI_FREE_POOL)(VOID *Buffer);
    typedef EFI_STATUS(EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP)(UINTN MemoryMapSize, UINTN DescriptorSize, UINT32 DescriptorVersion,
                                                            EFI_MEMORY_DESCRIPTOR *VirtualMap);
    typedef EFI_STATUS(EFIAPI *EFI_CONVERT_POINTER)(UINTN DebugDisposition, VOID **Address);
    typedef VOID(EFIAPI *EFI_EVENT_NOTIFY)(EFI_EVENT Event, VOID *Context);
    typedef EFI_STATUS(EFIAPI *EFI_CREATE_EVENT)(UINT32 Type, EFI_TPL NotifyTpl, EFI_EVENT_NOTIFY NotifyFunction, VOID *NotifyContext, EFI_EVENT *Event);
    typedef enum
    {
        TimerCancel,
        TimerPeriodic,
        TimerRelative,
        TimerTypeMax
    } EFI_TIMER_DELAY;
    typedef EFI_STATUS(EFIAPI *EFI_SET_TIMER)(EFI_EVENT Event, EFI_TIMER_DELAY Type, UINT64 TriggerTime);
    typedef EFI_STATUS(EFIAPI *EFI_SIGNAL_EVENT)(EFI_EVENT Event);
    typedef EFI_STATUS(EFIAPI *EFI_WAIT_FOR_EVENT)(UINTN NumberOfEvents, EFI_EVENT *Event, UINTN *Index);
    typedef EFI_STATUS(EFIAPI *EFI_CLOSE_EVENT)(EFI_EVENT Event);
    typedef EFI_STATUS(EFIAPI *EFI_CHECK_EVENT)(EFI_EVENT Event);
    typedef EFI_TPL(EFIAPI *EFI_RAISE_TPL)(EFI_TPL NewTpl);
    typedef VOID(EFIAPI *EFI_RESTORE_TPL)(EFI_TPL OldTpl);
    //
    // EFI platform varibles
    //
    typedef EFI_STATUS(EFIAPI *EFI_GET_VARIABLE)(CHAR16 *VariableName, EFI_GUID *VendorGuid, UINT32 *Attributes, UINTN *DataSize, VOID *Data);
    typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_VARIABLE_NAME)(UINTN *VariableNameSize, CHAR16 *VariableName, EFI_GUID *VendorGuid);
    typedef EFI_STATUS(EFIAPI *EFI_SET_VARIABLE)(CHAR16 *VariableName, EFI_GUID *VendorGuid, UINT32 Attributes, UINTN DataSize, VOID *Data);
    //
    // EFI Time
    //

    typedef struct
    {
        UINT32 Resolution;  // 1e-6 parts per million
        UINT32 Accuracy;    // hertz
        BOOLEAN SetsToZero; // Set clears sub-second time
    } EFI_TIME_CAPABILITIES;
    typedef EFI_STATUS(EFIAPI *EFI_GET_TIME)(EFI_TIME *Time, EFI_TIME_CAPABILITIES *Capabilities);
    typedef EFI_STATUS(EFIAPI *EFI_SET_TIME)(EFI_TIME *Time);
    typedef EFI_STATUS(EFIAPI *EFI_GET_WAKEUP_TIME)(BOOLEAN *Enabled, BOOLEAN *Pending, EFI_TIME *Time);
    typedef EFI_STATUS(EFIAPI *EFI_SET_WAKEUP_TIME)(BOOLEAN Enable, EFI_TIME *Time);
    // Image Entry prototype

    typedef EFI_STATUS(EFIAPI *EFI_IMAGE_ENTRY_POINT)(EFI_HANDLE ImageHandle, struct _EFI_SYSTEM_TABLE *SystemTable);
    typedef EFI_STATUS(EFIAPI *EFI_IMAGE_LOAD)(BOOLEAN BootPolicy, EFI_HANDLE ParentImageHandle, EFI_DEVICE_PATH *FilePath, VOID *SourceBuffer,
                                               UINTN SourceSize, EFI_HANDLE *ImageHandle);
    typedef EFI_STATUS(EFIAPI *EFI_IMAGE_START)(EFI_HANDLE ImageHandle, UINTN *ExitDataSize, CHAR16 **ExitData);
    typedef EFI_STATUS(EFIAPI *EFI_EXIT)(EFI_HANDLE ImageHandle, EFI_STATUS ExitStatus, UINTN ExitDataSize, CHAR16 *ExitData);
    typedef EFI_STATUS(EFIAPI *EFI_EXIT_BOOT_SERVICES)(EFI_HANDLE ImageHandle, UINTN MapKey);
    //
    // Misc
    //

    typedef EFI_STATUS(EFIAPI *EFI_STALL)(UINTN Microseconds);
    typedef EFI_STATUS(EFIAPI *EFI_SET_WATCHDOG_TIMER)(UINTN Timeout, UINT64 WatchdogCode, UINTN DataSize, CHAR16 *WatchdogData);
    typedef EFI_STATUS(EFIAPI *EFI_CONNECT_CONTROLLER)(EFI_HANDLE ControllerHandle, EFI_HANDLE *DriverImageHandle, EFI_DEVICE_PATH *RemainingDevicePath,
                                                       BOOLEAN Recursive);
    typedef EFI_STATUS(EFIAPI *EFI_DISCONNECT_CONTROLLER)(EFI_HANDLE ControllerHandle, EFI_HANDLE DriverImageHandle, EFI_HANDLE ChildHandle);
    typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL)(EFI_HANDLE Handle, EFI_GUID *Protocol, VOID **Interface, EFI_HANDLE AgentHandle, EFI_HANDLE ControllerHandle,
                                                  UINT32 Attributes);
    typedef EFI_STATUS(EFIAPI *EFI_CLOSE_PROTOCOL)(EFI_HANDLE Handle, EFI_GUID *Protocol, EFI_HANDLE AgentHandle, EFI_HANDLE ControllerHandle);
    typedef struct
    {
        EFI_HANDLE AgentHandle;
        EFI_HANDLE ControllerHandle;
        UINT32 Attributes;
        UINT32 OpenCount;
    } EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;
    typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION)(EFI_HANDLE Handle, EFI_GUID *Protocol, EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
                                                              UINTN *EntryCount);
    typedef EFI_STATUS(EFIAPI *EFI_PROTOCOLS_PER_HANDLE)(EFI_HANDLE Handle, EFI_GUID ***ProtocolBuffer, UINTN *ProtocolBufferCount);
    typedef enum
    {
        AllHandles,
        ByRegisterNotify,
        ByProtocol
    } EFI_LOCATE_SEARCH_TYPE;
    typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE_BUFFER)(EFI_LOCATE_SEARCH_TYPE SearchType, EFI_GUID *Protocol, VOID *SearchKey, UINTN *NoHandles,
                                                         EFI_HANDLE **Buffer);
    typedef EFI_STATUS(EFIAPI *EFI_LOCATE_PROTOCOL)(EFI_GUID *Protocol, VOID *Registration, VOID **Interface);
    typedef EFI_STATUS(EFIAPI *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES)(EFI_HANDLE *Handle, ...);
    typedef EFI_STATUS(EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES)(EFI_HANDLE Handle, ...);
    typedef EFI_STATUS(EFIAPI *EFI_CALCULATE_CRC32)(VOID *Data, UINTN DataSize, UINT32 *Crc32);
    typedef VOID(EFIAPI *EFI_COPY_MEM)(VOID *Destination, VOID *Source, UINTN Length);
    typedef VOID(EFIAPI *EFI_SET_MEM)(VOID *Buffer, UINTN Size, UINT8 Value);
    typedef EFI_STATUS(EFIAPI *EFI_CREATE_EVENT_EX)(UINT32 Type, EFI_TPL NotifyTpl, EFI_EVENT_NOTIFY NotifyFunction, const VOID *NotifyContext,
                                                    const EFI_GUID EventGroup, EFI_EVENT *Event);
    typedef enum
    {
        EfiResetCold,
        EfiResetWarm,
        EfiResetShutdown
    } EFI_RESET_TYPE;
    typedef EFI_STATUS(EFIAPI *EFI_RESET_SYSTEM)(EFI_RESET_TYPE ResetType, EFI_STATUS ResetStatus, UINTN DataSize, CHAR16 *ResetData);
    typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT)(UINT64 *Count);
    typedef EFI_STATUS(EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT)(UINT32 *HighCount);
    typedef struct
    {
        UINT64 Length;
        union {
            EFI_PHYSICAL_ADDRESS DataBlock;
            EFI_PHYSICAL_ADDRESS ContinuationPointer;
        } Union;
    } EFI_CAPSULE_BLOCK_DESCRIPTOR;
    typedef struct
    {
        EFI_GUID CapsuleGuid;
        UINT32 HeaderSize;
        UINT32 Flags;
        UINT32 CapsuleImageSize;
    } EFI_CAPSULE_HEADER;
    typedef EFI_STATUS(EFIAPI *EFI_UPDATE_CAPSULE)(EFI_CAPSULE_HEADER **CapsuleHeaderArray, UINTN CapsuleCount, EFI_PHYSICAL_ADDRESS ScatterGatherList);
    typedef EFI_STATUS(EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES)(EFI_CAPSULE_HEADER **CapsuleHeaderArray, UINTN CapsuleCount, UINT64 *MaximumCapsuleSize,
                                                               EFI_RESET_TYPE *ResetType);
    typedef EFI_STATUS(EFIAPI *EFI_QUERY_VARIABLE_INFO)(UINT32 Attributes, UINT64 *MaximumVariableStorageSize, UINT64 *RemainingVariableStorageSize,
                                                        UINT64 *MaximumVariableSize);
    //
    // Protocol handler functions
    //

    typedef enum
    {
        EFI_NATIVE_INTERFACE,
        EFI_PCODE_INTERFACE
    } EFI_INTERFACE_TYPE;
    typedef EFI_STATUS(EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE *Handle, EFI_GUID *Protocol, EFI_INTERFACE_TYPE InterfaceType, VOID *Interface);
    typedef EFI_STATUS(EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE Handle, EFI_GUID *Protocol, VOID *OldInterface, VOID *NewInterface);
    typedef EFI_STATUS(EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE)(EFI_HANDLE Handle, EFI_GUID *Protocol, VOID *Interface);
    typedef EFI_STATUS(EFIAPI *EFI_HANDLE_PROTOCOL)(EFI_HANDLE Handle, EFI_GUID *Protocol, VOID **Interface);
    typedef EFI_STATUS(EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY)(EFI_GUID *Protocol, EFI_EVENT Event, VOID **Registration);
    typedef EFI_STATUS(EFIAPI *EFI_LOCATE_HANDLE)(EFI_LOCATE_SEARCH_TYPE SearchType, EFI_GUID *Protocol, VOID *SearchKey, UINTN *BufferSize,
                                                  EFI_HANDLE *Buffer);
    typedef EFI_STATUS(EFIAPI *EFI_LOCATE_DEVICE_PATH)(EFI_GUID *Protocol, EFI_DEVICE_PATH **DevicePath, EFI_HANDLE *Device);
    typedef EFI_STATUS(EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE)(EFI_GUID *Guid, VOID *Table);
    typedef EFI_STATUS(EFIAPI *EFI_RESERVED_SERVICE)();
    //
    // Text output protocol
    //

#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID                                                                                                                   \
    {                                                                                                                                                          \
        0x387477c2, 0x69c7, 0x11d2,                                                                                                                            \
        {                                                                                                                                                      \
            0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                                                                                                      \
        }                                                                                                                                                      \
    }
#define SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID

    struct _SIMPLE_TEXT_OUTPUT_INTERFACE;
    typedef EFI_STATUS(EFIAPI *EFI_TEXT_RESET)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE *This, BOOLEAN ExtendedVerification);
    typedef EFI_STATUS(EFIAPI *EFI_TEXT_OUTPUT_STRING)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE *This, CHAR16 *String);
    typedef EFI_STATUS(EFIAPI *EFI_TEXT_TEST_STRING)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE *This, CHAR16 *String);
    typedef EFI_STATUS(EFIAPI *EFI_TEXT_QUERY_MODE)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE *This, UINTN ModeNumber, UINTN *Columns, UINTN *Rows);
    typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_MODE)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE *This, UINTN ModeNumber);
    typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE *This, UINTN Attribute);
#define EFI_BLACK 0x00
#define EFI_BLUE 0x01
#define EFI_GREEN 0x02
#define EFI_CYAN (EFI_BLUE | EFI_GREEN)
#define EFI_RED 0x04
#define EFI_MAGENTA (EFI_BLUE | EFI_RED)
#define EFI_BROWN (EFI_GREEN | EFI_RED)
#define EFI_LIGHTGRAY (EFI_BLUE | EFI_GREEN | EFI_RED)
#define EFI_BRIGHT 0x08
#define EFI_DARKGRAY (EFI_BRIGHT)
#define EFI_LIGHTBLUE (EFI_BLUE | EFI_BRIGHT)
#define EFI_LIGHTGREEN (EFI_GREEN | EFI_BRIGHT)
#define EFI_LIGHTCYAN (EFI_CYAN | EFI_BRIGHT)
#define EFI_LIGHTRED (EFI_RED | EFI_BRIGHT)
#define EFI_LIGHTMAGENTA (EFI_MAGENTA | EFI_BRIGHT)
#define EFI_YELLOW (EFI_BROWN | EFI_BRIGHT)
#define EFI_WHITE (EFI_BLUE | EFI_GREEN | EFI_RED | EFI_BRIGHT)

#define EFI_TEXT_ATTR(f, b) ((f) | ((b) << 4))

#define EFI_BACKGROUND_BLACK 0x00
#define EFI_BACKGROUND_BLUE 0x10
#define EFI_BACKGROUND_GREEN 0x20
#define EFI_BACKGROUND_CYAN (EFI_BACKGROUND_BLUE | EFI_BACKGROUND_GREEN)
#define EFI_BACKGROUND_RED 0x40
#define EFI_BACKGROUND_MAGENTA (EFI_BACKGROUND_BLUE | EFI_BACKGROUND_RED)
#define EFI_BACKGROUND_BROWN (EFI_BACKGROUND_GREEN | EFI_BACKGROUND_RED)
#define EFI_BACKGROUND_LIGHTGRAY (EFI_BACKGROUND_BLUE | EFI_BACKGROUND_GREEN | EFI_BACKGROUND_RED)

    typedef EFI_STATUS(EFIAPI *EFI_TEXT_CLEAR_SCREEN)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE *This);
    typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_CURSOR_POSITION)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE *This, UINTN Column, UINTN Row);
    typedef EFI_STATUS(EFIAPI *EFI_TEXT_ENABLE_CURSOR)(struct _SIMPLE_TEXT_OUTPUT_INTERFACE *This, BOOLEAN Enable);
    typedef struct
    {
        INT32 MaxMode;
        // current settings
        INT32 Mode;
        INT32 Attribute;
        INT32 CursorColumn;
        INT32 CursorRow;
        BOOLEAN CursorVisible;
    } SIMPLE_TEXT_OUTPUT_MODE;
    typedef struct _SIMPLE_TEXT_OUTPUT_INTERFACE
    {
        EFI_TEXT_RESET Reset;
        EFI_TEXT_OUTPUT_STRING OutputString;
        EFI_TEXT_TEST_STRING TestString;
        EFI_TEXT_QUERY_MODE QueryMode;
        EFI_TEXT_SET_MODE SetMode;
        EFI_TEXT_SET_ATTRIBUTE SetAttribute;
        EFI_TEXT_CLEAR_SCREEN ClearScreen;
        EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
        EFI_TEXT_ENABLE_CURSOR EnableCursor;
        // Current mode
        SIMPLE_TEXT_OUTPUT_MODE *Mode;
    } SIMPLE_TEXT_OUTPUT_INTERFACE, EFI_SIMPLE_TEXT_OUT_PROTOCOL;
    //
    // Define's for required EFI Unicode Box Draw character
    //

#define BOXDRAW_HORIZONTAL 0x2500
#define BOXDRAW_VERTICAL 0x2502
#define BOXDRAW_DOWN_RIGHT 0x250c
#define BOXDRAW_DOWN_LEFT 0x2510
#define BOXDRAW_UP_RIGHT 0x2514
#define BOXDRAW_UP_LEFT 0x2518
#define BOXDRAW_VERTICAL_RIGHT 0x251c
#define BOXDRAW_VERTICAL_LEFT 0x2524
#define BOXDRAW_DOWN_HORIZONTAL 0x252c
#define BOXDRAW_UP_HORIZONTAL 0x2534
#define BOXDRAW_VERTICAL_HORIZONTAL 0x253c

#define BOXDRAW_DOUBLE_HORIZONTAL 0x2550
#define BOXDRAW_DOUBLE_VERTICAL 0x2551
#define BOXDRAW_DOWN_RIGHT_DOUBLE 0x2552
#define BOXDRAW_DOWN_DOUBLE_RIGHT 0x2553
#define BOXDRAW_DOUBLE_DOWN_RIGHT 0x2554

#define BOXDRAW_DOWN_LEFT_DOUBLE 0x2555
#define BOXDRAW_DOWN_DOUBLE_LEFT 0x2556
#define BOXDRAW_DOUBLE_DOWN_LEFT 0x2557

#define BOXDRAW_UP_RIGHT_DOUBLE 0x2558
#define BOXDRAW_UP_DOUBLE_RIGHT 0x2559
#define BOXDRAW_DOUBLE_UP_RIGHT 0x255a

#define BOXDRAW_UP_LEFT_DOUBLE 0x255b
#define BOXDRAW_UP_DOUBLE_LEFT 0x255c
#define BOXDRAW_DOUBLE_UP_LEFT 0x255d

#define BOXDRAW_VERTICAL_RIGHT_DOUBLE 0x255e
#define BOXDRAW_VERTICAL_DOUBLE_RIGHT 0x255f
#define BOXDRAW_DOUBLE_VERTICAL_RIGHT 0x2560

#define BOXDRAW_VERTICAL_LEFT_DOUBLE 0x2561
#define BOXDRAW_VERTICAL_DOUBLE_LEFT 0x2562
#define BOXDRAW_DOUBLE_VERTICAL_LEFT 0x2563

#define BOXDRAW_DOWN_HORIZONTAL_DOUBLE 0x2564
#define BOXDRAW_DOWN_DOUBLE_HORIZONTAL 0x2565
#define BOXDRAW_DOUBLE_DOWN_HORIZONTAL 0x2566

#define BOXDRAW_UP_HORIZONTAL_DOUBLE 0x2567
#define BOXDRAW_UP_DOUBLE_HORIZONTAL 0x2568
#define BOXDRAW_DOUBLE_UP_HORIZONTAL 0x2569

#define BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE 0x256a
#define BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL 0x256b
#define BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL 0x256c

    //
    // EFI Required Block Elements Code Chart
    //

#define BLOCKELEMENT_FULL_BLOCK 0x2588
#define BLOCKELEMENT_LIGHT_SHADE 0x2591
    //
    // EFI Required Geometric Shapes Code Chart
    //

#define GEOMETRICSHAPE_UP_TRIANGLE 0x25b2
#define GEOMETRICSHAPE_RIGHT_TRIANGLE 0x25ba
#define GEOMETRICSHAPE_DOWN_TRIANGLE 0x25bc
#define GEOMETRICSHAPE_LEFT_TRIANGLE 0x25c4

    //
    // EFI Required Arrow shapes
    //

#define ARROW_UP 0x2191
#define ARROW_DOWN 0x2193

    //
    // Text input protocol
    //

#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID                                                                                                                    \
    {                                                                                                                                                          \
        0x387477c1, 0x69c7, 0x11d2,                                                                                                                            \
        {                                                                                                                                                      \
            0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                                                                                                      \
        }                                                                                                                                                      \
    }
#define SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID

    struct _SIMPLE_INPUT_INTERFACE;
    typedef struct
    {
        UINT16 ScanCode;
        CHAR16 UnicodeChar;
    } EFI_INPUT_KEY;
    //
    // Baseline unicode control chars
    //

#define CHAR_NULL 0x0000
#define CHAR_BACKSPACE 0x0008
#define CHAR_TAB 0x0009
#define CHAR_LINEFEED 0x000A
#define CHAR_CARRIAGE_RETURN 0x000D

    //
    // Scan codes for base line keys
    //

#define SCAN_NULL 0x0000
#define SCAN_UP 0x0001
#define SCAN_DOWN 0x0002
#define SCAN_RIGHT 0x0003
#define SCAN_LEFT 0x0004
#define SCAN_HOME 0x0005
#define SCAN_END 0x0006
#define SCAN_INSERT 0x0007
#define SCAN_DELETE 0x0008
#define SCAN_PAGE_UP 0x0009
#define SCAN_PAGE_DOWN 0x000A
#define SCAN_F1 0x000B
#define SCAN_F2 0x000C
#define SCAN_F3 0x000D
#define SCAN_F4 0x000E
#define SCAN_F5 0x000F
#define SCAN_F6 0x0010
#define SCAN_F7 0x0011
#define SCAN_F8 0x0012
#define SCAN_F9 0x0013
#define SCAN_F10 0x0014
#define SCAN_F11 0x0015
#define SCAN_F12 0x0016
#define SCAN_ESC 0x0017

    typedef EFI_STATUS(EFIAPI *EFI_INPUT_RESET)(struct _SIMPLE_INPUT_INTERFACE *This, BOOLEAN ExtendedVerification);
    typedef EFI_STATUS(EFIAPI *EFI_INPUT_READ_KEY)(struct _SIMPLE_INPUT_INTERFACE *This, EFI_INPUT_KEY *Key);
    typedef struct _SIMPLE_INPUT_INTERFACE
    {
        EFI_INPUT_RESET Reset;
        EFI_INPUT_READ_KEY ReadKeyStroke;
        EFI_EVENT WaitForKey;
    } SIMPLE_INPUT_INTERFACE, EFI_SIMPLE_TEXT_IN_PROTOCOL;
    //
    // Standard EFI table header
    //

    typedef struct _EFI_TABLE_HEADER
    {
        UINT64 Signature;
        UINT32 Revision;
        UINT32 HeaderSize;
        UINT32 CRC32;
        UINT32 Reserved;
    } EFI_TABLE_HEADER;
    //
    // EFI Runtime Serivces Table
    //

    typedef struct
    {
        EFI_TABLE_HEADER Hdr;
        //
        // Time services
        //

        EFI_GET_TIME GetTime;
        EFI_SET_TIME SetTime;
        EFI_GET_WAKEUP_TIME GetWakeupTime;
        EFI_SET_WAKEUP_TIME SetWakeupTime;
        //
        // Virtual memory services
        //

        EFI_SET_VIRTUAL_ADDRESS_MAP SetVirtualAddressMap;
        EFI_CONVERT_POINTER ConvertPointer;
        //
        // Variable serviers
        //

        EFI_GET_VARIABLE GetVariable;
        EFI_GET_NEXT_VARIABLE_NAME GetNextVariableName;
        EFI_SET_VARIABLE SetVariable;
        //
        // Misc
        //

        EFI_GET_NEXT_HIGH_MONO_COUNT GetNextHighMonotonicCount;
        EFI_RESET_SYSTEM ResetSystem;
        EFI_UPDATE_CAPSULE UpdateCapsule;
        EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;
        EFI_QUERY_VARIABLE_INFO QueryVariableInfo;
    } EFI_RUNTIME_SERVICES;
    //
    // EFI Boot Services Table
    //
    typedef EFI_STATUS(EFIAPI *EFI_IMAGE_UNLOAD)(EFI_HANDLE ImageHandle);
    typedef struct _EFI_BOOT_SERVICES
    {

        EFI_TABLE_HEADER Hdr;
        //
        // Task priority functions
        //

        EFI_RAISE_TPL RaiseTPL;
        EFI_RESTORE_TPL RestoreTPL;
        //
        // Memory functions
        //

        EFI_ALLOCATE_PAGES AllocatePages;
        EFI_FREE_PAGES FreePages;
        EFI_GET_MEMORY_MAP GetMemoryMap;
        EFI_ALLOCATE_POOL AllocatePool;
        EFI_FREE_POOL FreePool;
        //
        // Event & timer functions
        //

        EFI_CREATE_EVENT CreateEvent;
        EFI_SET_TIMER SetTimer;
        EFI_WAIT_FOR_EVENT WaitForEvent;
        EFI_SIGNAL_EVENT SignalEvent;
        EFI_CLOSE_EVENT CloseEvent;
        EFI_CHECK_EVENT CheckEvent;
        //
        // Protocol handler functions
        //

        EFI_INSTALL_PROTOCOL_INTERFACE InstallProtocolInterface;
        EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface;
        EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface;
        EFI_HANDLE_PROTOCOL HandleProtocol;
        EFI_HANDLE_PROTOCOL PCHandleProtocol;
        EFI_REGISTER_PROTOCOL_NOTIFY RegisterProtocolNotify;
        EFI_LOCATE_HANDLE LocateHandle;
        EFI_LOCATE_DEVICE_PATH LocateDevicePath;
        EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable;
        //
        // Image functions
        //

        EFI_IMAGE_LOAD LoadImage;
        EFI_IMAGE_START StartImage;
        EFI_EXIT Exit;
        EFI_IMAGE_UNLOAD UnloadImage;
        EFI_EXIT_BOOT_SERVICES ExitBootServices;
        //
        // Misc functions
        //

        EFI_GET_NEXT_MONOTONIC_COUNT GetNextMonotonicCount;
        EFI_STALL Stall;
        EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;
        //
        // DriverSupport Services
        //

        EFI_CONNECT_CONTROLLER ConnectController;
        EFI_DISCONNECT_CONTROLLER DisconnectController;
        //
        // Open and Close Protocol Services
        //
        EFI_OPEN_PROTOCOL OpenProtocol;
        EFI_CLOSE_PROTOCOL CloseProtocol;
        EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation;
        //
        // Library Services
        //
        EFI_PROTOCOLS_PER_HANDLE ProtocolsPerHandle;
        EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer;
        EFI_LOCATE_PROTOCOL LocateProtocol;
        EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES InstallMultipleProtocolInterfaces;
        EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces;
        //
        // 32-bit CRC Services
        //
        EFI_CALCULATE_CRC32 CalculateCrc32;
        //
        // Misc Services
        //
        EFI_COPY_MEM CopyMem;
        EFI_SET_MEM SetMem;
        EFI_CREATE_EVENT_EX CreateEventEx;
    } EFI_BOOT_SERVICES;

    //
    // Block IO protocol
    //

#define EFI_BLOCK_IO_PROTOCOL_GUID                                                                                                                             \
    {                                                                                                                                                          \
        0x964e5b21, 0x6459, 0x11d2,                                                                                                                            \
        {                                                                                                                                                      \
            0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                                                                                                      \
        }                                                                                                                                                      \
    }
#define BLOCK_IO_PROTOCOL EFI_BLOCK_IO_PROTOCOL_GUID

#define EFI_BLOCK_IO_PROTOCOL_REVISION 0x00010000
#define EFI_BLOCK_IO_PROTOCOL_REVISION2 0x00020001
#define EFI_BLOCK_IO_PROTOCOL_REVISION3 ((2 << 16) | 31)
#define EFI_BLOCK_IO_INTERFACE_REVISION EFI_BLOCK_IO_PROTOCOL_REVISION
#define EFI_BLOCK_IO_INTERFACE_REVISION2 EFI_BLOCK_IO_PROTOCOL_REVISION2
#define EFI_BLOCK_IO_INTERFACE_REVISION3 EFI_BLOCK_IO_PROTOCOL_REVISION3

    struct _EFI_BLOCK_IO_PROTOCOL;

    typedef EFI_STATUS(EFIAPI *EFI_BLOCK_RESET)(struct _EFI_BLOCK_IO_PROTOCOL *This, BOOLEAN ExtendedVerification);

    typedef EFI_STATUS(EFIAPI *EFI_BLOCK_READ)(struct _EFI_BLOCK_IO_PROTOCOL *This, UINT32 MediaId, EFI_LBA LBA, UINTN BufferSize, VOID *Buffer);

    typedef EFI_STATUS(EFIAPI *EFI_BLOCK_WRITE)(struct _EFI_BLOCK_IO_PROTOCOL *This, UINT32 MediaId, EFI_LBA LBA, UINTN BufferSize, VOID *Buffer);

    typedef EFI_STATUS(EFIAPI *EFI_BLOCK_FLUSH)(struct _EFI_BLOCK_IO_PROTOCOL *This);

    typedef struct
    {
        UINT32 MediaId;
        BOOLEAN RemovableMedia;
        BOOLEAN MediaPresent;

        BOOLEAN LogicalPartition;
        BOOLEAN ReadOnly;
        BOOLEAN WriteCaching;

        UINT32 BlockSize;
        UINT32 IoAlign;

        EFI_LBA LastBlock;

        /* revision 2 */
        EFI_LBA LowestAlignedLba;
        UINT32 LogicalBlocksPerPhysicalBlock;
        /* revision 3 */
        UINT32 OptimalTransferLengthGranularity;
    } EFI_BLOCK_IO_MEDIA;

    typedef struct _EFI_BLOCK_IO_PROTOCOL
    {
        UINT64 Revision;

        EFI_BLOCK_IO_MEDIA *Media;

        EFI_BLOCK_RESET Reset;
        EFI_BLOCK_READ ReadBlocks;
        EFI_BLOCK_WRITE WriteBlocks;
        EFI_BLOCK_FLUSH FlushBlocks;

    } EFI_BLOCK_IO_PROTOCOL;

    typedef struct _EFI_BLOCK_IO_PROTOCOL _EFI_BLOCK_IO;
    typedef EFI_BLOCK_IO_PROTOCOL EFI_BLOCK_IO;

#define EFI_BLOCK_IO2_PROTOCOL_GUID                                                                                                                            \
    {                                                                                                                                                          \
        0xa77b2472, 0xe282, 0x4e9f,                                                                                                                            \
        {                                                                                                                                                      \
            0xa2, 0x45, 0xc2, 0xc0, 0xe2, 0x7b, 0xbc, 0xc1                                                                                                     \
        }                                                                                                                                                      \
    }

    struct _EFI_BLOCK_IO2_PROTOCOL;

    typedef struct
    {
        EFI_EVENT Event;
        EFI_STATUS TransactionStatus;
    } EFI_BLOCK_IO2_TOKEN;

    typedef EFI_STATUS(EFIAPI *EFI_BLOCK_RESET_EX)(struct _EFI_BLOCK_IO2_PROTOCOL *This, BOOLEAN ExtendedVerification);

    typedef EFI_STATUS(EFIAPI *EFI_BLOCK_READ_EX)(struct _EFI_BLOCK_IO2_PROTOCOL *This, UINT32 MediaId, EFI_LBA LBA, EFI_BLOCK_IO2_TOKEN *Token,
                                                  UINTN BufferSize, VOID *Buffer);

    typedef EFI_STATUS(EFIAPI *EFI_BLOCK_WRITE_EX)(struct _EFI_BLOCK_IO2_PROTOCOL *This, UINT32 MediaId, EFI_LBA LBA, EFI_BLOCK_IO2_TOKEN *Token,
                                                   UINTN BufferSize, VOID *Buffer);

    typedef EFI_STATUS(EFIAPI *EFI_BLOCK_FLUSH_EX)(struct _EFI_BLOCK_IO2_PROTOCOL *This, EFI_BLOCK_IO2_TOKEN *Token);

    typedef struct _EFI_BLOCK_IO2_PROTOCOL
    {
        EFI_BLOCK_IO_MEDIA *Media;
        EFI_BLOCK_RESET_EX Reset;
        EFI_BLOCK_READ_EX ReadBlocksEx;
        EFI_BLOCK_WRITE_EX WriteBlocksEx;
        EFI_BLOCK_FLUSH_EX FlushBlocksEx;
    } EFI_BLOCK_IO2_PROTOCOL;

    typedef struct _EFI_CONFIGURATION_TABLE
    {
        EFI_GUID VendorGuid;
        VOID *VendorTable;
    } EFI_CONFIGURATION_TABLE;
    //
    // EFI System Table
    //

    typedef struct _EFI_SYSTEM_TABLE
    {
        EFI_TABLE_HEADER Hdr;
        CHAR16 *FirmwareVendor;
        UINT32 FirmwareRevision;
        EFI_HANDLE ConsoleInHandle;
        SIMPLE_INPUT_INTERFACE *ConIn;
        EFI_HANDLE ConsoleOutHandle;
        SIMPLE_TEXT_OUTPUT_INTERFACE *ConOut;
        EFI_HANDLE StandardErrorHandle;
        SIMPLE_TEXT_OUTPUT_INTERFACE *StdErr;
        EFI_RUNTIME_SERVICES *RuntimeServices;
        EFI_BOOT_SERVICES *BootServices;
        UINTN NumberOfTableEntries;
        EFI_CONFIGURATION_TABLE *ConfigurationTable;
    } EFI_SYSTEM_TABLE;

/* Graphics output protocol */
#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID                                                                                                                      \
    {                                                                                                                                                          \
        0x9042a9de, 0x23dc, 0x4a38,                                                                                                                            \
        {                                                                                                                                                      \
            0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a                                                                                                     \
        }                                                                                                                                                      \
    }
    typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL EFI_GRAPHICS_OUTPUT_PROTOCOL;

    typedef struct
    {
        UINT32 RedMask;
        UINT32 GreenMask;
        UINT32 BlueMask;
        UINT32 ReservedMask;
    } EFI_PIXEL_BITMASK;

    typedef enum
    {
        PixelRedGreenBlueReserved8BitPerColor,
        PixelBlueGreenRedReserved8BitPerColor,
        PixelBitMask,
        PixelBltOnly,
        PixelFormatMax
    } EFI_GRAPHICS_PIXEL_FORMAT;

    typedef struct
    {
        UINT32 Version;
        UINT32 HorizontalResolution;
        UINT32 VerticalResolution;
        EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
        EFI_PIXEL_BITMASK PixelInformation;
        UINT32 PixelsPerScanLine;
    } EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

    /**
      Return the current video mode information.

      @param  This       Protocol instance pointer.
      @param  ModeNumber The mode number to return information on.
      @param  SizeOfInfo A pointer to the size, in bytes, of the Info buffer.
      @param  Info       A pointer to callee allocated buffer that returns information about ModeNumber.

      @retval EFI_SUCCESS           Mode information returned.
      @retval EFI_BUFFER_TOO_SMALL  The Info buffer was too small.
      @retval EFI_DEVICE_ERROR      A hardware error occurred trying to retrieve the video mode.
      @retval EFI_NOT_STARTED       Video display is not initialized. Call SetMode ()
      @retval EFI_INVALID_PARAMETER One of the input args was NULL.

    **/
    typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This, UINT32 ModeNumber, UINTN *SizeOfInfo,
                                                                        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);

    /**
      Return the current video mode information.

      @param  This              Protocol instance pointer.
      @param  ModeNumber        The mode number to be set.

      @retval EFI_SUCCESS       Graphics mode was changed.
      @retval EFI_DEVICE_ERROR  The device had an error and could not complete the request.
      @retval EFI_UNSUPPORTED   ModeNumber is not supported by this device.

    **/
    typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This, UINT32 ModeNumber);

    typedef struct
    {
        UINT8 Blue;
        UINT8 Green;
        UINT8 Red;
        UINT8 Reserved;
    } EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

    typedef union {
        EFI_GRAPHICS_OUTPUT_BLT_PIXEL Pixel;
        UINT32 Raw;
    } EFI_GRAPHICS_OUTPUT_BLT_PIXEL_UNION;

    typedef enum
    {
        EfiBltVideoFill,
        EfiBltVideoToBltBuffer,
        EfiBltBufferToVideo,
        EfiBltVideoToVideo,
        EfiGraphicsOutputBltOperationMax
    } EFI_GRAPHICS_OUTPUT_BLT_OPERATION;

    /**
      The following table defines actions for BltOperations:

      <B>EfiBltVideoFill</B> - Write data from the  BltBuffer pixel (SourceX, SourceY)
      directly to every pixel of the video display rectangle
      (DestinationX, DestinationY) (DestinationX + Width, DestinationY + Height).
      Only one pixel will be used from the BltBuffer. Delta is NOT used.

      <B>EfiBltVideoToBltBuffer</B> - Read data from the video display rectangle
      (SourceX, SourceY) (SourceX + Width, SourceY + Height) and place it in
      the BltBuffer rectangle (DestinationX, DestinationY )
      (DestinationX + Width, DestinationY + Height). If DestinationX or
      DestinationY is not zero then Delta must be set to the length in bytes
      of a row in the BltBuffer.

      <B>EfiBltBufferToVideo</B> - Write data from the  BltBuffer rectangle
      (SourceX, SourceY) (SourceX + Width, SourceY + Height) directly to the
      video display rectangle (DestinationX, DestinationY)
      (DestinationX + Width, DestinationY + Height). If SourceX or SourceY is
      not zero then Delta must be set to the length in bytes of a row in the
      BltBuffer.

      <B>EfiBltVideoToVideo</B> - Copy from the video display rectangle (SourceX, SourceY)
      (SourceX + Width, SourceY + Height) .to the video display rectangle
      (DestinationX, DestinationY) (DestinationX + Width, DestinationY + Height).
      The BltBuffer and Delta  are not used in this mode.

      @param  This         Protocol instance pointer.
      @param  BltBuffer    Buffer containing data to blit into video buffer. This
                           buffer has a size of Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
      @param  BltOperation Operation to perform on BlitBuffer and video memory
      @param  SourceX      X coordinate of source for the BltBuffer.
      @param  SourceY      Y coordinate of source for the BltBuffer.
      @param  DestinationX X coordinate of destination for the BltBuffer.
      @param  DestinationY Y coordinate of destination for the BltBuffer.
      @param  Width        Width of rectangle in BltBuffer in pixels.
      @param  Height       Hight of rectangle in BltBuffer in pixels.
      @param  Delta        OPTIONAL

      @retval EFI_SUCCESS           The Blt operation completed.
      @retval EFI_INVALID_PARAMETER BltOperation is not valid.
      @retval EFI_DEVICE_ERROR      A hardware error occured writting to the video buffer.

    **/
    typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer,
                                                                 EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation, UINTN SourceX, UINTN SourceY,
                                                                 UINTN DestinationX, UINTN DestinationY, UINTN Width, UINTN Height, UINTN Delta);

    typedef struct
    {
        UINT32 MaxMode;
        UINT32 Mode;
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
        UINTN SizeOfInfo;
        EFI_PHYSICAL_ADDRESS FrameBufferBase;
        UINTN FrameBufferSize;
    } EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

    struct _EFI_GRAPHICS_OUTPUT_PROTOCOL
    {
        EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode;
        EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE SetMode;
        EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT Blt;
        EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
    };

#ifdef __cplusplus
}
#endif
#endif //  EFI_H
