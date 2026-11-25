#include <stddef.h>
#include <stdint.h>

#ifndef UART_H
#define UART_H

void delay(int32_t count);
void uart_init();
void uart_putc(unsigned char c);
unsigned char uart_getc();
void uart_puts(const char* str);

#endif // !UART_H
