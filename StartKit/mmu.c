#include <stdio.h>
#include <stdlib.h>
#define SIZE 100
//Page and fram and TLB specifications
#define PAGES 256
#define PAGE_MASK 255
#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255

int pg_table[PAGES];
int free_pg = PAGES;

int main(){
    FILE *file;
    file = fopen("addresses.txt", "r");
    char pg[SIZE]; //#define SIZE 100
    int total_add = 0;

    if (file == NULL)
    {
        printf("file does not exist!");
        exit(1);        
    }

    while(fgets(pg,SIZE,file) != NULL)
    {
        int log_add = atoi(pg);
        int offset = log_add & OFFSET_MASK;
        int log_pg = (log_add >> OFFSET_BITS) & PAGE_MASK;
        printf("PAGE# : %d      OFFSET : %d \n", log_pg, offset);
        total_add = total_add + 1;
    }
    printf("TOTAL NUMBER OF PAGES : %d \n", total_add);
    return 0;
}