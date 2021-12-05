#include <stdio.h>
#include <stdlib.h>
#define SIZE 100 
//Page and fram and TLB specifications
#define PAGES 256
#define PAGE_MASK 255
#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255
#define MEM_size PAGES * PAGE_SIZE

int pg_table[PAGES];
int free_pg = PAGES;
signed char main_mem[MEM_size];
signed char *backingStore_ptr;

int main(int argc, const char *argv[]){
    FILE *file;
    file = fopen("addresses.txt", "r");
    char pg[SIZE]; //#define SIZE 100
    int total_add = 0;
    int pg_fault = 0; //counter for page faults

    if (file == NULL)
    {
        printf("file does not exist!");
        exit(1);        
    }

    //initialize entries of page table to -1 for empty table
    for (int i = 0; i < PAGES; i++)
    {
        pg_table[i] = -1;
    }
    

    while(fgets(pg,SIZE,file) != NULL)
    {
        int log_add = atoi(pg);
        int offset = log_add & OFFSET_MASK;
        int log_pg = (log_add >> OFFSET_BITS) & PAGE_MASK;
        int phy_pg = pg_table[log_pg];
        if (phy_pg == -1)
        {
            pg_fault = pg_fault + 1;
            phy_pg = free_pg;
            free_pg = free_pg + 1;
            //reading from the backing store for that logical page to get physical page/address
            memcpy(main_mem + phy_pg *PAGE_SIZE, backingStore_ptr + log_pg * PAGE_SIZE, PAGE_SIZE);
            pg_table[log_pg] = phy_pg;
        }
        
        int phy_add = (phy_pg << OFFSET_BITS) | offset;
        signed char val = main_mem[phy_pg * PAGE_SIZE + offset];

        fprintf(output_fp, "LOGGICAL ADDRESS : %d   PHYSICAL ADDRESS : %d   VALUE : %d \n", log_add, phy_add, val);
        printf("PAGE# : %d      OFFSET : %d \n", log_pg, offset);
        total_add = total_add + 1;
    }
    printf("PAGE FAULTS : %d \n", pg_fault);
    printf("PAAGE FAULT RATE (percentage) : %.1f \n", (pg_fault / (total_add*1.))*100);
    printf("TOTAL NUMBER OF PAGES : %d \n", total_add);
    return 0;
}