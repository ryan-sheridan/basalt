#include <kernel/pmm.h>

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

  // 0 out everything first
  for(uint32_t i = 0; i < total_frames; i++)
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

void *pmm_alloc_frame(void) {

}

void pmm_free_frame(void *address) {

}
