#include <kernel/pmm.h>
#include <kernel/uart.h>

// hardcoded for raspi2b
#define FRAME_SIZE 4096
#define TOTAL_FRAMES 262144 // 1GB / 4KB
#define PAGE_ALIGN_UP(addr) (((addr) + FRAME_SIZE - 1) & ~(FRAME_SIZE - 1))

static uint32_t *bitmap;
static uint32_t total_frames;
static uint32_t free_frames;
static uint32_t bitmap_bytes;

static void mark_used(uint32_t f) {
  bitmap[f / 32] |= (1u << (f % 32));
  free_frames--;
}

void pmm_init(void) {
  bitmap_bytes = TOTAL_FRAMES / 8;
  bitmap = (uint32_t *)__end; // address who is supplied by the linker
  total_frames = TOTAL_FRAMES;
  free_frames = TOTAL_FRAMES;

  // 0 out everything first (total_frames BITS = total_frames/32 WORDS)
  for(uint32_t i = 0; i < total_frames / 32; i++)
    bitmap[i] = 0;

  // reserved_end = 0 - __end + bitmap_bytes
  uint32_t reserved_end = (uint32_t)__end + bitmap_bytes;
  reserved_end = PAGE_ALIGN_UP(reserved_end); // snap up a whole frame boundary

  // convert the bytes to frames 0x12000 / 4096 = 18
  uint32_t end_frame = reserved_end / FRAME_SIZE;

  // looping in frame numbers
  for(uint32_t f = 0; f < end_frame; f++)
    mark_used(f);
}

// find the first deallocated frame and return the address
void *pmm_alloc_frame(void) {
  if(free_frames == 0) return 0; // out of memory, TODO: exception here

  for(uint32_t i = 0; i < total_frames / 32; i++) {
    if(bitmap[i] == 0xFFFFFFFF) continue; // skip fast

    for(uint32_t bit = 0; bit < 32; bit++) { // for every bit
      if(!(bitmap[i] & (1 << bit))) { // find first free frame
        uint32_t frame = i * 32 + bit; // get frame number
        mark_used(frame);
        return (void *)(frame * FRAME_SIZE);
      }
    }
  }
  return 0;
}

void pmm_free_frame(void *addr) {
  uint32_t frame = (uint32_t)addr / FRAME_SIZE;
  if(frame >= total_frames) return; // check if im crazy
  bitmap[frame / 32] &= ~(1 << (frame % 32));
  free_frames++;
}

// debug: print an address as 8 hex digits, e.g. 0x00012000
// created by GLM 5.3 flash, temp for debugging
void pmm_print_hex(void *addr) {
  uint32_t value = (uint32_t)addr;

  uart_puts("0x");
  for(int i = 28; i >= 0; i -= 4) {
    uint32_t nibble = (value >> i) & 0xF;      // grab one hex digit
    char c = nibble < 10 ? '0' + nibble        // 0-9
                         : 'a' + (nibble - 10); // a-f
    uart_putc(c);
  }
}
