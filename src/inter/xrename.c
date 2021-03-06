/*---------- file -- XRENAME.C ------------*/
/*           MO: file delete/rename        */
/*      Last edition date : 15.12.91       */
/*-----------------------------------------*/
#include "../refal.h"
#include <stdio.h>
extern refalproc_t refal;

static void unlnk_()
{
    linkcb_t* p;
    char namf[40];
    int i;
    p = refal.preva->next;
    for(i = 0; p != refal.nexta; i++) {
        if((p->tag != TAG_O) || (i >= 40))
            goto HEOT;
        else {
            namf[i] = p->info.infoc;
            p = p->next;
        }
    }
    namf[i] = '\0';
    /*   printf("\nUnlnk, namf=%s",namf);*/
    if(remove(namf) != 0) {
        printf("\nUnlnk: error");
        exit(1);
    } else
        return;
HEOT:
    printf("\nUnlnk: format error");
    refal.upshot = 2;
    return;
}
static char unlnk_0[] = { 'R', 'U', 'N', 'L', 'N', 'K', '\005' };
G_L_B char unlnk __asm__("runlnk") = '\122';
static void (*unlnk_1)() = unlnk_;

static void renam_()
{
    linkcb_t* p;
    char namf[40], namt[40]; /* from => to */
    int i;
    p = refal.preva->next;
    for(i = 0; p->tag != TAG_O || p->info.infoc != ' '; i++) {
        if((p->tag != TAG_O) || (i >= 40))
            goto HEOT;
        else {
            namf[i] = p->info.infoc;
            p = p->next;
        }
    }
    namf[i] = '\0';
    p = p->next;
    for(i = 0; p != refal.nexta; i++) {
        if((p->tag != TAG_O) || (i >= 40))
            goto HEOT;
        else {
            namt[i] = p->info.infoc;
            p = p->next;
        }
    }
    namt[i] = '\0';
    /*   printf("\nRenam: from=%s to=%s",namf,namt);*/
    if(rename(namf, namt) == -1) {
        printf("\nRenam: error");
        exit(1);
    } else
        return;
HEOT:
    printf("\nRenam: format error");
    refal.upshot = 2;
    return;
}
static char renam_0[] = { 'R', 'E', 'N', 'A', 'M', '\005' };
G_L_B char renam __asm__("rrenam") = '\122';
static void (*renam_1)() = renam_;

/*-------- end of file  XRENAME.C -------*/
