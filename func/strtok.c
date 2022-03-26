#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strtok
int convert(char num[]);
int main() {
    char str[] = "enter 123456789 300 425";//如果要輸入,用fget(因為scanf讀到空格就停了)
    const char* d = "  ";
    char *p;
    int order[15],i=0;//order 最多有3*10^6個指令->malloc
    
    p = strtok(str, d);
    printf("%c\n", p[0]);//get 指令種類
    p = strtok(NULL, d);
    
    while (p != NULL) {
        //printf("%s\n", p);
        order[i++]=convert(p);//存數字,其實可以用內建的atoi函數就好
        p = strtok(NULL, d);		   
    }
    for(int k=0;k<3;k++)printf("%d\n",order[k] );
    return 0;
}
int convert(char num[])//int夠用了
{
    int len = 0;
    while(num[len]!= '\0')
    {
        len++;
    }
    
    int ans=0;
    for(int i=0;i<len;i++)
    {
        ans = ans *10 + ((int)num[i]-48);//轉成ASCII CODE要扣掉48
    }
    return ans;
    
}
