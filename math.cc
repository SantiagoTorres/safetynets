/*
 * math module
 *
 * This module contains math-related, foundational code upon which the
 * outsorced inference builds upon.
 */
#include "math.h"


/* myPow
 *
 * computes x^b, that is, the power operation (not xor)
 *
 * Params:
 *  uint64 x: base operand
 *  uint64 b: power operand
 *
 * Returns:
 *  uint64 the resulting value of the power operation.
 */
uint64 myPow(uint64 x, uint64 b)
{
    uint64 i = 1;
    for (int j = 0; j <b; j++)  i *= x;
    return i;
}

/*
 * myMod:
 *
 *   efficient modular arithmetic function for p=2^61-1. Only works for this
 *   value of p.
 *
 *   This function might return a number slightly greater than p (possibly by
 *   an additive factor of 8); It'd be cleaner to check if the answer is
 *   greater than p and if so subtract p, but I don't want to pay that
 *   efficiency hit here, so the user should just be aware of this.
 *
 * Params:
 *   uint64 x: the number to compute the module of
 *
 * Return:
 *   uint64 x mod p;
 */
uint64 myMod(uint64 x)
{
    return (x >> 61) + (x & PRIME);
}

/*
 * myModPow:
 *   computes b^e mod p using repeated squaring. p should be 2^61-1.
 *
 * Params:
 *   uint64 b: the base number
 *   uint64 e: the exponent p
 *
 * Returns:
 *   uint64: the result of b^e mod p
 */
uint64 myModPow(uint64 b, uint64 e)
{
    uint64 result;
    if(e==1) return b;
    if(e == 0) return 1;
    if((e & 1) == 0)
    {
        result = myModPow(b, (e>>1));
        return myModMult(result, result);
    }
    else
        return myModMult(myModPow(b, e-1), b);
}

/*
 * extEuclideanAlg:
 *    Performs Extended Euclidean Algorithm, which is used for computing
 *    multiplicative inverses mod p
 *
 * Params:
 *    uint64 u:  the first parameter to the EUA
 *    uint64 u1: the second parameter to the EUA
 *    uint64 u2: the third parameter to the EUA
 *    uint64 u3: the fourth parameter to the EUA
 *
 * Returns:
 *    uint64: the result of the computation of the Extended Euclidean Algorithm
 */
void extEuclideanAlg(uint64 u, uint64* u1, uint64* u2, uint64* u3)
{
    *u1 = 1;
    *u2 = 0;
    *u3 = u;
    uint64 v1 = 0;
    uint64 v2 = 1;
    uint64 v3 = PRIME;
    uint64 q;
    uint64 t1;
    uint64 t2;
    uint64 t3;
    do
    {
        q = *u3 / v3;
        //t1 = *u1 + p - q * v1;
        //t2 = *u2 + p - q*v2;
        //t3 = *u3 + p - q*v3;
        t1 = myMod((*u1) + PRIME - myModMult(q, v1));
        t2 = myMod((*u2) + PRIME - myModMult(q, v2));
        t3 = myMod((*u3) + PRIME - myModMult(q, v3));
        (*u1) = v1;
        (*u2) = v2;
        (*u3) = v3;
        v1 = t1;
        v2 = t2;
        v3 = t3;
    }while(v3 != 0 && v3!= PRIME);
}


/*
 * inv:
 *    Computes the modular multiplicative inverse of a modulo m, using the
 *    extended Euclidean algorith only works for p=2^61-1
 *
 * Params:
 *    uint64 a: the value to which compute the inverse of.
 *
 * Returns:
 *    uint64: the inverse of a.
 */
uint64 inv(uint64 a)
{
    uint64 u1;
    uint64 u2;
    uint64 u3;
    extEuclideanAlg(a, &u1, &u2, &u3);
    if(u3==1)
        return myMod(u1);
    else
        return 0;
}

/* chi:
 *    computes chi_v(r), where chi is the Lagrange polynomial that takes
 *    boolean vector v to 1 and all other boolean vectors to 0. (we view v's
 *    bits as defining a boolean vector). n is dimension of this vector. all
 *    arithmetic is done mod p.
 *
 * Params:
 *    uint64 v: a boolean vector
 *    uint64 r: the parameter r to evaluate chi_v with
 *    uint64 n: the dimension of this vector
 *
 * Returns:
 *    chi_v(r)
 */
uint64 chi(uint64 v, uint64* r, uint64 n)
{
    uint64 x=v;
    uint64 c = 1;
    for(uint64 i = 0; i <n; i++)
    {
        if( x&1 )
            c=myModMult(c, r[i]);
        else
            c=myModMult(c, 1+PRIME-r[i]);
        x=x>>1;
    }
    return c;
}

/*
 * evaluate_V_i:
 *    evaluates V_i polynomial at location r. Here V_i is described in GKR08;
 *    it is the multi-linear extension] of the vector of gate values at level i
 *    of the circuit
 *
 * Params:
 *    int mi: the dimensionality of k
 *    int ni: the number of vectors
 *    uint64* level_i: the contents of the vectors for this level
 *    uint64* r: the value of r to evaluate V_i(r) of.
 *
 */
uint64 evaluate_V_i(int mi, int ni, uint64* level_i, uint64* r)
{
    uint64 ans=0;
    for(uint64 k = 0; k < ni; k++)
        ans=myMod(ans + myModMult(level_i[k], chi(k, r, mi)));
    return ans;
}
