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
    DEFINE_ARGS(
        ARGS_POSITIONAL(DATATYPE_STRING, &pos1),
        ARGS_POSITIONAL(DATATYPE_FLOAT, &pos2),
        ARGS_OPTION("-p", NULL, DATATYPE_STRING, &option),
        ARGS_FLAG("-h", "--help", &flag1),
        ARGS_FLAG("-v", "--version", &flag2)
    );

    parse_args(argc, argv, arglist);

    printf("POS1: %s\nPOS2: %f\nHELP: %d\nVERSION: %d\nPATH: %s\n", pos1, pos2, flag1, flag2, option);

    return 0;
}
