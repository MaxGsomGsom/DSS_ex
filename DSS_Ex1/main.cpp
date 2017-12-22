#include <iostream>

using namespace std;

typedef unsigned char byte;

int prim_poly = 0x11d;


inline byte GF_mulX(byte a) {
    //shift on 1 bit to left, because X rises deegre by 1
    //then XOR with prim_poly
    return (a << 1) ^ prim_poly;
}

inline byte GF_mul(byte a, byte b) {

    byte sum = 0;

    //if a and contains ones bits
    //for every bit of A
    while (a && b) {

        //if current bit of B = 1, add A to sum (with XOR)
        if (b & 1)
            sum ^= a;

        //shift B to next bit
        b >>= 1;

        //if A will overflow, then substract prim_poly (with XOR)
        if (a & 0b10000000)
            a = (a << 1) ^ prim_poly;
        //else simple shift to left
        else
            a <<= 1;
    }

    return sum;
}




int main()
{
    cout << (int)GF_mul(0b11101, 0b1000001) << endl;
    return 0;
}

