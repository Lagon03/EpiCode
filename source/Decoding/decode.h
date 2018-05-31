# ifndef _DECODE_H_
# define _DECODE_H_

struct dData
{
    int mode;       // 0001 : Numeric mode
                    // 0010 : Alphanumeric mode
                    // 0100 : Byte mode
    int version;    // 1 to 40
    int level;      // 0 : Low
                    // 1 : Medium
                    // 2 : Quartile
                    // 3 : High
    char* data;
};

char* listTakeAt(char* src, size_t start, size_t len);

char* decode(char* input, int version, int level);

# endif
