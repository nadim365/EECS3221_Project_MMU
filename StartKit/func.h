#include "tlb.h"
//functions for the TLB to search and add new pages
int max(int x, int y);
int search_pg(unsigned char log_pg, int index, struct TLB_val x[]);
int add_pg(unsigned char log_pg, unsigned char phy_pg, struct TLB_val x[], int index);