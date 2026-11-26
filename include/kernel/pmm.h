#ifndef PMM_H

#include <stdint.h>

extern uint8_t __end[];


void pmm_init(void);
void *pmm_alloc_frame(void);
void pmm_free_frame(void *addr);
void pmm_print_hex(void *addr); // debug

#endif // !PMM_H
