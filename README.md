# eyC-args
A portable, lightweight, macro-based CLI argument parser for C programs.

## Features
Work in progress !
## Usage
Just include the file `"eyc_args.h"`, the whole implementation is there.

## Examples
```c
#include "eyc_args.h"

int main (int argc, char *argv[]) {
    // Initialize to the default value in case the arg is not provided
    const char *str    = "";
    const char *option = "";
    int flag           = 0;

    DEFINE_ARGS(
        ARGS_POSITIONAL(DATATYPE_STRING, &str),
        ARGS_OPTION("-p", NULL , DATATYPE_STRING, &option),
        ARGS_FLAG("-h", "--help", &flag)
    )
    // Flags can assign two int values: 
    // 1 : if the short flag is used   (e.g. "-v") 
    // 2 : if the long flag is used    (e.g. "--verbose")

    // Parse all arguments and convert them to their target types
    parse_args(argc, argv, arglist);

    return 0;
}
```
## Behavior (For Now)

Invalid conversions are not validated. Bad input may result in undefined behavior or truncation

```c
// If you declare those args
const char *pos1 = "";
int pos2         = 0;

DEFINE_ARGS(
    ARGS_POSITIONAL(DATATYPE_STRING, &pos1),
    ARGS_POSITIONAL(DATATYPE_INT, &pos2)
)

printf("Arg1: %s\nArg2: %d\n",pos1, pos2);
```

and you run: `./program arg1 arg2`.

Both arguments will be updated. If `arg2` is not a valid integer, `pos2` will be set to 0 (or a partial conversion).
For example:
- input: `123ab`
- output: `123`

So the output will be:
```
Arg1: arg1
Arg2: 0
```

See the [basic example](./examples/basic_main.c) for a better explanation.