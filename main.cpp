#include "driver.h"

int main(int argc, char* argv[])
{
    printf("-------------------------------------------------\n");
#if defined __x86_64__ && !defined __ILP32__
    printf("  Build:  %s  %s, version x64\n", __DATE__, __TIME__);
#else
    printf("  Build:  %s  %s, version x32\n", __DATE__, __TIME__);
#endif
    printf("  GCC version:  %s\n", __VERSION__);
    printf("-------------------------------------------------\n");

    std::string fullBinpath = argv[0];
    printf("Bin path: %s", fullBinpath.c_str());

    initLogger(fullBinpath, "base_driver");

    Driver *pDevice;

    // if (pDevice->initLink())
    // {
    //     pDevice->startTask();
    // }
    // else
    // {
    //     printf("Can not connect to device\n");
    //     return 0;
    // }

    deinitLog();
    return 0;
}
