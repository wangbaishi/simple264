#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>

#define NUM_BYTES   64

struct simple264_stream{
    int fd;
    unsigned int bitIdx;
    char data[NUM_BYTES];
    int validBytes;
};

static struct simple264_stream stream;

int stream_open(char *fileName)
{
    int ret;
    int fd;
    
    fd = open(fileName, O_RDONLY);
    
    if (fd < 0)
    {
        printf("failed to open file %s\n", fileName);
        return -1;
    }
    
    stream.fd = fd;
    
    ret = read(fd, stream.data, NUM_BYTES);
    if (ret != NUM_BYTES)
    {
        printf("failed to read data from stream\n");
        close(fd);
        return -1;
    }
    
    stream.bitIdx = 0;
    stream.validBytes = NUM_BYTES;
    
    return 0;
}

int stream_close(void)
{
    close(stream.fd);
}

int stream_get_one_bit(void)
{
    int byteIdx = stream.bitIdx / 8;
    int bitIdx = stream.bitIdx % 8;
    
    char byte = stream.data[byteIdx];
    
    // Annex B : MSB is ordered first
    unsigned char mask = 0x1 << ( 7 - bitIdx );
    
    if (byteIdx >= stream.validBytes)
    {
        printf("not enough bytes in stream\n");
        return -1;
    }
    
    byte &= mask;
    byte >>= bitIdx;
    
    stream.bitIdx ++;
    
    if (stream.bitIdx == 8 * NUM_BYTES)
    {
        ret = read(fd, stream.data, NUM_BYTES);
        if (ret < 0)
        {
            printf("failed to refill data from stream\n");
            return -1;
        }
        
        stream.validBytes = ret;
        stream.bitIdx = 0;
    }
    
    return byte; 
}

int stream_get_bits(int bitNum)
{
    int value = 0;
    int bitIdx = stream.bitIdx % 8;
    
    unsigned char mask = 0x1 << bitIdx;
    
    if (bitNum + bitIdx > stream.validBytes * 8)
    {
        printf("not enough bits in stream\n");
        return -1;
    }
    
    while (bitNum -- > 0)
    {
        value <<= 1;
        value += stream_get_one_bit();
    }
    
    return value;
}

int stream_byte_aligned(void)
{
	return (stream.bitIdx == 0);
}
