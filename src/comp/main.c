#include "../optparse.h"

typedef struct options_t {
    bool source : 1;
    bool extname : 1;
    bool multmod : 1;
    bool names : 1;
} options_t;
options_t options;

FILE* sysprint;
FILE* syslin;
FILE* sysin;
FILE* systxt; /* for module names */

/* BLF */
char vers_i[] = "Refal-2  version 0.2.3-20050529 (c) Refal-2 Team";

extern int main_loop();
extern unsigned jwhere();

char parm_i[40];
short nommod;
long mod_length;

int main(int argc, char* argv[])
{
    int i, j, temp;

    /*
       printf ("qindex =%d\n", qindex);
    */

    nommod = 0;
    printf("\n"); /* BLF */
    printf("%s", vers_i);
    /* BLF  de();  ---------------------------------*/
    if(argc < 2) {
        /* BLF      printf("\nSer. No %s",regnom);      */
        printf("\n"); /* BLF */
        printf("\nSyntax: refal source_file [(option,...,option)]");
        printf("\nOptions:");
        printf("\n   mm  multi_module");
        printf("\n   nn  no_function_names");
        printf("\n   ns  no_source_listing");
        printf("\n   fn  full_names");
        printf("\n\n"); /* BLF */
        exit(1);
    };

    for(i = 0; (parm[i] = *(argv[1] + i)) != '\0'; i++)
        ;

    if(strchr(parm, '.') == NULL)
        strcat(parm, ".ref");

    printf("\n%s:\n", parm);
    for(i = 0; i <= (strlen(parm) + 1); i++)
        parm_i[i] = parm[i];
    sysin = fopen(parm, "r");
    if(sysin == NULL) {
        printf("Can't open %s\n", parm);
        exit(1);
    };
    sysprint = NULL;

    options.source = 1;
    options.extname = 0;
    options.multmod = 0;
    options.names = 1;
    for(j = 2; j < argc; ++j) {
        for(i = 0; (parm[i] = *(argv[j] + i)) != '\0'; i++)
            ;
        if(parm[0] == '(') {
            for(i = 1; (i < 40) && (parm[i] != ')') && (parm[i] != '\0');) {
                if(strncmp((parm + i), "nn", 2) == 0) /*  kras */
                    options.names = 0;
                else if(strncmp((parm + i), "ns", 2) == 0)
                    options.source = 0;
                else if(strncmp((parm + i), "fn", 2) == 0)
                    options.extname = 1;
                else if(*(parm + i) == 'm')
                    options.multmod = 1;
                else {
                    for(temp = 0; *(parm + temp) != '\0'; temp++)
                        ;
                    temp--;
                    if(*(parm + temp) == ')')
                        *(parm + temp) = '\0';
                    printf("Unknown option: %s\n", (parm + i));
                    printf("Options may be: ns,nn,mm,fn\n");
                    exit(1);
                }
                temp = i;
                char* pos = strchr((parm + i), ',') + 1;
                if((parm + i) == pos) {
                    pos = strchr((parm + temp), ')');
                    if(pos == NULL) {
                        printf("Missing ')' in option definition\n");
                        exit(1);
                    }
                }
                i += temp;
            } /* end for */
            for(i = 0; (parm[i] = *(argv[1] + i)) != '\0'; ++i)
                ;
        } /* end if */
        else {
            printf("Illegal options definition: %s\n", parm);
            exit(1);
        }
    } /* end for */
    for(i = 0; ((parm[i] = *(argv[1] + i)) != '\0') && (parm[i] != '.'); ++i)
        ;
    parm[i] = '\0';
    if(options.source == 1) {
        strcat(parm, ".lst");
        if((sysprint = fopen(parm, "w")) == NULL) {
            printf("Can't open %s\n", parm);
            exit(8);
        }
    }
    for(i = 0; ((parm[i] = *(argv[1] + i)) != '\0') && (parm[i] != '.'); ++i)
        ;
    parm[i] = '\0';
    if(options.multmod == 1) {
        char tmp[256];
        strcpy(tmp, parm);
        strcat(tmp, ".txt");
        systxt = fopen(tmp, "w");
        if(systxt == NULL) {
            printf("Can't open %s\n", parm);
            exit(8);
        }
    }
    int return_code = main_loop();
    if(return_code != 0) {
        return return_code;
    }
    fclose(sysin);
    if(sysprint != NULL) {
        fclose(sysprint);
    }
    if(options.multmod == 0) {
        mod_length = jwhere();
        fclose(syslin);
        if((mod_length == 0) || (flags.was_err != 0))
            remove(parm);
    }
    if(flags.was_err != 0) {
        if(options.multmod == 1)
            remove(parm);
        exit(1);
    } else {
        if(nommod <= 1 && options.multmod == 1)
            remove(parm); /* for multimod. */
        exit(0);
    }
}