// kernel/kernel.c

#include "tty.h"
#include "input.h"
#include "fs/fs.h"
#include <string.h>

#define INPUT_BUFFER_SIZE 128

// Simple shell command handler
void shell_handle_input(char* input) {
    if (strcmp(input, "charlot version") == 0) {
        tty_print("CharlotOS-05-Yellow v0.1 - loud, dumb, and dangerous\n");
    } else if (strcmp(input, "clear") == 0) {
        tty_clear();
    } else {
        tty_print("Unknown command: ");
        tty_print(input);
        tty_newline();
    }
}

// Main kernel entry point
void kernel_main() {
    tty_init();
    init_keyboard();

    tty_clear();
    tty_print("Welcome to CharlotOS-05-Yellow shell!\n");
    tty_print("> ");

    char input_buffer[INPUT_BUFFER_SIZE];
    int input_pos = 0;

    while (1) {
        // Wait for input character (blocking or polling)
        char c = input_get_char();

        if (c == '\b' || c == 127) {  // Backspace handling
            if (input_pos > 0) {
                input_pos--;
                tty_backspace();
            }
        } else if (c == '\r' || c == '\n') {  // Enter key
            input_buffer[input_pos] = '\0';  // Null terminate input
            tty_newline();

            shell_handle_input(input_buffer);

            // Reset input buffer and prompt
            input_pos = 0;
            tty_print("> ");
        } else if (c >= 32 && c <= 126 && input_pos < INPUT_BUFFER_SIZE - 1) { 
            // Printable characters
            input_buffer[input_pos++] = c;
            tty_putchar(c);
        }
    }
}
