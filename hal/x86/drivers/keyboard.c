/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Keyboard driver
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include "../hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <jslk.h>

static bool shift;
static bool ctrl;
static char scancode;

extern void mapXY(uint8_t direction);

/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */
unsigned char kbdus[128] =
    {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
        '9', '0', '-', '=', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'q', 'w', 'e', 'r',                               /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`', 0,                                     /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
        'm', ',', '.', '/', 0,                            /* Right shift */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow 75 */
        0,
        0, /* Right Arrow 77 */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

unsigned char capsus[128] =
    {
        0, 27, '!', '"', '#', '$', '%', '&', '/', '(',    /* 9 */
        ')', '=', '?', '@', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'Q', 'W', 'E', 'R',                               /* 19 */
        'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
        '\'', '`', 0,                                     /* Left shift 42*/
        '\\', 'Z', 'X', 'C', 'V', 'B', 'N',               /* 49 */
        'M', ',', '.', '/', 0,                            /* Right shift 54 */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

/* Handles the keyboard interrupt */
void keyboard_handler(regs_t regs)
{
    /* Read from the keyboard's data buffer */
    scancode = inb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
        //if (scancode - 0x80 == 42 || scancode - 80 == 54) {
            shift = false;
            return;
       // }
    }
    else
    {
        if (scancode == 42 || scancode == 54 || scancode == 58) {
            shift = true;
            return;
        }
        if (scancode == 80) {
            mapXY(1);
            return;
        }
        if (scancode == 72) {
            mapXY(2);
            return;
        }
        if (scancode == 77) {
            mapXY(3);
            return;
        }
        if (scancode == 75) {
            mapXY(4);
            return;
        }
        //kernelPrintDec(scancode);
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
        /*
        if (scancode == 28) {
            jsh(inbuf);
            inbuf[0] = '\0';
        }*/
        if (shift == true) {
            kputc(capsus[scancode]);
            append(inbuf, capsus[scancode]);
        } else {
            kputc(kbdus[scancode]);
            append(inbuf, kbdus[scancode]);
        }
        if (scancode == 14 || scancode == 75 /* left arrow */)
        {
            // Backspace was pressed delete a key
            backspace();
        } else if (scancode == 77) {
            rightArrow();
        }
    }
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install() {
    registerInterruptHandler(IRQ1, keyboard_handler, NF);
}

int read_key() {
    if (scancode == 28) {
        return 0;
    } else {
        return 1;
    }
}
