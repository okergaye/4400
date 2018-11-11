#include "mm.h"

#define ALIGNMENT 16
//Alignes to the next 16 byte alinment
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
// returns the size of the block_header ()
#define OVERHEAD sizeof(block_header)
//Macro for getting the header from a payload pointer:
/// bp i think is block payload, and is a pointer
#define HDRP(bp) ((char *)(bp) - sizeof(block_header))

//these are used on block header pointers to see the size of 
//the block and weather is it allocated
#define GET_SIZE(p)  ((block_header *)(p))->size
#define GET_ALLOC(p) ((block_header *)(p))->allocated

//Macro for getting the next block’s payload:

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))

typedef struct {
  size_t size;
  char   allocated;
} block_header;

void *first_bp;

void mm_init(void *heap, size_t heap_size)
{
  void *bp;
  
  bp = heap + sizeof(block_header);
  
  GET_SIZE(HDRP(bp)) = heap_size;
  GET_ALLOC(HDRP(bp)) = 0;

  first_bp = bp;
}

static void set_allocated(void *bp)
{
  GET_ALLOC(HDRP(bp)) = 1;
}

void *mm_malloc(size_t size)
{
  void *bp = first_bp;

  if (GET_ALLOC(HDRP(bp))) {
    /* Our only block is already allocated tes2*/
    return NULL;
  } else {
    set_allocated(bp);
    return bp;
  }
}

void mm_free(void *bp)
{
  GET_ALLOC(HDRP(bp)) = 0;
}
