#include <galois.cpp>


int Calculate_S0S1(byte *D, int nDrives, int nSyndromes) {

    if (D == nullptr || nDrives<1 || nSyndromes!=2) return -1;

    //Calc first syndrome
    byte S0 = 0;
    for (int i=0; i<nDrives; i++) {
        S0 = GF_sum(S0, D[i]);
    }
    D[nDrives] = S0;


    //Calc second syndrome
    byte S1 = 0;
    for (int i=0; i<nDrives; i++) {
        //S1+=D[i]*x^i
        S1 = GF_sum(S1,
             GF_mul(D[i],
             GF_pow(_primElem, i)));
    }
    D[nDrives+1] = S1;
}


int Recovery1d_S0(byte *D, int nDrives, int nSyndromes, int *Failed) {

    if (D == nullptr || nDrives<1 || nSyndromes!=2 ||
            Failed[0] < 0 || Failed[0] >= nDrives) return -1;

    D[Failed[0]] = 0b00;

    //Sum all data (except failed block, cause it's 0) and syndrome 0
    byte dRecovered = 0;
    for (int i=0; i<(nDrives+1); i++) {
            dRecovered = GF_sum(dRecovered, D[i]);
    }
    D[Failed[0]] = dRecovered;

    return 0;
}


int Recovery1d_S1(byte *D, int nDrives, int nSyndromes, int *Failed) {

    if (D == nullptr || nDrives<1 || nSyndromes!=2 ||
            Failed[0] < 0 || Failed[0] >= nDrives) return -1;

    D[Failed[0]] = 0b00;

    //Calc second syndrome with failed block equals 0
    byte S1d = 0;
    for (int i=0; i<nDrives; i++) {
        //S1d+=D[i]*x^i
        S1d = GF_sum(S1d,
             GF_mul(D[i],
             GF_pow(_primElem, i)));
    }

    //Calc failed block
    //dFailed = (S1+S1d)*x^(-dFiledPos)
    D[Failed[0]] = GF_mul(
                     GF_sum(D[nDrives+1], S1d),
                     GF_pow(
                       GF_inv(_primElem),
                       Failed[0]));

    return 0;
}


int Recovery2d_S0S1(byte *D, int nDrives, int nSyndromes, int *Failed) {

    if (D == nullptr || nDrives<1 || nSyndromes!=2 ||
            Failed[0] < 0 || Failed[0] >= nDrives ||
            Failed[1] < 0 || Failed[1] >= nDrives ||
            Failed[0] >= Failed[1]) //Index of first failed element must be less then second
        return -1;


    D[Failed[0]] = 0b00;
    D[Failed[1]] = 0b00;

    //Calc first syndrome with failed blocks equals 0
    byte S0dd = 0;
    for (int i=0; i<nDrives; i++) {
        S0dd = GF_sum(S0dd, D[i]);
    }

    //Calc second syndrome with failed blocks equals 0
    byte S1dd = 0;
    for (int i=0; i<nDrives; i++) {
        //S1dd+=D[i]*x^i
        S1dd = GF_sum(S1dd,
               GF_mul(D[i],
               GF_pow(_primElem, i)));
    }

    //Calc failed block 1
    // primElemInvPow = x^(d1FiledPos-d0FiledPos)
    byte primElemInvPow = GF_pow(
                             _primElem,
                             Failed[1]-Failed[0]);

    // sumS0S0dd = S0+S0dd
    byte sumS0S0dd = GF_sum(D[nDrives], S0dd);

    // d0Failed = (S1+S1dd)*x^(-d0FiledPos) + (S0+S0dd)*x^(d1FiledPos-d0FiledPos) /
    // (x^(d1FiledPos-d0FiledPos) + 1)
    D[Failed[0]] = GF_div(
                     GF_sum(
                       GF_mul(
                         GF_sum(D[nDrives+1], S1dd),
                         GF_pow(
                           GF_inv(_primElem),
                           Failed[0])),
                       GF_mul(
                         sumS0S0dd,
                         primElemInvPow)),
                     GF_sum(primElemInvPow, 1));


    //Calc failed block 2
    // d1Failed = (S0+S0dd)+d0;
    D[Failed[1]] = GF_sum(sumS0S0dd, D[Failed[0]]);

    return 0;
}


int SDC_Detection_S0S1(byte *D, int nDrives, int nSyndromes, int *Failed) {

    if (D == nullptr || nDrives<1 || nSyndromes!=2) return -1;

    //Calc first syndrome
    byte S0d = 0;
    for (int i=0; i<nDrives; i++) {
        S0d = GF_sum(S0d, D[i]);
    }
    //If correct then there is no fail blocks
    if (D[nDrives] == S0d) return 0;


    //Calc second syndrome without current block
    byte S1d = 0;
    for (int k=0; k<nDrives; k++) {
        //S1d+=D[i]*x^i
        S1d = GF_sum(S1d,
             GF_mul(D[k],
             GF_pow(_primElem, k)));
    }

    //Calc index of filed element
    // n = log((S0d+s0)*(s1d+s1)^(-1))
    byte n = GF_log(
               GF_mul(
                 GF_inv(
                   GF_sum(D[nDrives], S0d)),
                 GF_sum(D[nDrives+1], S1d)));

    //Calc element
    // d = S0d+s0+dFailed
    D[n] = GF_sum(GF_sum(D[nDrives], S0d), D[n]);
    Failed[0] = n;

    return 1;

}
