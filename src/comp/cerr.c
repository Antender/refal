/*-------------  file  --  CERR.C  ---------------*/
/*               Print error file                 */
/*        Last edition date : 14.07.89            */
/*------------------------------------------------*/
#include <stdio.h>
#include "../refal.def"

extern void oshibka();
extern FILE *sysprint, *systerm;

void pchosh(s) char* s;
{
    char tmp[255];
    oshibka();
    sprintf(tmp, "***** %s\n", s);
    if(sysprint != NULL)
        fputs(tmp, sysprint);
    fputs(tmp, systerm);
}

void pchosi(s, t) char *s, *t;
{
    char tmp[255];
    oshibka();
    sprintf(tmp, "***** %s %s\n", s, t);
    if(sysprint != NULL)
        fputs(tmp, sysprint);
    fputs(tmp, systerm);
}

void pchosj(s, sid, lsid, s1) char *s, *sid, *s1;
int lsid;
{
    char tmp[255];
    char tmp1[255];
    int i;
    oshibka();
    for(i = 0; i < lsid; i++)
        tmp1[i] = *(sid + i);
    tmp1[lsid] = '\0';
    sprintf(tmp, "***** %s %s %s\n", s, tmp1, s1);
    if(sysprint != NULL)
        fputs(tmp, sysprint);
    fputs(tmp, systerm);
}

void pchosx(s, sid, lsid, s1) char *s, *sid, *s1;
int lsid;
{
    char tmp[255];
    char tmp1[255];
    int i;
    oshibka();
    for(i = 0; i < lsid; i++)
        tmp1[i] = *(sid + i);
    tmp1[lsid] = '\0';
    sprintf(tmp, "***** %s %s %s\n", s, tmp1, s1);
    if(sysprint != NULL)
        fputs(tmp, sysprint);
    fputs(tmp, systerm);
}

void pchosa(s, c) char *s, c;
{
    char tmp[255];
    oshibka();
    sprintf(tmp, "***** %s %c\n", s, c);
    if(sysprint != NULL)
        fputs(tmp, sysprint);
    fputs(tmp, systerm);
}

void pchose(s, t, lt) char *s, *t;
int lt;
{
    char tmp[255];
    char tmp1[255];
    int i;
    oshibka();
    for(i = 0; i < lt; i++)
        tmp1[i] = *(t + i);
    tmp1[lt] = '\0';
    sprintf(tmp, "***** %s %s\n", s, tmp1);
    if(sysprint != NULL)
        fputs(tmp, sysprint);
    fputs(tmp, systerm);
}
/*--------  end  of  file  CERR.C  ---------*/
