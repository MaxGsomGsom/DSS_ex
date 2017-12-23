#include <iostream>
#include <galois.cpp>

using namespace std;

typedef unsigned char byte;


int main()
{
    byte a = 0b11101;
    byte b = 0b1000001;

    byte wtf = GF_log(b);

    bool testMul = GF_mul(a, b) == GF_mul2(a, b);
    bool testDiv = GF_div(a, b) == GF_div2(a, b);
    bool testPow = GF_pow(a, 10) == GF_pow2(a, 10);
    bool testInv = GF_inv(a) == GF_inv2(a);
    bool testLog = GF_log(a) == GF_log2(a);

    bool test1 = (a == GF_pow(_primElem, GF_log(a)));
    bool test2 = (GF_log(GF_mul(a, b)) == GF_log(a) + GF_log(b));
    bool test3 = (GF_mul(a, GF_inv(a)) == 1);

    cout << "tests: " <<
            testMul << " " <<
            testDiv << " " <<
            testPow << " " <<
            testInv << " " <<
            testLog << " " <<
            test1 << " " <<
            test2 << " " <<
            test3 << " " << endl;

    cout << "primitives_count=" << (int)GF_find_prim_elems() << endl;

    return 0;
}

