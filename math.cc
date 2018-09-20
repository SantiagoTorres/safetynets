#include "math.h"

//computes x^b
uint64 myPow(uint64 x, uint64 b)
{
    uint64 i = 1;
    for (int j = 0; j <b; j++)  i *= x;
    return i;
}

//efficient modular arithmetic function for p=2^61-1. Only works for this value of p.
//This function might
//return a number slightly greater than p (possibly by an additive factor of 8);
//It'd be cleaner to check if the answer is greater than p
//and if so subtract p, but I don't want to pay that
//efficiency hit here, so the user should just be aware of this.
uint64 myMod(uint64 x)
{
    return (x >> 61) + (x & PRIME);
}

//computes b^e mod p using repeated squaring. p should be 2^61-1
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

//Performs Extended Euclidean Algorithm
//Used for computing multiplicative inverses mod p
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


//Computes the modular multiplicative inverse of a modulo m,
//using the extended Euclidean algorithm
//only works for p=2^61-1
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

//computes chi_v(r), where chi is the Lagrange polynomial that takes
//boolean vector v to 1 and all other boolean vectors to 0. (we view v's bits as defining
//a boolean vector. n is dimension of this vector.
//all arithmetic done mod p
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

//evaluates V_i polynomial at location r.
//Here V_i is described in GKR08; it is the multi-linear extension
//of the vector of gate values at level i of the circuit
uint64 evaluate_V_i(int mi, int ni, uint64* level_i, uint64* r)
{
    uint64 ans=0;
    for(uint64 k = 0; k < ni; k++)
        ans=myMod(ans + myModMult(level_i[k], chi(k, r, mi)));
    return ans;
}
