#include <stdint.h>
#include <pic32mx.h>

int getsw(void)
{
    return (PORTD & 0xf00) >> 8;
    // ... ... ... ... 0000 1111 0000 0000
}

int getbtns(void)
{
    return (PORTD & 0xe0) >> 5;
    // . . . . . 0000 1110 0000
}