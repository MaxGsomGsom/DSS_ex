#include <galois.cpp>

int Calculate_S0S1(byte *D, int nDrives) {

    if (nDrives<1) return -1;

    //calc first
    byte S0 = 0;
    for (int i=0; i<nDrives; i++) {
        S0 = GF_sum(S0, D[i]);
    }
    D[nDrives] = S0;

    //cals second
    byte S1 = 0;
    for (int i=0; i<nDrives; i++) {
        S1 = GF_sum(S1,
             GF_mul(D[i],
             GF_pow(X, nDrives-i-1)));
    }
    D[nDrives+1] = S1;

    return 0;
}
