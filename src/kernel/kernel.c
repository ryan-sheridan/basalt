#include <kernel/uart.h>
#include <kernel/pmm.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    uart_puts("Hello, kernel World!\r\n");

    pmm_init();
    uart_puts("PMM up, testing...\r\n");

    void *a = pmm_alloc_frame();
    void *b = pmm_alloc_frame();
    void *c = pmm_alloc_frame();

    uart_puts("a = "); pmm_print_hex(a); uart_puts("\r\n");
    uart_puts("b = "); pmm_print_hex(b); uart_puts("\r\n");
    uart_puts("c = "); pmm_print_hex(c); uart_puts("\r\n");

    pmm_free_frame(b);
    uart_puts("freed b\r\n");

    void *d = pmm_alloc_frame();
    uart_puts("d = "); pmm_print_hex(d); uart_puts("\r\n");
    uart_puts("(d should equal b)\r\n");

    while (1) {
        uart_putc(uart_getc());
        uart_putc('\n');
    }
}

