#include "tty.h"
#include "input.h"
#include "fs/fs.h"

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

void kernel_main() {
    tty_init();
    init_keyboard();
}
