#include <string.h>
#include <stdlib.h>

#ifndef PARSE_H
#define PARSE_H

/* MACROS FOR THE LIST DEFINITION */

#define ARGS_END {NULL, NULL, NULL, ARGTYPE_POSITIONAL, DATATYPE_END}

#define ARGS_POSITIONAL(type, data) \
    {NULL, NULL, (void*)(data), ARGTYPE_POSITIONAL, (type)}

#define ARGS_OPTION(short_f, long_f, type, data) \
    {(short_f), (long_f), (void*)(data), ARGTYPE_OPTION, (type)}

#define ARGS_FLAG(short_f, long_f, data) \
    {(short_f), (long_f), (void*)(data), ARGTYPE_FLAG, DATATYPE_INT}

#define DEFINE_ARGS(...) \
    args_type arglist[] = { __VA_ARGS__, ARGS_END }

/* DATA STRUCTURES */

typedef enum {
    DATATYPE_INT,
    DATATYPE_FLOAT,
    DATATYPE_STRING,
    DATATYPE_END
}data_types;

typedef enum {
    ARGTYPE_POSITIONAL,
    ARGTYPE_FLAG,
    ARGTYPE_OPTION
}arg_types;

typedef struct {
    const char *flag;           //SHORT FLAG                            "-v"
    const char *verbose_flag;   //VERBOSE FLAG                          "--verbose"
    void* data;                 //VOID POINTER TO THE DATA
    arg_types arg_type;         //TYPE OF ARG
    data_types data_type;       //ACTUAL TYPE OF THE DATA
}args_type;

/* FUNCTIONS */

void _init_value (args_type arglist []) {
    for (int i = 0; arglist[i].data_type != DATATYPE_END; i++) {
        switch (arglist[i].data_type)
        {
            case DATATYPE_INT:    {int *r = (int*)arglist[i].data;      *r = 0;     break; }
            case DATATYPE_FLOAT:  {float *r = (float*)arglist[i].data;  *r = 0.0f;  break; }
            case DATATYPE_STRING: {char **r = (char**)arglist[i].data;  *r = NULL;  break; }
            case DATATYPE_END:                                                                      break;
        }
    }
}

void _assign_value (args_type *arg, const char *str) {
    switch (arg->data_type)
    {        
        case DATATYPE_INT:    *(int *)arg->data         = atoi(str);                break;
        case DATATYPE_FLOAT:  *(float *)arg->data       = (float)atof(str);         break;
        case DATATYPE_STRING: *(const char **)arg->data = str;                      break;
        case DATATYPE_END:                                                          break;
    }
}

void _assigning_flag(args_type *arg, const char *str) {
    if (arg->flag && strcmp(arg->flag, str) == 0) {
        *(int *)arg->data = 1;
    }
    else if (arg->verbose_flag && strcmp(arg->verbose_flag, str) == 0) {
        *(int *)arg->data = 2;
    }
}

int parse_args(int argc, char *argv[], args_type arglist []) {
    if (argv == NULL || arglist == NULL) { return -1; }

    _init_value(arglist);

    int index = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 0; arglist[j].data_type != DATATYPE_END; j++) {
                //FLAG
                if (arglist[j].arg_type == ARGTYPE_FLAG) {
                    _assigning_flag(&arglist[j], argv[i]);
                }
                //OPTION
                if (arglist[j].arg_type == ARGTYPE_OPTION) {
                    if ((arglist[j].flag && strcmp(arglist[j].flag, argv[i]) == 0) ||
                        (arglist[j].verbose_flag && strcmp(arglist[j].verbose_flag, argv[i]) == 0)) {
                        if (i + 1 < argc && argv[i+1][0] != '-') {
                            _assign_value(&arglist[j], argv[i+1]);
                            i++;
                        }
                    }
                }
            }
        }
        else {  //POSITIONAL
            for (; arglist[index].data_type != DATATYPE_END; index ++) {
                if (arglist[index].arg_type == ARGTYPE_POSITIONAL) {
                    _assign_value(&arglist[index], argv[i]);
                    index++;
                    break;
                }
            } 
        }
    }
    return 0;
}

#endif