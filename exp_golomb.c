#include <math.h>

#include "stream.h"

unsigned int ue(void)
{
    int lzb = 0;
    int i;
    int codeNum = 0;
    
    while (stream_get_one_bit() == 0)
        lzb ++;
    
    for (i = 0; i < lzb; i ++)
    {
        codeNum <<= 1;
        codeNum += stream_get_one_bit();
    }
    
    codeNum += (pow(2, lzb) - 1);
    
    return codeNum;
}

int se(void)
{
    unsigned int k;
    int codeNum;
    
    k = ue();
    
    codeNum = pow(-1, k + 1) * ceil(k >> 1);
    
    return codeNum;
}

static int intra[] = {
            47, 31, 15, 0,  23, 27, 29, 30, 7,  11, 13, 14, 39, 43, 45, 46,
            16, 3,  5,  10, 12, 19, 21, 26, 28, 35, 37, 42, 44, 1,  2,  4,
            8,  17, 18, 20, 24, 6,  9,  22, 25, 32, 33, 34, 36, 40, 38, 41 };
static int inter[] = {
            0 , 16, 1,  2,  4,  8,  32, 3,  5,  10, 12, 15, 47, 7,  11, 13,
            14, 6,  9,  31, 35, 37, 42, 44, 33, 34, 36, 40, 39, 43, 45, 46,
            17, 18, 20, 24, 19, 21, 26, 28, 23, 27, 29, 30, 22, 25, 38, 41 };

int me(int intraFlag)
{
    int codeNum = ue();
    
    if (intraFlag)
        return intra[codeNum];
    return inter[codeNum];
}