//64 bits complier
#include"cv_stdlib.h"

static unsigned int mallocLength=0,SECTORNUM=0,countPublic=0;
static unsigned long long *mallocList,*head;
void free_all()
{
    void *pointer=0,*pointer2=0;
    int count;
    
    for(;mallocLength;)
    {
        for(count=0;count<SECTOR_SIZE-1 && mallocLength;count++)
        {
            pointer=head[count];
            free(pointer);
            pointer=0;
            mallocLength--;
        }
        pointer2=head;
        if(mallocLength)
            head=head[SECTOR_SIZE-1];
        free(pointer2);
        pointer2=0;
    }
    return;
}

void mallocList_append(unsigned long long address)
{
    unsigned int *pointer=0;
    if(!SECTORNUM)
    {
        pointer=(unsigned int *)malloc(SECTOR_SIZE*8);
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
        pointer=(unsigned int *)malloc(sizeof(void *));
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



