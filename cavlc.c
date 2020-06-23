
 
#include <stdio.h>

#define COEFF_TOKEN_VLC_LEN     62

struct coeff_token{
    char trailing_ones;
    char total_coeff;
    char leading_zeros;
    char value;
    char valid;
};

struct coeff_token ct02[COEFF_TOKEN_VLC_LEN] = {
        {0, 0, 0, 1},
        {0, 1, 3, 5},
        {1, 1, 1, 1},
        {0, 2, 5, 7},
        {1, 2, 3, 4},
        {2, 2, 2, 1},
        {0, 3, 6, 7},
        {1, 3, 5, 6},
        {2, 3, 4, 5},
        {3, 3, 3, 3},
        {0, 4, 7, 7},
        {1, 4, 6, 6},
        {2, 4, 5, 5},
        {3, 4, 4, 3},
        {0, 5, 8, 7},
        {1, 5, 7, 6},
        {2, 5, 6, 5},
        {3, 5, 4, 4},
        {0, 6, 9, 0xf},
        {1, 6, 8, 6},
        {2, 6, 7, 5},
        {3, 6, 5, 4},
        {0, 7, 9, 11},
        {1, 7, 9, 0xe},
        {2, 7, 8, 5},
        {3, 7, 6, 4},
        {0, 8, 9, 8},
        {1, 8, 9, 10},
        {2, 8, 9, 0xd},
        {3, 8, 7, 4},
        {0, 9, 10, 0xf},
        {1, 9, 10, 0xe},
        {2, 9, 9, 9},
        {3, 9, 8, 4},
        {0, 10, 10, 0xb},
        {1, 10, 10, 10},
        {2, 10, 10, 0xd},
        {3, 10, 9, 0xc},
        {0, 11, 11, 0xf},
        {1, 11, 11, 0xe},
        {2, 11, 10, 9},
        {3, 11, 10, 0xc},
        {0, 12, 11, 0xb},
        {1, 12, 11, 10},
        {2, 12, 11, 0xd},
        {3, 12, 10, 8},
        {0, 13, 12, 0xf},
        {1, 13, 14, 1},
        {2, 13, 11, 9},
        {3, 13, 11, 0xc},
        {0, 14, 12, 0xb},
        {1, 14, 12, 0xe},
        {2, 14, 12, 0xd},
        {3, 14, 11, 8},
        {0, 15, 13, 7},
        {1, 15, 12, 0xa},
        {2, 15, 12, 9},
        {3, 15, 12, 0xc},
        {0, 16, 13, 4},
        {1, 16, 13, 6},
        {2, 16, 13, 5},
        {3, 16, 12, 8}
};

struct coeff_token ct02_trans[16][16];

int init_coeff_table(struct coeff_token *orig_table, struct coeff_token *table)
{
    int i;
    
    for (i = 0; i < COEFF_TOKEN_VLC_LEN; i ++)
    {
        table[orig_table[i].leading_zeros][orig_table[i].value].trailing_ones = orig_table[i].trailing_ones;
        table[orig_table[i].leading_zeros][orig_table[i].value].total_coeff = orig_table[i].total_coeff;
        table[orig_table[i].leading_zeros][orig_table[i].value].valid = 1;
    }
    
    return 0;
}

struct coeff_token *decode_coeff_vlc(struct coeff_token *table)
{
    int leading_zeros = 0;
    int value = 1;
    
    char bit;
    
    int counter = 0;
    
    while ((bit = stream_get_one_bit()) == 0)
        leading_zeros ++;
    
    do{
        if (ct02_trans[leading_zeros][value].valid == 1)
            return &(table[leading_zeros][value]);
        
        value <<= 1;
        value += stream_get_one_bit();
        
        count ++;
    }while(count < 4);
    
    return NULL;
}

int decode_level(int total_coeff, int t_one, int *level)
{
    int i = 0;
    
    for (i = 0; i < t_one; i ++)
    {
        level[i] = 1 - 2 * stream_get_one_bit();
    }
    
    
}
    
        
        