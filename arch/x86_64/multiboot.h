#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#pragma pack(push, 8)
struct kernel_multiboot_header
{
    alignas(8) multiboot_header mboot_header;
    alignas(8) multiboot_header_tag_information_request tag_info_req;
    multiboot_uint32_t requests[5];
    //alignas(8) multiboot_header_tag_framebuffer tag_framebuffer;
    alignas(8) multiboot_header_tag_module_align tag_mod_align;
    alignas(8) multiboot_header_tag tag_end;
};
#pragma pack(pop)

extern kernel_multiboot_header momentum_multiboot_header;

#endif