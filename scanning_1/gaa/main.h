#ifndef _MAIN_H
#define _MAIN_H
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define key_num 25
#define special_num 27
void push(char a);
void empty();
int check_key();
void init_hash();
int calcu_hash(char *str);
int check_special(char str);



#endif // MAIN_H
