#include"cv_stdlib.h"

static unsigned int mallocList[MAX_MALLOC_SIZE],mallocLength=0;

void free_all()
{
    void *pointer;
    printf(" %d ",mallocLength);
    for(int count=0;count<mallocLength;count++)
    {
        if(mallocList[count])
        {
            pointer=mallocList[count];
            free(pointer);
        }
    }
    return;
}

void mallocList_append(unsigned int address)
{
    if(mallocLength<MAX_MALLOC_SIZE)
        mallocList[mallocLength]=address;
    else
    {
        free_all();
        printf("malloc_error : out of range at :mallocList_append(u16 address)");
        exit(0);
    }
    mallocLength++;
}

void *my_malloc(unsigned int uSize)
{
    void *pointer=0;
    pointer=malloc(uSize);
    mallocList_append(pointer);
    return pointer;
}

void my_free(void *pPtr)
{
    free(pPtr);
    pPtr=(unsigned char)malloc(1);
    return;
}



