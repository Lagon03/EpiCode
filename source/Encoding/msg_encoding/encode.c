# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

# include "../headers/encode.h"
# include "../headers/encode_message.h"
# include "../headers/encode_RS.h"
# include "../headers/analysis.h"
# include "../headers/polynomials.h"

//-----------------------------------------------------------------------------
//                              HARD CODED CONST
//-----------------------------------------------------------------------------

const size_t TOTAL_DECC[4][41] = {
    // Version: (note that index 0 is for padding, and is set to an illegal value)
    {-1, 19, 34, 55, 80, 108, 136, 156, 194, 232, 274, 324, 370, 428, 461, 523, 
        589, 647, 721, 795, 861, 932, 1006, 1094, 1174, 1276, 1370, 1468, 1531, 1631,
        1735, 1843, 1955, 2071, 2191, 2306, 2434, 2566, 2702, 2812, 2956},  // Low
    {-1, 16, 28, 44, 64, 86,  108, 124, 154, 182, 216, 254, 290, 334, 365, 415, 
        453, 507, 563, 627, 669, 714, 782, 860, 914, 1000, 1062, 1128, 1193, 1267,
        1373, 1455, 1541, 1631, 1725, 1812, 1914, 1992, 2102, 2216, 2334},  // Medium
    {-1, 13, 22, 34, 48, 62,  76,   88, 110, 132, 154, 180, 206, 244, 261, 295, 
        325, 367, 397, 445, 485, 512, 568, 614, 664, 718, 754, 808, 871, 911, 985, 
        1033, 1115, 1171, 1231, 1286, 1354, 1426, 1502, 1582, 1666},  // Quartile
    {-1, 9 , 16, 26, 36, 46,  60,   66,  86, 100, 122, 140, 158, 180, 197, 223,
        253, 283, 313, 341, 385, 406, 442, 464, 514, 538, 596, 628, 661, 701, 745, 
        793, 845, 901, 961, 986, 1054, 1096, 1142, 1222, 1276}  // High
};

//  If group 1 or 2 ------------|
const size_t GROUP_CODEWORDS[4][2][41] = {
    { 
        {-1, 19, 34, 55, 80, 108, 68, 78, 97, 116, 68, 81, 92, 107, 115, 87, 98, 
            107, 120, 113, 107, 116, 111, 121, 117, 106, 114, 122, 117, 116, 115,
            115, 115, 115, 115, 121, 121, 122, 122, 117, 118},
        {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 69, 0, 93, 0, 116, 88, 99, 108, 121, 114,
            108, 117, 112, 122, 118, 107, 115, 123, 118, 117, 116, 116, 0, 116, 116,
            122, 122, 123, 123, 118, 119 } 
    }, // Low
    { 
        {-1, 16, 28, 44, 32, 43, 27, 31, 38, 36, 43, 50, 36, 37, 40, 41, 45, 46,
            43, 44, 41, 42, 46, 47, 45, 47, 46, 45, 45, 45, 47, 46, 46, 46, 46,
            47, 47, 46, 46, 47, 47},
        {-1, 0, 0, 0, 0, 0, 0, 0, 39, 37, 44, 51, 37, 38, 41, 42, 46, 47, 44, 45,
            42, 0, 0, 48, 46, 48, 47, 46, 46, 46, 48, 47, 47, 47, 47, 48, 48,
            47, 47, 48, 48}
    }, // Medium
    {
        {-1, 13, 22, 17, 24, 15, 19, 14, 18, 16, 19, 22, 20, 20, 16, 24, 19, 22,
            22, 21, 24, 22, 24, 24, 24, 24, 22, 23, 24, 23, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24},
        {-1, 0, 0, 0, 0, 16, 0, 15, 19, 17, 20, 23, 21, 21, 17, 25, 20, 23, 23,
            22, 25, 23, 25, 25, 25, 25, 23, 24, 25, 24, 25, 25, 25, 25, 25, 25,
            25, 25, 25, 25, 25} 
    },// Quartile
    {
        {-1,9,16,13,9,11,15,13,14,12,15,12,14,11,12,12,15,14,14,13,15,16,13,15,16,
            15,16,15,15,15,15,15,15,15,16,15,15,15,15,15,15 },
        {-1, 0,0,0,0,12,0,14,15,13,16,13,15,12,13,13,16,15,15,14,16,17,0,16,17,16,
            17,16,16,16,16,16,16,16,17,16,16,16,16,16,16} 
    } // High
};

const size_t GROUP_BLOCK_CODEWORDS[2][4][41] = {
    {
        {-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 4, 2, 4, 3, 5, 5, 1, 5, 3, 3, 4, 2, 4, 6,
            8, 10, 8, 3, 7, 5, 13, 17, 17, 13, 12, 6, 17, 4, 20, 19},
        {-1, 1, 1, 1, 2, 2, 4, 4, 2, 3, 4, 1, 6, 8, 4, 5, 7, 10, 9, 3, 3, 17, 17, 
            4, 6, 8, 19, 22, 3, 21, 19, 2, 10, 14, 14, 12, 6, 29, 13, 40, 18},
        {-1, 1, 1, 2, 2, 2, 4, 2, 4, 4, 6, 4, 4, 8, 11, 5, 15, 1, 17, 17, 15, 17,
            7, 11, 11, 7, 28, 8, 4, 1, 15, 42, 10, 29, 44, 39, 46, 49, 48, 43, 34},
        {-1, 1, 1, 2, 4, 2, 4, 4, 4, 4, 6, 3, 7, 12, 11, 11, 3, 2, 2, 9, 15, 19, 34,
            16, 30, 22, 33, 12, 11, 19, 23, 23, 19, 11, 59, 22, 2, 24, 42, 10, 20}
    },
    {
        {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 1, 1, 1, 5, 1, 4, 5, 4, 7, 5, 4,
            4, 2, 4, 10, 7, 10, 3, 0, 1, 6, 7, 14, 4, 18, 4, 6},
        {-1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 1, 4, 2, 1, 5, 5, 3, 1, 4, 11, 13, 0, 0, 14,
            14, 13, 4, 3, 23, 7, 10, 29, 23, 21, 23, 26, 34, 14, 32, 7, 31},
        {-1, 0, 0, 0, 0, 2, 0, 4, 2, 4, 2, 4, 6, 4, 5, 7, 2, 15, 1, 4, 5, 6, 16, 14,
            16, 22, 6, 26, 31, 37, 25, 1, 35, 19, 7, 14, 10, 10, 14, 22, 34},
        {-1, 0, 0, 0, 0, 2, 0, 1, 2, 4, 2, 8, 4, 4, 5,  7, 13, 17, 19, 16, 10, 6, 0, 14,
            2, 13, 4, 28, 31, 26, 25, 28, 35, 46, 1, 41, 64, 46, 32, 67, 61}
    }
};

const int8_t ECC_CODEWORDS_PER_BLOCK[4][41] = {
    // Version: (note that index 0 is for padding, and is set to an illegal value)
    {-1,  7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30,
        28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
        30, 30, 30},  // Low
    {-1, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26,
        26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
        28, 28, 28},  // Medium
    {-1, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28,
        26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
        30, 30, 30},  // Quartile
    {-1, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28,
        26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
        30, 30, 30},  // High
};

// For generating error correction codes.
const int8_t NUM_ERROR_CORRECTION_BLOCKS[4][41] = {
    // Version: (note that index 0 is for padding, and is set to an illegal value)
    {-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,
        8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24,
        25},  // Low
    {-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16,
        17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47,
        49},  // Medium
    {-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20,
        23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65,
        68},  // Quartile
    {-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25,
        25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77,
        81},  // High
};

// Characters capacities

const size_t L_LEVEL[3][41] = {
    // Version: (note that index 0 is for padding, and is set to an illegal value)
    {-1, 41, 77, 127, 187, 255, 322, 370, 461, 552, 652, 772, 883, 1022, 1101,
        1250, 1408, 1548, 1725, 1903, 2061, 2232, 2409, 2620, 2812, 3057, 3283, 3517,
        3669, 3909, 4158, 4417, 4686, 4965, 5253, 5529, 5836, 6153, 6479, 6743,
        7089},  
    // Numeric
    {-1, 25, 47, 77,  114, 154, 195, 224, 289, 335, 395, 468, 535,  619,  667,
        758,  854, 938,  1046, 1153, 1249, 1352, 1460, 1588, 1704, 1853, 1990, 2132,
        2223, 2369, 2520, 2677, 2840, 3009, 3183, 3351, 3537, 3729, 3927, 4087,
        4296},
    // Alpha
    {-1, 17, 32, 53,   78, 106, 134, 154, 192, 230, 271, 321, 367,  425,  458,
        520,  586, 644,   718,  792,  858,  929, 1003, 1091, 1171, 1273, 1367, 1465,
        1528, 1628, 1732, 1840, 1952, 2068, 2188, 2303, 2431, 2563, 2699, 2809, 2953}
    // Byte 
};


const size_t M_LEVEL[3][41] = {
    { -1, 34, 63, 101, 149, 202, 255, 293, 365, 432, 513, 604, 691, 796, 871,
        991, 1082, 1212, 1349, 1346, 1500, 1600, 1708, 2059, 2188, 2395, 2544, 2701,
        2857, 3035, 3289, 3486, 3693, 3909, 4134, 4343, 4588, 4775, 5039, 5313,
        5596}, 
    // Numeric
    { -1, 20, 38, 61, 90, 122, 154, 178, 221, 262, 311, 366, 419, 483, 528, 600,
        656, 734, 816, 909, 970, 1035, 1134, 1248, 1326, 1451, 1542, 1637, 1732,
        1839, 1994, 2113, 2238, 2369, 2506, 2632, 2780, 2894, 3054, 3220, 3391},
    // Alpha
    { -1, 14, 26, 42, 62, 84, 106, 122, 152, 180, 213, 251, 287, 331, 362, 412,
        450, 504, 560, 624, 666, 711, 779, 857, 911, 997, 1059, 1125, 1190, 1264,
        1370, 1452, 1538, 1628, 1722, 1809, 1911, 1989, 2099, 2213, 2331} 
    // Byte 
};

const size_t Q_LEVEL[3][41] = {
    {-1, 27, 48, 77, 111, 144, 178, 207, 259, 312, 364, 489, 580, 621, 703, 775,
        876, 948, 1063, 1159, 1224, 1358, 1468, 1588, 1718, 1804, 1933, 2085, 2181,
        2358, 2473, 2670, 2805, 2949, 3081, 3244, 3417, 3599, 3791, 3993}, 
    // Numeric
    {-1, 16, 29, 47, 67,   87, 108, 125, 157, 189, 221, 296, 352, 376, 426, 470,
        531, 574,  644,  702,  742,  823,  890,  963, 1041, 1094, 1172, 1263, 1322,
        1429, 1499, 1618, 1700, 1787, 1867, 1966, 2071, 2181, 2298, 2420}, 
    // Alpha
    {-1, 11, 20, 32, 46,   60,  74,  86, 108, 130, 151, 203, 241, 258, 292, 322,
        364, 394,  442,  482,  509,  565,  611,  661,  715,  751,  805,  868,  908,
        982, 1030, 1112, 1168, 1228, 1283, 1351, 1423, 1499, 1579, 1663}, 
    // Byte 
};

const size_t H_LEVEL[3][41] = {
    {-1, 17, 34, 58, 82, 106, 139, 154, 202, 235, 288, 331, 374, 427, 468, 530,
        602, 674, 746, 813, 919, 969, 1056, 1108, 1228, 1286, 1425, 1501, 1581,
        1677, 1782, 1897, 2022, 2157, 2301, 2361, 2524, 2625, 2735, 2927, 3057},
    {-1, 10, 20, 35, 50, 64, 84, 93, 122, 143, 174, 200, 227, 259, 283, 321,
        365, 408, 452, 493, 557, 587, 640, 672, 744, 779, 864, 910, 958, 1016,
        1080, 1150, 1226, 1307, 1394, 1431, 1530, 1591, 1658, 1774, 1852}, 
    {-1, 7, 14, 24, 34, 44, 58, 64, 84, 98, 119, 137, 155, 177, 194, 220, 250,
        280, 310, 338, 382, 403, 439, 461, 511, 535, 593, 625, 658, 698, 742,
        790, 842, 898, 958, 983, 1051, 1093, 1139, 1219, 1273}
};

const char SpecAdd[2][8] = { {'1','1','1','0','1', '1', '0', '0'},
    {'0','0','0','1','0','0','0','1'}};

const size_t Remainder_bits[41] = { -1, 0, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0,
    3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0,
    0, 0 };

const char* S_bits[4][8] =
{ 
    {   "111011111000100", // Low
        "111001011110011",
        "111110110101010",
        "111100010011101",
        "110011000101111",
        "110001100011000",
        "110110001000001",
        "110100101110110", 
    }, 
    {   "101010000010010", // Medium
        "101000100100101",
        "101111001111100",
        "101101101001011",
        "100010111111001",
        "100000011001110",
        "100111110010111",
        "100101010100000", 
    }, 
    {   "011010101011111", // Quartile
        "011000001101000",
        "011111100110001",
        "011101000000110",
        "010010010110100",
        "010000110000011",
        "010111011011010",
        "010101111101101", 
    }, 
    {   "001011010001001", // High
        "001001110111107",
        "001110011100111",
        "001100111010000",
        "000011101100010",
        "000001001010101",
        "000110100001100",
        "000100000111011" 
    }
};

const char* V_bits[34] =
{
    "000111110010010100",   //7
    "001000010110111100",   //8
    "001001101010011001",   //9
    "001010010011010011",   //10
    "001011101111110110",   //11
    "001100011101100010",   //12
    "001101100001000111",   //13
    "001110011000001101",  //14
    "001111100100101000",  //15
    "010000101101111000",  //16
    "010001010001011101",  //17
    "010010101000010111",  //18
    "010011010100110010",  //19
    "010100100110100110",  //20
    "010101011010000011",  //21
    "010110100011001001",  //22
    "010111011111101100",  //23
    "011000111011000100",  //24
    "011001000111100001",  //25
    "011010111110101011",  //26
    "011011000010001110",  //27
    "011100110000011010",  //28
    "011101001100111111",  //29
    "011110110101110101",  //30
    "011111001001010000",  //31
    "100000100111010101",  //32
    "100001011011110000",  //33
    "100010100010111010",  //34
    "100011011110011111",  //35
    "100101010000101110",  //36
    "100101010000101110",  //37
    "100110101001100100",  //38
    "100111010101000001",  //39
    "101000110001101001"   //40  
};
//-----------------------------------------------------------------------------
//                            END OF HARD CODE
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//                            Tools functions
//-----------------------------------------------------------------------------

// Simple pow function
size_t sPow(size_t x, int n)
{
    size_t res = 1;
    if(n > 0)
        while(n > 0) {
            res *= x;
            --n;
        }
    else
        while(n < 0) {
            res >>= x;
            ++n;
        }
    return res;
}

// Function used to retrieve the last inferior power of two
size_t getLastInf(size_t x)
{
    size_t pow = 0;
    while(x >= sPow(2, pow))
        ++pow;
    return pow;
}

// Function used to convert size_t to bit array
char* convertToByte(size_t input)
{
    size_t len = (getLastInf(input) * sizeof(char)) + 1;
    char *array = calloc(len, sizeof(char));
    array[len - 1] = '\0';

    size_t index = len - 2;

    while(input > 0) {
        if(input % 2 == 0)
            array[index] = '0';
        else
            array[index] = '1';

        input >>= 1;
        --index;
    }
    return array;
}

size_t convertToDec(char* input) {
    //printf("Input to convert : %s\n", input);
    size_t dec = 0;
    size_t length = getSize(input);
    for(size_t i = 0; i < length; ++i) {
        if(input[i] == '1')
            dec += sPow(2,(length - 1) - i);
    }
    return dec;
}

static size_t getSmallestVersion(int mod, size_t count, int correction_level)
{
    size_t version = 0;
    switch(correction_level)
    {
        case 1:   // M
            {
                for(size_t vers = 1; vers < 41; ++vers) {
                    if(M_LEVEL[mod][vers] >= count) {
                        version = vers;
                        break;
                    }
                }
                break;
            }
        case 2:   // Q
            {
                for(size_t vers = 1; vers < 41; ++vers) {
                    if(Q_LEVEL[mod][vers] >= count) {
                        version = vers;
                        break;
                    }
                }
                break;
            }
        case 3:   // H
            {
                for(size_t vers = 1; vers < 41; ++vers) {
                    if(H_LEVEL[mod][vers] >= count) {
                        version = vers;
                        break;
                    }
                }
                break;
            }
        default:  // L
            {
                for(size_t vers = 1; vers < 41; ++vers) {
                    if(L_LEVEL[mod][vers] >= count) {
                        version = vers;
                        break;
                    }
                }
                break;
            }
    }
    return version;
}

char* adjustSize(char* bits, int limit) {
    // Use to add 0 if the count indicator is not X bits long
    // the 0 added are added to the left
    size_t cur_count = getSize(bits);
    char *tmp = calloc(limit + 2, sizeof(char));
    tmp[limit + 1] = '\0';
    for(int i = limit; i > -1; --i) {
        if (i - (limit - (int)cur_count) >= 0)
            tmp[i] = bits[i - (limit - cur_count)];
        else
            tmp[i] = '0';
    }
    free(bits);
    return tmp;
}


char* adjustSizeInv(char* bits, int limit) {
    // Use to add 0 if the count indicator is not X bits long
    // the 0 added are added to the left
    size_t cur_count = getSize(bits);
    char *tmp = calloc(limit + 2, sizeof(char));
    tmp[limit + 1] = '\0';
    for(int i = 0; i < limit; ++i) {
        if (i - (limit - (int)cur_count) >= 0)
            tmp[i] = bits[i - (limit - cur_count)];
        else
            tmp[i] = '0';
    }
    free(bits);
    return tmp;
}

char* getModeIndicator(int mode)
{
    char* indicator = calloc(5, sizeof(char));
    switch(mode) {
        case 0: { // Numeric indicator
                    indicator = "0001\0";
                    break;
                }
        case 1: { // Alphanumeric indicator
                    indicator = "0010\0";
                    break;
                }
        default: { // Byte indicator
                     indicator = "0100\0";
                     break;
                 }
    }
    return indicator;
}

// Function used to add 0 to the left of the input
void adjustBits(struct EncData *input, size_t length)
{
    // Full size of the encoded data
    size_t size = getSize(input->character_count_ind) + 4 
        + getSize(input->encoded_data);
    //size_t strict_l = size - 4 - getSize(input->character_count_ind);
    // We're looking for the smallest superior mulitple of 8 of size
    size_t mulE = 8;
    while(mulE < size)
        mulE += 8;
    char* data = input->encoded_data;
    size_t data_s = getSize(data);

    /*printf("\tSize to match : %li | Current message size : %li\n", mulE, data_s);
    printf("\tTotal size : %li\n", size);*/

    data = realloc(data, (data_s + (mulE - size) + 1) * sizeof(char));
    //printf("\tNew size : %li\n", data_s + (mulE - size) + 1);
    data[data_s + mulE - size] = '\0';
    for(; size < mulE; ++data_s, ++size)
        data[data_s] = '0';

    // Now we can add the specified supplementary 8-bits codewords
    data = realloc(data, (data_s + (length - size) + 1) * sizeof(char));

    data[data_s + length - size] = '\0';
    size_t repeat = (length - size) / 8;

    printf("\tNumber of repeat : %li\n", repeat);
    printf("\tLength : %li | size = %li | data_s = %li\n", length, size, data_s);

    for(size_t i = 0; i < repeat; ++i) { 
        for(size_t j = 0; j < 8; ++j, ++data_s, ++size) {
                data[data_s] = SpecAdd[i % 2][j];
        }
    }

    // be sure that the pointer is reassigned otherwise we might have 
    // heap-use-after-free errors
    input->encoded_data = data;
}

//============================================================================#
//                                END OF TOOLS
//============================================================================#

//=============================================================================
//                                MAIN FUNC
//=============================================================================

struct Codewords* breakCodeword(struct EncData* data)
{
    /*printf("\033[0;31m");
    printf("First checkpoint\n");
    printf("\033[0m");
    printf("Version = %li\n", data->version);*/
    size_t correction = data->correction_level;
    size_t version = data->version;
    size_t f_size = getSize(data->character_count_ind) 
        + 4 + getSize(data->encoded_data) + 1;
    char *full_data = malloc(f_size * sizeof(char));

    full_data = strcpy(full_data, data->mode_ind);
    full_data = strcat(full_data, data->character_count_ind);
    full_data = strcat(full_data, data->encoded_data);

    // Number of codewords
    int nb_cw = TOTAL_DECC[correction][version];
    // Current position in encoded data
    int cur = 0;

    struct Codewords *codewords = malloc(sizeof(struct Codewords));
    size_t group = GROUP_BLOCK_CODEWORDS[0][correction][version];
    size_t group_ = GROUP_BLOCK_CODEWORDS[1][correction][version];
    //printf("group 1 : %li\ngroup 2 : %li\n", group, group_);
    if(group != 0 && group_ == 0)
    {
        codewords->group = malloc(sizeof(struct Group*));
        codewords->size = 1; // number of group 
    }
    else
    {
        codewords->group = malloc(2 * sizeof(struct Group*));
        codewords->size = 2; // number of group
    }
    codewords->words = nb_cw;
    codewords->nb_block = 0;
    /*printf("\033[0;31m");
    printf("Second checkpoint\n");
    printf("\033[0m");*/

    //printf("Version %li\n", version);
    for(size_t g = 0; g < codewords->size; ++g) {
        //printf("Group %2li:\n", g + 1);
        size_t block_nb = GROUP_BLOCK_CODEWORDS[g][correction][version];
        size_t nb_cw_err = ECC_CODEWORDS_PER_BLOCK[data->correction_level][data->version];

        codewords->nb_block += block_nb;
        /*printf("Number of block : %li | Number of words : %i\n", block_nb, nb_cw);
        printf("\033[0;31m");
        printf("Third checkpoint\n");
        printf("\033[0m");*/

        if(block_nb != 0) {
            codewords->group[g] = malloc(sizeof(struct Group));
            codewords->group[g]->blocks = malloc(block_nb * sizeof(struct Block*));

            codewords->group[g]->id = g;
            codewords->group[g]->size = block_nb;

            /*printf("\033[0;31m");
            printf("Fourth checkpoint\n");
            printf("\033[0m");*/

            for(size_t b = 0; b < block_nb; ++b) {
                //printf("\tBlock %2li:\n", b + 1);
                size_t data_cd = GROUP_CODEWORDS[correction][g][version];
                //printf("\tNumber of word : %li\n", data_cd);

                codewords->group[g]->blocks[b] = malloc(sizeof(struct Block));
                codewords->group[g]->blocks[b]->words = malloc(data_cd * sizeof(char*));

                codewords->group[g]->blocks[b]->id = g;
                codewords->group[g]->blocks[b]->size = data_cd;
                
                /*printf("\033[0;31m");
                printf("Fifth checkpoint\n");
                printf("\033[0m");*/

                for(size_t w = 0; w < data_cd; ++w) {
                    codewords->group[g]->blocks[b]->words[w] = malloc(9 * sizeof(char));
                    codewords->group[g]->blocks[b]->words[w][8] = '\0';
                    codewords->group[g]->blocks[b]->words[w][1] = '6';
                    for(int i = 0; i < 8; ++i, ++cur) {
                        codewords->group[g]->blocks[b]->words[w][i] = full_data[cur];
                    }
                    /*printf("\t\tCodeword %2li: %s | value : %ld\n", w + 1, 
                      codewords->group[g]->blocks[b]->words[w], 
                      convertToDec(codewords->group[g]->blocks[b]->words[w]));*/
                }

                // Here we compute the correction codewords
                //printf("Number of code words erro : %li\n", ECC_CODEWORDS_PER_BLOCK[data->correction_level][data->version]);
                codewords->group[g]->blocks[b]->correction = malloc(nb_cw_err * sizeof(char*));
                /*size_t* err_cw = GenPolyFromCW(codewords->group[g]->blocks[b],
                        nb_cw_err);*/
                size_t* err_cw = JtoL(codewords->group[g]->blocks[b]->words, data_cd, nb_cw_err); 
                for(size_t i = 0; i < nb_cw_err; ++i)
                {
                    //printf("%li ", err_cw[i]);
                    codewords->group[g]->blocks[b]->correction[i] = malloc(8 * sizeof(char));
                    codewords->group[g]->blocks[b]->correction[i] = adjustSize(convertToByte(err_cw[i]), 8);
                }
                free(err_cw);
                
                /*printf("\033[0;31m");
                printf("Fifth-2 checkpoint\n");
                printf("\033[0m");*/

            }
        }
    }

    /*printf("\033[0;31m");
    printf("Sixth checkpoint\n");
    printf("\033[0m");*/

    //printf("Total blocks : %li\n", codewords->nb_block);
    free(full_data);
    return codewords;
}

void freeCodeWords(struct Codewords* codewords) {
    for(size_t g = 0; g < codewords->size; ++g) {
        for(size_t b = 0; b < codewords->group[g]->size; ++b ) {
            for(size_t i = 0; i < codewords->group[g]->blocks[b]->size; ++i) {
                free(codewords->group[g]->blocks[b]->words[i]);
            }
            free(codewords->group[g]->blocks[b]->words);
            free(codewords->group[g]->blocks[b]->correction);
            free(codewords->group[g]->blocks[b]);
        }
        free(codewords->group[g]->blocks);
        free(codewords->group[g]);
    }
    free(codewords->group);
    free(codewords);
}

struct EncData* getEncodedSize(struct options *arg)
{  
    struct EncData *data = malloc(sizeof(struct EncData));
   
    data->epi_v = 41;
    size_t char_count = 0;
    char* count_bits;
    
    char_count = getSize(arg->message);
    count_bits = convertToByte(char_count);
    
    //printf("%s\n", count_bits);
    size_t version = arg->version;
    size_t tmp_version = getSmallestVersion(arg->mode, char_count, arg->correction);
    size_t tmp_ev = getSmallestVersion(arg->mode, char_count / 2, arg->correction);
    if((version > 40) | (tmp_version > version))
        version = tmp_version;
    if((tmp_ev < data->epi_v) | (data->epi_v > 40))
        data->epi_v = tmp_ev;
    printf("version : %li\n",version);
    // need to specify the limit in function of the version and of the mod

    if(version <= 9) {
        if(arg->mode == 0)
            count_bits = adjustSize(count_bits, 10);    // Num
        else if(arg->mode == 1)
            count_bits = adjustSize(count_bits, 9);     // Alpha
        else
            count_bits = adjustSize(count_bits, 8);     // Byte
    }
    else if(version <= 26) {
        if(arg->mode == 0)
            count_bits = adjustSize(count_bits, 12);
        else if(arg->mode == 1)
            count_bits = adjustSize(count_bits, 11);
        else
            count_bits = adjustSize(count_bits, 16);
    }
    else {
        if(arg->mode == 0)
            count_bits = adjustSize(count_bits, 14);
        else if(arg->mode == 1)
            count_bits = adjustSize(count_bits, 13);
        else
            count_bits = adjustSize(count_bits, 16);
    }

    // --- 

    data->mode_ind = getModeIndicator(arg->mode);
    data->character_count_ind = count_bits;
    data->version = version;
    data->correction_level = arg->correction;

    //printf("Setting attribute done.\n");
    //printf("Version is : %li\n", version);
    
    char_count = getSize(arg->message);
    if(arg->mode == 0)
        data->encoded_data = num_encoding(arg->message, char_count);
    else if(arg->mode == 1)
        data->encoded_data = alpha_encoding(arg->message, char_count);
    else
        data->encoded_data = byte_encoding(arg->message, char_count);

    // Size of the encoded message
    size_t data_size = getSize(data->encoded_data);
    // size of the whole encoded data (message + mode ind + char count)
    size_t enc_size = data_size + 4 + getSize(data->character_count_ind);
    // full size according to the correction level and version * 8 (bits)
    size_t full_size = TOTAL_DECC[data->correction_level][data->version] * 8;

    /*printf("Version is %li\n", data->version);
    printf("Version size is %li\n", full_size);
    printf("Current encoded size is %li\n", enc_size);*/

    // We add terminating 0 if neccessary, maximum of 4
    if(enc_size < full_size) {
        int x = 0;
        if(full_size - enc_size <= 4) {
            x = full_size - enc_size;
            data->encoded_data = realloc(data->encoded_data,
                    data_size + (x + 1) * sizeof(char));
        }
        else {
            x = 4;
            data->encoded_data = realloc(data->encoded_data,
                    data_size + (x + 1) * sizeof(char));
        }
        data->encoded_data[data_size + x] = '\0';
        for(int i = 0; i < x && enc_size + i < full_size ; ++i)
        {
            data->encoded_data[data_size + i] = '0';
        }
        enc_size += (x + 1);
    }
    while(getSize(data->encoded_data) > full_size)
    {
        data->version += 1;
        full_size = TOTAL_DECC[data->correction_level][data->version] * 8;
    }

    // Now we need to break the whole (mode indicator + characters count +
    // encoded data) into 8-bits Codewords to prepare the error correction
    adjustBits(data, full_size);

    struct Codewords *codewords = breakCodeword(data);
    data->codewords = codewords; 
    //freeCodeWords(codewords);

    // Now we must group the codewords according to the error correction table

    return data;
}
