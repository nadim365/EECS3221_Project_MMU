#include <stdlib.h>
#include "func.h"
#include "tlb.h"
#define TLB_SIZE 16

//compute max of 2 given integers
int max(int x, int y)
{
    if(x > y)
      return x;

  return y;
}
//searching for physical frame number given logical page
//returns -1 if the page is not in TLB
int search_pg(unsigned char log_pg, int num, struct TLB_val x[]){
    int i;
    for (i = max((num - TLB_SIZE), 0); i < num; i++)
    {
        struct TLB_val *page = &x[i % TLB_SIZE];
        if (page->log_pg == log_pg)
        {
            return page->phy_pg;
        }
        
    }
    return -1;
}
//add the supplied logical page and physical page to TLB using FIFO replacement
//returns updated index for TLB
int add_pg(unsigned char log_pg, unsigned char phy_pg, struct TLB_val x[], int num){
    struct TLB_val *page = &x[num % TLB_SIZE];
    num = num + 1;
    page->log_pg = log_pg;
    page->phy_pg = phy_pg;
    return num;    
}