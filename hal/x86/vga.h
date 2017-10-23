#ifndef _x86_vga_h
#define _x86_vga_h

// Provide simple interface to set vga text/background colors
enum vga_color_t
{
    vga_black = 0,
    vga_blue = 1,
    vga_green = 2,
    vga_cyan = 3,
    vga_red = 4,
    vga_magenta = 5,
    vga_brown = 6,
    vga_light_grey = 7,
    vga_grey = 8,
    vga_light_blue = 9,
    vga_light_green = 10,
    vga_light_cyan = 11,
    vga_light_red = 12,
    vga_light_magenta = 13,
    vga_light_brown = 14,
    vga_white = 15,
};

typedef enum vga_color_t vga_color;

#endif