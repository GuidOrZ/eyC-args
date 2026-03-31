#include <stdio.h>
#include <stdlib.h>

//Include the header
#include "../eyc_args.h"

int main (int argc, char *argv[]) {
    const char *pos1 = NULL;
    float pos2 = 0.0f;
    int help = 0;
    const char *path = "./";

    //Use this macro to define the arglist.
    DEFINE_ARGS(
        ARGS_POSITIONAL(DATATYPE_STRING, &pos1),
        ARGS_POSITIONAL(DATATYPE_FLOAT, &pos2),
        ARGS_OPTION("-p", NULL, DATATYPE_STRING, &path),
        ARGS_FLAG("-h", "--help", &help)
    );

    parse_args(argc, argv, arglist);

    printf("POS1: %s\nPOS2: %f\nHELP: %d\nPATH: %s\n", pos1, pos2, help, path);

    return 0;
}