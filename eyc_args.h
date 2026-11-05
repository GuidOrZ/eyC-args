#ifndef EYC_ARGS_H
#define EYC_ARGS_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(EYC_PARSE_NO_GENERIC)
#   define EYC_PARSE_HAVE_GENERIC 1
#else
#   define EYC_PARSE_HAVE_GENERIC 0
#endif

#if EYC_PARSE_HAVE_GENERIC
#define EYC_DATATYPE_OF(t) _Generic((t),\
    int *:           EYC_DATATYPE_INT,\
    unsigned int *:  EYC_DATATYPE_UINT,\
    long *:          EYC_DATATYPE_LONG,\
    unsigned long *: EYC_DATATYPE_ULONG,\
    float *:         EYC_DATATYPE_FLOAT,\
    double *:        EYC_DATATYPE_DOUBLE,\
    const char **:   EYC_DATATYPE_STRING,\
    char **:         EYC_DATATYPE_STRING)

#define EYC_ARGS_POSITIONAL_AUTO(data) \
    {NULL, NULL, (void*)(data), EYC_ARGTYPE_POSITIONAL, EYC_DATATYPE_OF(data)}

#define EYC_ARGS_OPTION_AUTO(short_f, long_f, data) \
    {(short_f), (long_f), (void*)(data), EYC_ARGTYPE_OPTION, EYC_DATATYPE_OF(data)}
#endif

#define EYC_ARGS_END {NULL, NULL, NULL, EYC_ARGTYPE_POSITIONAL, EYC_DATATYPE_END}

#define EYC_ARGS_POSITIONAL(type, data) \
    {NULL, NULL, (void*)(data), EYC_ARGTYPE_POSITIONAL, (type)}

#define EYC_ARGS_OPTION(short_f, long_f, type, data) \
    {(short_f), (long_f), (void*)(data), EYC_ARGTYPE_OPTION, (type)}

#define EYC_ARGS_FLAG(short_f, long_f, data) \
    {(short_f), (long_f), (void*)(data), EYC_ARGTYPE_FLAG, EYC_DATATYPE_INT}

#define EYC_DEFINE_ARGS(...) \
    eyc_args_type eyc_arglist[] = { __VA_ARGS__, EYC_ARGS_END }

typedef enum {
    EYC_FLAG_NONE,
    EYC_FLAG_SHORT,
    EYC_FLAG_LONG
} eyc_flag_values;

typedef enum {
    EYC_DATATYPE_INT,
    EYC_DATATYPE_UINT,
    EYC_DATATYPE_LONG,
    EYC_DATATYPE_ULONG,
    EYC_DATATYPE_FLOAT,
    EYC_DATATYPE_DOUBLE,
    EYC_DATATYPE_STRING,
    EYC_DATATYPE_END
} eyc_data_types;

typedef enum {
    EYC_ARGTYPE_POSITIONAL,
    EYC_ARGTYPE_FLAG,
    EYC_ARGTYPE_OPTION
} eyc_arg_types;

typedef struct {
    const char *flag;           //SHORT FLAG                            "-v"
    const char *verbose_flag;   //VERBOSE FLAG                          "--verbose"
    void* data;                 //VOID POINTER TO THE DATA
    eyc_arg_types arg_type;     //TYPE OF ARG
    eyc_data_types data_type;   //ACTUAL TYPE OF THE DATA
} eyc_args_type;

static inline void eyc_priv_assign_value(eyc_args_type *arg, const char *str) {
    switch (arg->data_type)
    {
        case EYC_DATATYPE_INT:    *(int *)arg->data           = atoi(str);                              break;
        case EYC_DATATYPE_FLOAT:  *(float *)arg->data         = (float)atof(str);                       break;
        case EYC_DATATYPE_STRING: *(const char **)arg->data   = str;                                    break;
        case EYC_DATATYPE_UINT:   *(unsigned int *)arg->data  = (unsigned int)strtoul(str, NULL, 10);   break;
        case EYC_DATATYPE_LONG:   *(long *)arg->data          = strtol(str, NULL, 10);                  break;
        case EYC_DATATYPE_ULONG:  *(unsigned long *)arg->data = strtoul(str, NULL, 10);                 break;
        case EYC_DATATYPE_DOUBLE: *(double *)arg->data        = strtod(str, NULL);                      break;
        case EYC_DATATYPE_END:                                                                          break;
    }
}

static inline int eyc_priv_assign_flag(eyc_args_type *arg, const char *str) {
    if (arg->flag && strcmp(arg->flag, str) == 0) {
        *(int *)arg->data = EYC_FLAG_SHORT;
        return 1;
    }
    if (arg->verbose_flag && strcmp(arg->verbose_flag, str) == 0) {
        *(int *)arg->data = EYC_FLAG_LONG;
        return 1;
    }

    return 0;
}

static inline int eyc_priv_is_flag(const char *str) {
    if (str[0] != '-' || str[1] == '\0') return 0;
    if (isdigit((unsigned char)str[1]) || str[1] == '.') {
        return 0;
    }
    return 1;
}

static inline int eyc_parse_args(int argc, char *argv[], eyc_args_type arglist[]) {
    if (argv == NULL || arglist == NULL) { return -1; }

    int index = 0;
    for (int i = 1; i < argc; i++) {
        if (eyc_priv_is_flag(argv[i])) {
                //STACKED FLAGS
            if (argv[i][0] == '-' && argv[i][1] != '-' && strlen(argv[i]) > 2) {
                for (int j = 1; argv[i][j] != '\0'; j++){
                    char short_flag[3] = {
                       '-',
                       argv[i][j],
                       '\0'
                    };
                    for (int k = 0; arglist[k].data_type != EYC_DATATYPE_END; k++) {
                        if (arglist[k].arg_type == EYC_ARGTYPE_FLAG) {
                            if (eyc_priv_assign_flag(&arglist[k], short_flag)) {
                                break;
                            }
                        }
                    }
                }
                continue;
            }

            for (int j = 0; arglist[j].data_type != EYC_DATATYPE_END; j++) {
                //SINGLE FLAG
                if (arglist[j].arg_type == EYC_ARGTYPE_FLAG) {
                    if (eyc_priv_assign_flag(&arglist[j], argv[i])) {
                        break;
                    }
                }
                //OPTION
                if (arglist[j].arg_type == EYC_ARGTYPE_OPTION) {
                    if ((arglist[j].flag && strcmp(arglist[j].flag, argv[i]) == 0) ||
                        (arglist[j].verbose_flag && strcmp(arglist[j].verbose_flag, argv[i]) == 0)) {
                        if (i + 1 < argc && !eyc_priv_is_flag(argv[i+1])) {
                            eyc_priv_assign_value(&arglist[j], argv[i+1]);
                            i++;
                        }
                        break;
                    }
                }
            }
        }
        else {  //POSITIONAL
            for (; arglist[index].data_type != EYC_DATATYPE_END; index++) {
                if (arglist[index].arg_type == EYC_ARGTYPE_POSITIONAL) {
                    eyc_priv_assign_value(&arglist[index], argv[i]);
                    index++;
                    break;
                }
            }
        }
    }
    return 0;
}

#endif
