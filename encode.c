# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# include "encode.h"
# include "analysis.h"

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

size_t getSmallestVersion(int mod, size_t count, int correction_level)
{
  size_t version = 0;
  switch(correction_level)
  {
    case 1: // M
      {
        for(size_t vers = 1; vers < 41; ++vers) {
          if(M_LEVEL[mod][vers] >= count) {
            version = vers;
            break;
          }
        }
        break;
      }
    case 2: // Q
      {
        for(size_t vers = 1; vers < 41; ++vers) {
          if(Q_LEVEL[mod][vers] >= count) {
            version = vers;
            break;
          }
        }
        break;
      }
    case 3: // H
      {
        break;
      }
    default: // L
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

size_t getEncodedSize(struct options *arg)
{  
  size_t char_count = getSize(arg->message);
  char *count_bits = convertToByte(char_count);

  printf("size_t to bits : %li -> [%s]\n", char_count, count_bits);

  //size_t num_bits = getSize(count_bits);

  size_t version = getSmallestVersion(1, char_count, arg->correction);
  printf("Smallest version is : v.%li\n \t| Characters counter : %li\n", version, char_count);

  free(count_bits);
  return 0;
}
