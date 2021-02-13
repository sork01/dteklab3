#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw()
{
    return ((*(volatile int*)PORTD & 0xf00) >> 8);
}

int getbtns()
{
    return ((*(volatile int*)PORTD & 0xe0) >> 4) | ((*(volatile int*)PORTF & 0x2) >> 1);
}
