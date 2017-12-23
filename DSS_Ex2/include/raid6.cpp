#include <galois.cpp>

int Calculate_S0S1(byte *D, int nDrives, int nSyndromes) {

    if (nDrives<1 || nSyndromes!=2) return -1;

    //calc first
    byte S0 = 0;
    for (int i=0; i<nDrives; i++) {
        S0 = GF_sum(S0, D[i]);
    }
    D[nDrives] = S0;

    //calc second
    byte S1 = 0;
    for (int i=0; i<nDrives; i++) {
        //S1+=D[i]*x^i
        S1 = GF_sum(S1,
             GF_mul(D[i],
             GF_pow(_primElem, i)));
    }
    D[nDrives+1] = S1;

    return 0;
}


int Recovery1d_S0(byte *D, int nDrives, int nSyndromes, int *Failed) {

    if (nDrives<1 || nSyndromes!=2 || Failed[0] < 0 || Failed[0] >= nDrives) return -1;

    //Sum all data (except failed block) and syndrome 0
    byte dRecovered = 0;
    for (int i=0; i<(nDrives+1); i++) {
        if (i!=Failed[0])
            dRecovered = GF_sum(dRecovered, D[i]);
    }
    D[Failed[0]] = dRecovered;

    return 0;
}


int Recovery1d_S1(byte *D, int nDrives, int nSyndromes, int *Failed) {

    if (nDrives<1 || nSyndromes!=2 || Failed[0] < 0 || Failed[0] >= nDrives) return -1;

    D[Failed[0]] = 0b00;

    //calc second syndrome with failed block equals 0
    byte S1d = 0;
    for (int i=0; i<nDrives; i++) {
        //S1d+=D[i]*x^i
        S1d = GF_sum(S1d,
             GF_mul(D[i],
             GF_pow(_primElem, i)));
    }

    //calc failed block
    //dFailed = (S1+S1d)/x^dFiledPos
    D[Failed[0]] = GF_div(
                   GF_sum(D[nDrives+1], S1d),
                   GF_pow(_primElem, Failed[0]));

    return 0;
}

