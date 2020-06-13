
/* definition of functions to manage stream */

#ifndef STREAM_H
#define STREAM_H

int stream_open(char *fileName);

int stream_close(void);

int stream_get_one_bit(void);

int stream_get_bits(int bitNum);

#endif