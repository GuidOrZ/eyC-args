#include <stdio.h>
#include <stdlib.h>

//Include the header
#include "../eyc_args.h"

int main (int argc, char *argv[]) {
    const char* pos1;
    int pos2;
    int help;
    const char* path;

    //Use this macro to define the arglist.
    DEFINE_ARGS(
        ARGS_POSITIONAL(DATATYPE_STRING, &pos1),
        ARGS_POSITIONAL(DATATYPE_INT, &pos2),
        ARGS_OPTION("-p", "-path", DATATYPE_STRING, &path),
        ARGS_FLAG("-h", "-help", &help)
    );

    parse_args(argc, argv, arglist);

    printf("POS1: %s\nPOS2: %d\nHELP: %d\nPATH: %s\n", pos1, pos2, help, path);

    return 0;
}