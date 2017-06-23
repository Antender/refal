/*-------------- file -- XVV.C ------------*/
/*           MO: file input/output         */
/*      Last edition date : 02.09.90       */
/*-----------------------------------------*/
#include "../refal.h"
#include <stdio.h>
extern refalproc_t refal;

#define fmax 5

FILE* fopen();
FILE* inr;
FILE* inw;
static FILE *uniput[fmax], *uniget[fmax];
static int junp = 1;
static int jung = 1;
static long jl;

static void opng_()
{
    linkcb_t* p;
    char namf[40];
    register i;
    for(i = 0; i < 40; i++)
        namf[i] = '\0';
    p = refal.preva->next;
    if(p->tag != TAG_N)
        jl = 0; /* jung zamenila na jl */
    else {
        jl = p->info.coden;
        p = p->next;
    }
    if(jl >= fmax)
        goto HEOT;
    else
        jung = jl;
    for(i = 0; p != refal.nexta; i++)
        if((p->tag != TAG_O) || (i == 39))
            goto HEOT;
        else {
            namf[i] = p->info.infoc;
            p = p->next;
        }
    inr = fopen(namf, "r");
    /*printf("\n opn, namf=%s flg=%d fd=%d",namf,inr->flags,inr->fd);*/
    if(inr == NULL) {
        printf("\nopnget: can't open file %s", namf);
        goto HEOT1;
    }
    uniget[jung] = inr;
    return;
HEOT:
    printf("\nopnget: format error");
HEOT1:
    refal.upshot = 2;
    return;
}
static char opng_0[] = { 'O', 'P', 'N', 'G', 'E', 'T', '\006' };
G_L_B char opnget __asm__("ropnget") = '\122';
static void (*opng_1)() = opng_;

static void opnp_()
{
    linkcb_t* p;
    char namf[40];
    char* m;
    int i;

    for(i = 0; i < 40; i++)
        namf[i] = '\0';
    p = refal.preva->next;
    m = "w";
    if(p->tag == TAG_O && p->info.infoc == '*') {
        m = "a";
        p = p->next;
    }
    if(p->tag != TAG_N)
        jl = 0;
    else {
        jl = p->info.coden;
        p = p->next;
    }
    if(jl >= fmax)
        goto HEOT;
    junp = jl;
    for(i = 0; p != refal.nexta; i++)
        if((p->tag != TAG_O) || (i == 40))
            goto HEOT;
        else {
            namf[i] = p->info.infoc;
            p = p->next;
        }
    inw = fopen(namf, m);
    /*printf("\n opnput, namf=%s mode=%s",namf,m);*/
    if(inw == NULL) {
        printf("\nopnput: can't open file %s", namf);
        goto HEOT1;
    }
    uniput[junp] = inw;
    return;
HEOT:
    printf("\nopnput: format error");
HEOT1:
    refal.upshot = 2;
    return;
}
static char opnp_0[] = { 'R', 'O', 'P', 'N', 'P', 'U', 'T', '\006' };
G_L_B char opnput __asm__("ropnput") = '\122';
static void (*opnp_1)() = opnp_;

static void clsg_()
{
    linkcb_t* p;
    p = refal.preva->next;
    if(p->tag != TAG_N)
        jl = 0;
    else {
        jl = p->info.coden;
        p = p->next;
    }
    if(jl >= fmax)
        goto HEOT;
    else
        jung = jl;
    inr = uniget[jung];
    /*printf("\n cls, flg=%d fd=%d",inr->flags,inr->fd);*/
    fclose(inr);
    return;
HEOT:
    printf("\nclsget: format error");
    refal.upshot = 2;
    return;
}
static char clsg_0[] = { 'C', 'L', 'S', 'G', 'E', 'T', '\006' };
G_L_B char clsget __asm__("rclsget") = '\122';
static void (*clsg_1)() = clsg_;

static void clsp_()
{
    linkcb_t* p;
    p = refal.preva->next;
    if(p->tag != TAG_N)
        jl = 0;
    else {
        jl = p->info.coden;
        p = p->next;
    }
    if(jl >= fmax)
        goto HEOT;
    else
        junp = jl;
    inw = uniput[junp];
    /*printf("\n cls, flg=%d fd=%d",inw->flags,inw->fd);*/
    fclose(inw);
    return;
HEOT:
    printf("\nclsput: format error");
    refal.upshot = 2;
    return;
}
static char clsp_0[] = { 'R', 'C', 'L', 'S', 'P', 'U', 'T', '\006' };
G_L_B char clsput __asm__("rclsput") = '\122';
static void (*clsp_1)() = clsp_;

static void libg_()
{
    linkcb_t* p;
    char s[128];
    int i, j, c, new;
    p = refal.preva->next;
    new = 0;
    if(p->tag != TAG_N) {
        jl = 0;
        new = 1;
    } else {
        jl = p->info.coden;
        p = p->next;
    }
    if(jl >= fmax)
        goto HEOT;
    else
        jung = jl;
    inr = uniget[jung];
    s[0] = ' ';
    for(i = 0; ((c = getc(inr)) != '\n') && (c != EOF) && (i < 128); i++)
        s[i] = c;
    if(c == EOF)
        return;
    if(new == 1) { /* sovmestimost s ES */
        p = refal.prevr;
        if(slins(p, 80) == 0)
            return;
        for(j = 0; j < i; j++) {
            p = p->next;
            p->tag = TAG_O;
            p->info.codep = NULL;
            p->info.infoc = s[j];
        }
        for(j = i; j < 80; j++) {
            p = p->next;
            p->tag = TAG_O;
            p->info.codep = NULL;
            p->info.infoc = ' ';
        }
        return;
    }
    if(i < 128)
        i = i - 1;
    for(; i > 1; i--)
        if(s[i] != ' ')
            goto WYW;
WYW:
    j = i + 1;
    if(j > 128)
        j = 128;
    p = refal.prevr;
    for(i = 0; i < j; i++) {
        if(!slins(p, 1))
            return;
        p = p->next;
        p->info.codep = NULL;
        if(s[0] == EOF) {
            p->tag = TAG_N;
            return;
        }
        p->tag = TAG_O;
        p->info.infoc = s[i];
    }
    return;
HEOT:
    printf("\nlibget: format error");
    refal.upshot = 2;
    return;
}
static char libg_0[] = { 'L', 'I', 'B', 'G', 'E', 'T', '\006' };
G_L_B char libget __asm__("rlibget") = '\122';
static void (*libg_1)() = libg_;

static void libp_()
{
    linkcb_t* p;
    char s[81];
    int i, c;
    p = refal.preva->next;
    if(p->tag != TAG_N)
        jl = 0;
    else {
        jl = p->info.coden;
        p = p->next;
    }
    if(jl >= fmax)
        goto HEOT;
    else
        junp = jl;
    inw = uniput[junp];
    for(i = 0; p != refal.nexta; i++) {
        if((p->tag != TAG_O) && (p->tag != TAG_LB) && (p->tag != TAG_RB)) {
        HEOT:
            refal.upshot = 2;
            return;
        }
        if(i == 128) {
            c = '\n';
            putc(c, inw);
            i = 0;
        }
        switch(p->tag) {
        case TAG_LB:
            c = '(';
            break;
        case TAG_RB:
            c = ')';
            break;
        default:
            c = p->info.infoc;
        }
        p = p->next;
        putc(c, inw);
    }
    if(i != 0) {
        c = '\n';
        putc(c, inw);
    }
    return;
}
static char libp_0[] = { 'L', 'I', 'B', 'P', 'U', 'T', '\006' };
G_L_B char libput __asm__("rlibput") = '\122';
static void (*libp_1)() = libp_;

static void card_()
{
    linkcb_t* p;
    char c;
    if(refal.preva->next != refal.nexta) /* refal.upshot = 2; */
        rfpex("", refal.preva, refal.nexta);
    p = refal.prevr;
    while((c = getchar()) != '\n') {
        if(slins(p, 1) == 0)
            return;
        p = p->next;
        p->info.codep = NULL;
        if(c == EOF) {
            p->tag = TAG_N;
            return;
        }
        p->tag = TAG_O;
        p->info.infoc = c;
    }
    return;
}
static char card_0[] = { 'C', 'A', 'R', 'D', '\004' };
G_L_B char card __asm__("rcard") = '\122';
static void (*card_1)() = card_;

static void pr1_()
{
    rfpex("", refal.preva, refal.nexta);
    rftpl(refal.prevr, refal.preva, refal.nexta);
    return;
}
static char pr1_0[] = { 'P', 'R', 'I', 'N', 'T', '\005' };
G_L_B char print __asm__("rprint") = '\122';
static void (*pr1_1)() = pr1_;

static void pr2_()
{
    rfpexm("", refal.preva, refal.nexta);
    rftpl(refal.prevr, refal.preva, refal.nexta);
    return;
}
static char pr2_0[] = { 'P', 'R', 'I', 'N', 'T', 'M', '\006' };
G_L_B char printm __asm__("rprintm") = '\122';
static void (*pr2_1)() = pr2_;

static void pr3_()
{
    rfpex("", refal.preva, refal.nexta);
    return;
}
static char pr3_0[] = { 'P', 'R', 'O', 'U', 'T', '\005' };
G_L_B char prout __asm__("rprout") = '\122';
static void (*pr3_1)() = pr3_;

static void pr4_()
{
    rfpexm("", refal.preva, refal.nexta);
    return;
}
static char pr4_0[] = { 'P', 'R', 'O', 'U', 'T', 'M', '\006' };
G_L_B char proutm __asm__("rproutm") = '\122';
static void (*pr4_1)() = pr4_;

/*------------------ end of file  XVV.C ----------------*/
