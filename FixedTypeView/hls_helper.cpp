/*
 * =====================================================================================
 *
 *       Filename:  hls_helper.cpp
 *
 *    Description:  hls help functions.
 *
 *        Version:  1.0
 *        Created:  8/17/2021 4:36:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexander Hsu (), chenyu.xu@surgnova.com
 *   Organization:  Surgnova
 *
 * =====================================================================================
 */

#include "hls_helper.h"

#include <iostream>
#include <bitset>
using namespace std;

#include <string.h>
#include <math.h>
#include <stdint.h>

namespace hls_helper {

// Assuming that w won't larger then 32
double hls_apfixed_to_double(unsigned int bits, unsigned int W, unsigned int I) {
  uint32_t mask0 = 0xFFFFFFFF;
  for (int i = 0; i < I; i++) {
    mask0 &= (0xFFFFFFFF << (i + 1));
  }
  int32_t ivalue = ((bits >> (W - 1)) & 0x01) ? (mask0 | (bits >> (W - I))) : (bits >> (W - I));
  uint64_t mask1 = 0;
  for (int i = 0; i < (W - I); i++) {
    mask1 |= (0x1 << i);
  }
  double dvalue = bits & mask1;
  return ivalue + dvalue / pow(2, (W - I));
}

unsigned int double_to_hls_apfixed(double v, unsigned int W, unsigned I) {
  uint64_t res;
  if (v > 0) {
    uint32_t ivalue = floor(v);
    double dleft = v - ivalue;
    uint64_t dvalue = (uint64_t)(dleft * pow(2, (W - I)));
    res = (ivalue << (W - I)) | dvalue;
  } else if (v < 0 ) {
    uint32_t ivalue = ceil(0 - v);
    double dleft = ivalue + v;
    int64_t dvalue;
    if (dleft == 1) {
      dvalue = -1;
    } else {
      dvalue = (int64_t)(dleft * pow(2, (W - I)));
    }
    res = ((0 - ivalue) << (W - I)) | dvalue;
  } else {
    res = 0;
  }
  return res;
}

}
