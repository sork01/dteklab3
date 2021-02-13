/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog 

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

int timeoutcount = 0;

/* Interrupt Service Routine */
void user_isr()
{
    return;
}

/* Lab-specific initialization goes here */
void labinit()
{
    (*((volatile int*) 0xbf886100)) &= 0x00; // set RE0 ... RE7 to 0 (output, LEDs)
    (*(volatile int*)TRISD) |= (0x7f << 5); // set RD11 ... RD5 to 1 (input) (all four switches, BTN4 ... BTN2)
    (*(volatile int*)TRISF) |= 0x2; // set RF1 to 1 (input) (BTN1)

    // T2CON = 0b 1000 0000 0111 0000
    PR2 = 31250;
    TMR2 = 0;
    IFS(0) &= 0xeff; // zero index 8
    T2CON = 0x8070;

    return;
}

/* This function is called repetitively from the main program */
void labwork()
{
    volatile int* p = (volatile int*) 0xbf886110;
    int buttons = getbtns();

    if (buttons != 0) // some button is pressed
    {
        if (buttons & 0b1000) // pressed button 4
        {
            mytime = (getsw() << 12) | (mytime & 0x0fff);
        }
        
        if (buttons & 0b0100) // pressed button 3
        {
            mytime = (getsw() << 8) | (mytime & 0xf0ff);
        }
        
        if (buttons & 0b0010) // pressed button 2
        {
            mytime = (getsw() << 4) | (mytime & 0xff0f);
        }
        
        if (buttons & 0b0001) // pressed button 1
        {
            mytime = (getsw()) | (mytime & 0xfff0);
        }
        
        int seconds = (mytime & 0x000f) + 10 * ((mytime & 0x00f0) >> 4) + 60 * ((mytime & 0x0f00) >> 8) + 600 * ((mytime & 0xf000) >> 12);
        
        (*p) = (*p) & 0xffffff00 | (seconds % 256);
        
        time2string( textstring, mytime );
        display_string( 3, textstring );
        display_update();
    }
    else
    {
        while ((IFS(0) & 0x100) >> 8 == 0);
        
        IFS(0) &= 0xeff; // zero index 8
        
        timeoutcount = ((timeoutcount + 1) % 10);
        
        if (timeoutcount == 0)
        {
            tick( &mytime );
            // update the LEDs
            (*p) = ((*p) & 0xffffff00) | (((*p) & 0xff) + 1);
            
            time2string( textstring, mytime );
            display_string( 3, textstring );
            display_update();
            display_image(96, icon);
        }
    }
}
