#ifndef TLB_H
#define TLB_H

// representation of a entry in TLB
struct TLB_val{
    unsigned char log_pg;
    unsigned char phy_pg;
};
#endif