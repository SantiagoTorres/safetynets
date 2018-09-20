#ifndef MATH_H
#define MATH_H

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>

#include <sstream>
#include <vector>



#define MASK 4294967295 //2^32-1
#define PRIME 2305843009213693951 //2^61-1

typedef unsigned long long uint64;

uint64 myPow(uint64 x, uint64 b);
uint64 myMod(uint64 x);
uint64 myModPow(uint64 b, uint64 e);
void extEuclideanAlg(uint64 u, uint64* u1, uint64* u2, uint64* u3);
uint64 inv(uint64 a);
uint64 chi(uint64 v, uint64* r, uint64 n);
uint64 evaluate_V_i(int mi, int ni, uint64* level_i, uint64* r);

//efficient modular multiplication function mod 2^61-1
inline uint64 myModMult(uint64 x, uint64 y)
{
    uint64 hi_x = x >> 32;
    uint64 hi_y = y >> 32;
    uint64 low_x = x & MASK;
    uint64 low_y = y & MASK;

    //since myMod might return something slightly large than 2^61-1,
    //we need to multiply by 8 in two pieces to avoid overflow.
    uint64 piece1 = myMod((hi_x * hi_y)<< 3);
    uint64 z = (hi_x * low_y + hi_y * low_x);
    uint64 hi_z = z >> 32;
    uint64 low_z = z & MASK;

    //Note 2^64 mod (2^61-1) is 8
    uint64 piece2 = myMod((hi_z<<3) + myMod((low_z << 32)));
    uint64 piece3 = myMod(low_x * low_y);
    uint64 result = myMod(piece1 + piece2 + piece3);

    return result;
}

#endif // MATH_H
