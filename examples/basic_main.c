#include <stdio.h>

//Include the header
#include "../eyc_args.h"

int main (int argc, char *argv[]) {
    int pos1;
    int pos2;
    int help;

    //Use this macro to define the arglist.
    DEFINE_ARGS(
        ARGS_POSITIONAL(DATATYPE_INT, &pos1),
        ARGS_POSITIONAL(DATATYPE_INT, &pos2),
        ARGS_FLAG("-h", "-help", &help)
    );

    parse_args(argc, argv, arglist);

    printf("%d\n%d\n%d\n", pos1, pos2, help);

    return 0;
}