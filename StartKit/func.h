#include "tlb.h"

int search_pg(unsigned char log_pg, int index, struct TLB_val x[]);
void add_pg(unsigned char log_pg, unsigned char phy_pg, struct TLB_val x[], int index);