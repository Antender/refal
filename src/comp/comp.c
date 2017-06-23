#include "../refal.h"



int _eoj; /* "sysin" end flag */ /* kras */
int cdnumb; /* card number */    /* kras */

typedef struct parser_flags_t {
    bool was_72 : 1;
    bool uzhe_krt : 1;
    bool was_err : 1;
    bool uzhe_zgl : 1;
    bool uzhekrt_t : 1;
} parser_flags_t;

parser_flags_t flags;

char parm[40];
#include "main.c"

void log(char* message)
{
    if(options.source == 1) {
        fputs(message, sysprint);
    }
    fputs(message, systerm);
}

char card[81]; /* card buffer (input) */
char* card72 = card;

void pchk()
{ /* writing of card into sysprint */
    int i;
    char tmpstr[80];
    if(flags.uzhe_krt == 0 && sysprint != NULL) {
        flags.uzhe_krt = 1;
        card[72] = '\0';
        if(_eoj == 0) {
            sprintf(tmpstr, "%4d %s", cdnumb, card);
            for(i = 76; i > 4; i--)
                if(tmpstr[i] != ' ')
                    break;
            i++;
            tmpstr[i] = '\n';
            i++;
            tmpstr[i] = '\0';
            fputs(tmpstr, sysprint);
        }
    }
}
void pchk_t()
{ /* card writing into systerm */
    char tmpstr[80];
    if(flags.uzhekrt_t == 0) {
        flags.uzhekrt_t = 1;
        card[72] = '\0';
        if(_eoj == 0) {
            sprintf(tmpstr, "%4d %s\n", cdnumb, card);
            fputs(tmpstr, systerm);
        }
    }
}

#include "cerr.c"
#include "refal.c"