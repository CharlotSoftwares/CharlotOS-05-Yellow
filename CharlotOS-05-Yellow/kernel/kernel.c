// kernel.c — CharlotOS Yellow core kernel
void print_char(char c, int color) {
    volatile char* video = (volatile char*)0xB8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video[i] = ' ';        // clear screen
        video[i + 1] = 0x0E;   // yellow on black
    }

    video[0] = c;
    video[1] = color;
}

void print_str(const char* str, int color) {
    volatile char* video = (volatile char*)0xB8000;
    int i = 0;
    while (*str) {
        video[i++] = *str++;
        video[i++] = color;
    }
}
// shit that took 19203493409520193 hours
void kernel_main() {
    print_str("CharlotOS-05-Yellow boots up like a damn beast.\n", 0x0E);
}
