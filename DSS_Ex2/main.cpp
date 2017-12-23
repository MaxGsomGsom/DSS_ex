#include <iostream>
#include <raid6.cpp>

using namespace std;

int main()
{
    int nDrives = 14, nSyndromes = 2;
    byte D_test[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x88};

    Calculate_S0S1(&(D[0]), nDrives);
    byte S0=D_test[14], S1 = D_test[15];
    bool test1 = S0 == 0xff && S1 == 0x88;

    cout << "tests: " <<
            test1 << " " << endl;
    return 0;
}

