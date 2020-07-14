#include "main.h"
char tempN[256]={'\0'};
int PO=0;
char key[24][10]={"if","else","for","break","case","continue","extern","goto","return","switch","sizeof",\
"struct","static","typedef","enum","union","while","int","float","short","long","double","void","unsigned"};
unsigned char hash_table[2400]={'\0'};
int main(int argc, char *argv[])
{
    FILE *fptr,*fout;
    char *buff;
    int statu=0,row=0,column=0,row_now=0,column_now=0;
    long fsize;
    init_hash();
    if ((fptr = fopen(argv[1],"r")) == NULL)
    {
        printf("Error! opening file");
        exit(0);
    }
    fout = fopen("haha.txt","w");
    fseek(fptr, 0, SEEK_END);
    fsize = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);  /* same as rewind(f); */
    buff= malloc(fsize + 1);
    fread(buff, 1, fsize, fptr);
    for(int point=0;point<fsize + 1 && buff[point]!='\0';)
    {
        if(statu==0)
        {
            if(buff[point]==' ')
            {
                ++point;
                ++column;
            }
            else if(buff[point]=='\n')
            {
                ++point;
                ++row;
                column=0;
            }
            else
            {
                statu=1;
                row_now=row,column_now=column;
            }
        }
        else if(statu==1)
        {
            if(buff[point]==' ')
            {
                ++point;
                ++column;
                statu=0;
                if(!check())
                    fprintf(fout,"%s(ID) %d %d\n",tempN,row_now,column_now);
                else
                    fprintf(fout,"%s %d %d\n",tempN,row_now,column_now);
                empty();
            }
            else if(buff[point]=='\n')
            {
                ++point;
                ++row;
                column=0;
                statu=0;
                if(!check())
                    fprintf(fout,"%s(ID) %d %d\n",tempN,row_now,column_now);
                else
                    fprintf(fout,"%s %d %d\n",tempN,row_now,column_now);
                empty();
            }
            else
            {
                push(buff[point]);
                ++column;
                ++point;
            }
        }
    }
    fprintf(fout,"bizhi");
    fclose(fptr);
    fclose(fout);
    return 0;
}
void push(char a)
{
    tempN[PO]=a;
    ++PO;
}
void empty()
{
    memset(tempN,'\0',256);
    PO=0;
}

int check()
{
    if(hash_table[calcu_hash(tempN)]==1)
        return 1;
    else
        return 0;
}
int calcu_hash(char *str)
{
    int count=0,ans;
    while(str[++count]!='\0');
    ans=count%20*200+(str[count-1]-'a')*2+(str[0]-'a')*3+(str[(int)count/2]-'a');
    return ans;
}
void init_hash()
{
    for(int i=0;i<24;i++)
    {
        hash_table[calcu_hash(key[i])]=1;
    }
}








