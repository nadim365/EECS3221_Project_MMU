#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tlb.h"
#define SIZE 100

//Page and fram and TLB specifications
#define TLB_SIZE 16
#define PAGES 256
#define PAGE_MASK 255
#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255
#define MEM_size PAGES * PAGE_SIZE

int pg_table[PAGES];
signed char main_mem[MEM_size];
signed char *backingStore_ptr;
struct TLB_val tlb[TLB_SIZE];
int index = 0; // count how many pages inserted into TLB

int main(int argc, const char *argv[])
{
    if (argc != 4)
    {
        printf("please enter 4 arguements : ./mmu 256 <Backing Store> <Input File>");
        exit(0);
    }
    
     //initialize entries of page table to -1 for empty table
     for (int i = 0; i < PAGES; i++)
     {
         pg_table[i] = -1;
     }

    FILE *input;
    FILE *output;
    const char *backStore_file = argv[2]; //variable to hold the bin file name
    const char *input_file = argv[3]; //variable to hold address of addresses.txt file
    const char *output_file = "output256.csv"; //file name where output is written

    int backingStore_fd = open(backStore_file, O_RDONLY); //file descriptor for reading from Backing Store .bin file 
    backingStore_ptr = mmap(0, MEM_size, PROT_READ, MAP_PRIVATE,backingStore_fd,0);
    input = fopen(input_file, "r"); //opening file to be read
    output = fopen(output_file, "w"); //opening file to write output to

    char pg[SIZE]; //to store lines read from input file
    int total_add = 0; //store total number of addresses read
    int pg_fault = 0; //counter for page faults

    if (input == NULL)
    {
        printf("file does not exist!");
        exit(1);        
    }
    unsigned char free_pg = 0;
    while(fgets(pg,SIZE,input) != NULL)
    {
        int log_add = atoi(pg);
        int offset = log_add & OFFSET_MASK;
        int log_pg = (log_add >> OFFSET_BITS) & PAGE_MASK;
        int phy_pg = pg_table[log_pg];
        total_add = total_add + 1;
        if (phy_pg == -1) //check if page exists in page table, if value = -1 then page fault
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

        fprintf(output, "%d,%d,%d\n", log_add, phy_add, val);
       //printf("PAGE# : %d      OFFSET : %d \n", log_pg, offset);
        
    }
    //printf("PAGE FAULTS : %d \n", pg_fault);
    fprintf(output, "Page Faults Rate, %.2f%%,\n", (pg_fault / (total_add*1.))*100);
    //printf("PAAGE FAULT RATE (percentage) : %.2f \n", (pg_fault / (total_add*1.))*100);
    printf("TOTAL NUMBER OF PAGES : %d \n", total_add);
    return 0;
}