#include <iostream>
#include <raid6.cpp>

using namespace std;

int main()
{
    int nDrives = 14, nSyndromes = 2;
    byte D[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x1b};
    int dFailedPos[2] = {2, 6};

    //Calc syndromes
    Calculate_S0S1(&D[0], nDrives, nSyndromes);
    bool test1 = D[14] == 0xff && D[15] == 0x1b;

    //Recovery 1 block with S0
    Recovery1d_S0(&D[0], nDrives, nSyndromes, &dFailedPos[0]);
    bool test2 = D[dFailedPos[0]] == 0x33;

    //Recovery 2 block with S0
    Recovery1d_S1(&D[0], nDrives, nSyndromes, &dFailedPos[0]);
    bool test3 = D[dFailedPos[0]] == 0x33;

    //Recovery 2 blocks
    Recovery2d_S0S1(&D[0], nDrives, nSyndromes, &dFailedPos[0]);
    bool test4 = D[dFailedPos[0]] == 0x33 && D[dFailedPos[1]] == 0x77;


    //Find 1 hidden fail
    //block 3 is failed, correct value is 0x2a
    byte D2[16] = {0x1e, 0x45, 0x6f, 0x2, 0x88, 0xef, 0x7b, 0x0, 0xbb, 0x67, 0x17, 0x8, 0x0f, 0x86, 0x48, 0x88};
    int dFailedPos2 = 0;
    SDC_Detection_S0S1(&D2[0], nDrives, nSyndromes, &dFailedPos2);
    bool test5 = dFailedPos2 == 3 && D2[3] == 0x2a;

    cout << "tests: " <<
            test1 << " " <<
            test2 << " " <<
            test3 << " " <<
            test4 << " " <<
            test5 << " " << endl;
    return 0;
}

