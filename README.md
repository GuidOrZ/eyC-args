# eyC-args
A portable, lightweight, macro-based CLI argument parser for C programs.

**WIP**

## Features
- Single header, no dependencies beyond the C standard library
- Positional arguments, flags (`-v` / `--verbose`), and options with values (`-n 5`)
- Stacked short flags (`-abc`)
- Optional type deduction with `EYC_ARGS_*_AUTO` (C11 `_Generic`;
  define `EYC_PARSE_NO_GENERIC` to disable)

## Limitations (WIP)
- No `--` terminator, no `--opt=value`, no generated `--help`
- No conversion error reporting
- Unknown flags are ignored

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

    EYC_DEFINE_ARGS(
        EYC_ARGS_POSITIONAL(EYC_DATATYPE_STRING, &str),
        EYC_ARGS_OPTION("-p", NULL , EYC_DATATYPE_STRING, &option),
        EYC_ARGS_FLAG("-h", "--help", &flag)
    );
    // Flags can assign two int values:
    // EYC_FLAG_SHORT (1): if the short flag is used   (e.g. "-v") 
    // EYC_FLAG_LONG (2) : if the long flag is used    (e.g. "--verbose")

    // Parse all arguments and convert them to their target types
    eyc_parse_args(argc, argv, eyc_arglist);

    return 0;
}
```
## Behavior (For Now)

No error reporting: invalid input yields 0 or a partial conversion.

```c
// If you declare those args
const char *pos1 = "";
int pos2         = 0;

EYC_DEFINE_ARGS(
    EYC_ARGS_POSITIONAL(EYC_DATATYPE_STRING, &pos1),
    EYC_ARGS_POSITIONAL(EYC_DATATYPE_INT, &pos2)
);

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
