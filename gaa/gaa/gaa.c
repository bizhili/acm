#include "stdio.h"
#include "stdlib.h"

int main(int argc, char *argv[])
{
    FILE *fptr,*fout;
    char buff[256]={'\0'},tempN[10]={'\0'};
    int count=0,numFlag=0,point=1,tempI,sum=0;
    if ((fptr = fopen(argv[1],"r")) == NULL)
    {
        printf("Error! opening file");
        exit(0);
    }
    fout = fopen("haha.txt","w");
    fscanf(fptr, "%s", buff);
    for(int i=0;i<256 && buff[i]!='\0';i++)
    {
        if(buff[i] !='+' && (buff[i] >'9' || buff[i] <'0'))
        {
            printf("Error: type error");
        }
        if(buff[i] !='+' && !numFlag)
        {
            tempN[0]=buff[i];
            numFlag=1;
            point=1;
        }
        else if(buff[i] !='+' && numFlag)
        {
            tempN[point]=buff[i];
            ++point;
            if(point==11)
            {
                printf("out of int range\n");
                exit(0);
            }
        }
        else if(buff[i] =='+')
        {
            ++count;
            sscanf(tempN,"%d",&tempI);
            sum+=tempI;
            fprintf(fout,"push %d\n",tempI);
            memset(tempN,'\0',10);
            numFlag=0;
        }
    }
    sscanf(tempN,"%d",&tempI);
    fprintf(fout,"push %d\n",tempI);
    sum+=tempI;
    for(int i=0;i<count;i++)
        fprintf(fout,"sum\n");
    printf("%d",sum);
    fprintf(fout,"print");
    fclose(fptr);
    fclose(fout);
    return 0;
}





