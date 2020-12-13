#include <arch/x86_64/descriptor_tables.h>
#include <stdint.h>
#include <uuid.h>

#pragma pack(push, 1)
typedef struct system_info
{
    uint32_t no_of_cpus;
    // struct gdt_t *gdt_ptr;
    void *memory_map;
    uint32_t memory_map_len;
    uint32_t kernel_end;
    struct local_apic_structure *local_apic;
    idt_t idt;
    struct acpi_rsdp *rsdp;
    uuid_t root_drive_uuid;
} system_info_t;
#pragma pack(pop)

extern system_info_t sys_info;