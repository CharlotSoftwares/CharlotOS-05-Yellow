#include "tty.h"
#include "input.h"
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60

extern void shell_handle_input(char* buffer);

static char kbdus[128] = {
  0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
  '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
  'a','s','d','f','g','h','j','k','l',';','\'','`',   0,'\\',
  'z','x','c','v','b','n','m',',','.','/',   0,'*',   0,' ', 0
};

char input_buffer[256];
int buffer_index = 0;

void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode & 0x80) return;  // key release
    char key = kbdus[scancode];
    if (!key) return;

    if (key == '\n') {
        input_buffer[buffer_index] = '\0';
        tty_newline();
        shell_handle_input(input_buffer);
        buffer_index = 0;
        tty_print("> ");
    } else {
        input_buffer[buffer_index++] = key;
        tty_putc(key);
    }
}

void init_keyboard() {
    // Stub for now, needs to register IRQ later
}
