#include "tty.h"
#include <stddef.h>
#include <stdint.h>

#define VIDEO_MEMORY (uint8_t*)0xB8000
#define WIDTH 80
#define HEIGHT 25
#define COLOR 0x0E

static int cursor = 0;

void tty_clear() {
    for (int i = 0; i < WIDTH * HEIGHT * 2; i += 2) {
        VIDEO_MEMORY[i] = ' ';
        VIDEO_MEMORY[i + 1] = COLOR;
    }
    cursor = 0;
}

void tty_putc(char c) {
    if (c == '\n') {
        cursor += (WIDTH - cursor % WIDTH);
    } else {
        VIDEO_MEMORY[cursor * 2] = c;
        VIDEO_MEMORY[cursor * 2 + 1] = COLOR;
        cursor++;
    }

    if (cursor >= WIDTH * HEIGHT) {
        tty_clear();
    }
}

void tty_print(const char* str) {
    while (*str) tty_putc(*str++);
}

void tty_newline() {
    tty_putc('\n');
}

void tty_init() {
    tty_clear();
    tty_print("╔══════════════════════════════════╗\n");
    tty_print("║  Welcome to CharlotOS-05-Yellow ║\n");
    tty_print("╚══════════════════════════════════╝\n\n");
    tty_print("> ");
}
