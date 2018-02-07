# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

# include "../headers/encode.h"
# include "../headers/encode_message.h"
# include "../headers/analysis.h"

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

// TODO ADD H_LEVEL FOR H CORRECTION

const char SpecAdd[2][8] = { {'1','1','1','0','1', '1', '0', '0'},
  {'0','0','0','1','0','0','0','1'}};


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
  data = realloc(data, (data_s + (mulE - size) + 1) * sizeof(char));
  data[data_s + mulE - size] = '\0';
  for(; size < mulE; ++data_s, ++size)
    data[data_s] = '0';

  // Now we can add the specified supplementary 8-bits codewords
  data = realloc(data, (data_s + (length - size) + 1) * sizeof(char));

  data[data_s + length - size] = '\0';
  size_t repeat = (length - size) / 8;
  for(size_t i = 0; i < repeat; ++i) {
    for(size_t j = 0; j < 8; ++j, ++data_s, ++size) {
      if(i % 2 == 0)
        data[data_s] = SpecAdd[0][j];
      else
        data[data_s] = SpecAdd[1][j];
    }
  }

  // be sure that the pointer is reassigned otherwise we might have 
  // heap-use-after-free errors
  input->encoded_data = data;
}

//#############################################################################
//                                END OF TOOLS
//#############################################################################

char** breakCodeword(struct EncData* data)
{
  size_t f_size = getSize(data->character_count_ind) 
    + 4 + getSize(data->encoded_data) + 1;
  char *full_data = malloc(f_size * sizeof(char));
  
  full_data = strcpy(full_data, data->mode_ind);
  full_data = strcat(full_data, data->character_count_ind);
  full_data = strcat(full_data, data->encoded_data);

  // Number of codewords
  int nb_cw = TOTAL_DECC[data->correction_level][data->version];
  // Current position in encoded data
  int cur = 0;

  char **codewords = malloc(nb_cw * sizeof(char*));
  for(int id = 0; id < nb_cw; ++id) { // we set the codewords
    codewords[id] = malloc(9 * sizeof(char));
    codewords[id][8] = '\0';
    for(int i = 0; i < 8; ++i, ++cur)
      codewords[id][i] = full_data[cur];
    printf("\t Codeword n°%3i: %s\n", id, codewords[id]);
  }
  free(full_data);
  return codewords;
}

struct EncData* getEncodedSize(struct options *arg)
{  
  struct EncData *data = malloc(sizeof(struct EncData));

  size_t char_count = getSize(arg->message);
  char *count_bits = convertToByte(char_count);

  size_t version = getSmallestVersion(1, char_count, arg->correction); 
  // need to specify the limit in function of the version and of the mode
  if(version <= 9) {
    if(arg->mode == 0)
      count_bits = adjustSize(count_bits, 10);
    else if(arg->mode == 1)
      count_bits = adjustSize(count_bits, 9);
    else
      count_bits = adjustSize(count_bits, 8);
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
  adjustBits(data, full_size);
  char **codewords = breakCodeword(data);
  for(int i = 0; i < 13; ++i)
    free(codewords[i]);
  free(codewords);
  // if the size of the whole encoded data is still different from a multiple
  // of 8, we must add zero to the left of it

  // Now we need to break the whole (mode indicator + characters count +
  // encoded data) into 8-bits Codewords to prepare the error correction
  return data;
}
