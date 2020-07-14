#ifndef CV_STDLIB_H
#define CV_STDLIB_H
#include "stdlib.h"
#include "stdio.h"

#define SECTOR_SIZE 64

extern void my_free(void *pPtr);
extern void *my_malloc(unsigned int uSize);
extern void free_all();

#endif // MAIN_H

