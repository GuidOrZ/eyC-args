#include <stdio.h>

//Include the header
#include "../eyc_args.h"

int main (int argc, char *argv[]) {

    const char *pos1 = NULL;
    float pos2 = 0.0f;
    int flag1 = 0;
    int flag2 = 0;
    const char *option = "./";

    //Use this macro to define the arglist.
    EYC_DEFINE_ARGS(
        EYC_ARGS_POSITIONAL(EYC_DATATYPE_STRING, &pos1),
        EYC_ARGS_POSITIONAL(EYC_DATATYPE_FLOAT, &pos2),
        EYC_ARGS_OPTION("-p", NULL, EYC_DATATYPE_STRING, &option),
        EYC_ARGS_FLAG("-h", "--help", &flag1),
        EYC_ARGS_FLAG("-v", "--version", &flag2)
    );

    eyc_parse_args(argc, argv, eyc_arglist);

    printf("POS1: %s\nPOS2: %f\nHELP: %d\nVERSION: %d\nPATH: %s\n", pos1, pos2, flag1, flag2, option);

    return 0;
}
