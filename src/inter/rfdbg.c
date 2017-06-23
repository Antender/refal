/*------------ file -- RFDBG.C -------------*/
/*          Standard REFAL-Debugger         */
/*        Last modification : 14.09.91      */
/*------------------------------------------*/
#include "../refal.h"

static st s_st;
extern char rgo();

void main()
{
    rfinit();
    if(!lincrm())
        goto LACK;
    if(!lcre(&s_st))
        goto LACK;
    if(!linskd(&s_st, rgo))
        goto LACK;
    rfdbg(&s_st); /* there is exit */
LACK:
    printf("\nREFAL-debugger: no memory for initialization");
    fclose(stdin);
    rftermm();
    exit(1);
}
/*---------  end of file RFDBG.C -----------*/
