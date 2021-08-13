#include <stdint.h>
#include <stddef.h>
#include <arch/video.h>
#include "efi.h"
#include <kernel/framebuffer.h>

extern EFI_SYSTEM_TABLE *st;
SIMPLE_TEXT_OUTPUT_INTERFACE *oi = nullptr;
EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsOutput = nullptr;
extern framebuffer fb;

void print_string_efi_text(const char *ch)
{
    wchar_t wch[1024] = {};
    for (size_t i = 0; i < 1024 && *ch != 0; i++)
    {
        if (*ch == '\n' && i < 1023)
            wch[i++] = '\r';
        wch[i] = *ch++;
    }
    oi->OutputString(oi, wch);
}
void scroll_efi_text(void)
{
}
void setColor_efi_text(uint32_t c)
{
}

int init_framebuffer_rendering();

int init_video(SIMPLE_TEXT_OUTPUT_INTERFACE *output_interface)
{
    EFI_STATUS status = 0;
    EFI_HANDLE handles[128] = {};
    UINTN handle_count = sizeof(handles);
    EFI_GUID guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    oi = output_interface;
    set_io_functions(print_string_efi_text, scroll_efi_text, setColor_efi_text);

    status = st->BootServices->LocateHandle(ByProtocol, &guid, nullptr, &handle_count, handles);
    if (status)
    {
        return status;
    }
    handle_count /= sizeof(EFI_HANDLE);
    status = st->BootServices->HandleProtocol(handles[0], &guid, (VOID **)&graphicsOutput);
    if (status)
    {
        return status;
    }

    status = init_framebuffer_rendering();
    if (status)
    {
        return status;
    }

    return status;
}

//  this include needs to be here
#include <stdio.h>
int init_framebuffer_rendering()
{
    EFI_STATUS status = 0;
    UINT32 bestModeId = graphicsOutput->Mode->Mode;
    size_t modeScore = graphicsOutput->Mode->Info->VerticalResolution * graphicsOutput->Mode->Info->HorizontalResolution;

    printf("Supported Graphics modes:\n");
    for (UINT32 modeId = 0; modeId < graphicsOutput->Mode->MaxMode; modeId++)
    {
        UINTN sizeOfInfo = 0;
        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = nullptr;

        status = graphicsOutput->QueryMode(graphicsOutput, modeId, &sizeOfInfo, &info);
        if (status)
        {
            printf("Failed to query mode info. error %d\n", status);
            return status;
        }
        printf("Mode:%d,Resolution:%dx%d,PixelFormat:%x,PixelsPerScanLine:%d\n", modeId, info->HorizontalResolution, info->VerticalResolution,
               info->PixelFormat, info->PixelsPerScanLine);

        if (info->VerticalResolution > 720)
            break;

        auto newModeScore = info->VerticalResolution * info->HorizontalResolution;
        if (newModeScore > modeScore)
        {
            modeScore = newModeScore;
            bestModeId = modeId;
        }
    }
    printf("Switching to mode %d\n", bestModeId);
    status = graphicsOutput->SetMode(graphicsOutput, bestModeId);
    if (status)
    {
        printf("Failed to switch mode!\n");
        return status;
    }
    printf("New mode selected\n");
    printf("Mode:%d,Resolution:%dx%d,PixelFormat:%x,PixelsPerScanLine:%d\n", graphicsOutput->Mode->Mode, graphicsOutput->Mode->Info->HorizontalResolution,
           graphicsOutput->Mode->Info->VerticalResolution, graphicsOutput->Mode->Info->PixelFormat, graphicsOutput->Mode->Info->PixelsPerScanLine);
    fb.framebuffer_base = (uint8_t *)graphicsOutput->Mode->FrameBufferBase;
    fb.framebuffer_size = graphicsOutput->Mode->FrameBufferSize;
    fb.horizontal_sz = graphicsOutput->Mode->Info->HorizontalResolution;
    fb.vertical_sz = graphicsOutput->Mode->Info->VerticalResolution;
    fb.pitch = graphicsOutput->Mode->Info->PixelsPerScanLine * 4;
    fb.depth = 4;

    console_height = graphicsOutput->Mode->Info->VerticalResolution / 16;
    console_width = graphicsOutput->Mode->Info->HorizontalResolution / 8;

    set_io_functions(print_string_fb, scroll_fb, setColor_fb);
    printf("Switched to software frame buffer rendering!\n");
    return status;
}
