#include <stdint.h>
#include <string.h>


void scroll(void);


static uint16_t* videomemory = (void*) 0xb8000;
static uint16_t color = 0x0F00;
void putcharacter(const char ch, uint32_t x, uint32_t y);

void putcharacter(const char ch, uint32_t x, uint32_t y)
{
    videomemory[((y * 80) + x)] = (uint16_t) (color | ch);
}

void scroll()
{
    memcpy((char*) videomemory, (char*) (videomemory + 80), 80 * 24 * 2);
}
