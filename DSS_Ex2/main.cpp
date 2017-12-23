#include <iostream>
#include <raid6.cpp>

using namespace std;

int main()
{
    int nDrives = 14, nSyndromes = 2;
    byte D[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x1b};

    Calculate_S0S1(&(D[0]), nDrives, nSyndromes);
    byte S0=D[14], S1 = D[15];
    bool test1 = S0 == 0xff && S1 == 0x1b;

    int dnFailed = 2;
    Recovery1d_S0(&(D[0]), nDrives, nSyndromes, &dnFailed);
    bool test2 = D[dnFailed] == 0x33;

    Recovery1d_S1(&(D[0]), nDrives, nSyndromes, &dnFailed);
    bool test3 = D[dnFailed] == 0x33;

    cout << "tests: " <<
            test1 << " " <<
            test2 << " " <<
            test3 << " " << endl;
    return 0;
}

