#include"cv_stdlib.h"

static unsigned int mallocLength=0,SECTORNUM=0,*mallocList,*head,countPublic=0;

void free_all()
{
    void *pointer,*pointer2;
    int count;
    for(;mallocLength;)
    {
        for(count=0;count<SECTOR_SIZE-1 && mallocLength;count++)
        {
            pointer=head[count];
            free(pointer);
            mallocLength--;
        }
        pointer2=head;
        if(mallocLength)
            head=head[SECTOR_SIZE-1];
        free(pointer2);
    }
    return;
}

void mallocList_append(unsigned int address)
{
    unsigned int *pointer=0;
    if(!SECTORNUM)
    {
        pointer=(unsigned int *)malloc(SECTOR_SIZE*4);
        head=pointer;
        mallocList=head;
        SECTORNUM=1;
    }
    if(countPublic<SECTOR_SIZE-1)
    {
        mallocList[countPublic]=address;
        countPublic++;
        mallocLength++;
    }
    else
    {
        pointer=(unsigned int *)malloc(SECTOR_SIZE*4);
        mallocList[SECTOR_SIZE-1]=pointer;
        mallocList=pointer;
        countPublic=0;
        SECTORNUM++;
        mallocList[countPublic]=address;
        countPublic++;
        mallocLength++;
    }
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



