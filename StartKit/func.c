#include "func.h"
#include "tlb.h"

int search_pg(unsigned char log_pg, int index, struct TLB_val x[]){
    int i;
    for (i = max((index - 16), 0); i < index; i++)
    {
        struct TLB_val *page = &x[i & 16];
        if (page->log_add == log_pg)
        {
            return page->phy_add;
        }
        
    }
    return -1;
}

void add_pg(unsigned char log_pg, unsigned char phy_pg, struct TLB_val x[], int index){
    struct TLB_val *page = &x[index % 16]
    page->log_add = log_pg;
    page->phy_add = phy_pg;    
}