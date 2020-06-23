
#ifndef MACROBLOCK_H
#define MACROBLOCK_H

#define MB_TYPE_I_PCM 	0

#define MB_TYPE_I_NxN	1

int process_macroblock(void);

int mb_pred(int mb_type);

int sub_mb_pred(int mb_type);

#endif