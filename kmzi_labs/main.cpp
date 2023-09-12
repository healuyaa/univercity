#include <iostream>
#include "NTL/ZZ.h"

int main(int, char**){
    NTL::ZZ mod;
    NTL::ZZ g, secretNumber, n;

    NTL::conv(g, "1231312312313123213123131231");
    NTL::conv(n, "1231312312313123213123131231");
    NTL::conv(secretNumber, "123123121231231312312131231");

    NTL::LowLevelPowerMod(mod, g, secretNumber, n);
}
