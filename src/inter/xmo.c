/*-------------- file -- XMO.C -------------*/
/*                 General MO:              */
/*     p1, m1, numb, symb, first, last,     */
/*     lengr, lengw, multe, crel, delf      */
/*      Last edition date : 02.09.90        */
/*------------------------------------------*/
#include <stdio.h>
#include "../refal.h"
extern refalproc_t refal;

static void p1_()
{
    linkcb_t* p;
    long l;
    p = refal.preva->next;
    if((p->next != refal.nexta) || (p->tag != TAG_N)) {
    NEOT:
        refal.upshot = 2;
        return;
    }
    l = gcoden(p) + 1;
    if(l > 16777215l)
        goto NEOT;
    pcoden(p, l);
    rftpl(refal.prevr, p->prev, p->next);
    return;
}
static char p1_0[] = { 'P', '1', '\002' };
G_L_B char p1 asm("rp1") = '\122';
static void (*p1_1)() = p1_;

static void m1_()
{
    linkcb_t* p;
    long l;
    p = refal.preva->next;
    if((p->next != refal.nexta) || (p->tag != TAG_N)) {
    NEOT:
        refal.upshot = 2;
        return;
    }
    l = gcoden(p) - 1;
    if(l < 0)
        goto NEOT;
    pcoden(p, l);
    rftpl(refal.prevr, p->prev, p->next);
    return;
}
static char m1_0[] = { 'M', '1', '\002' };
G_L_B char m1 asm("rm1") = '\122';
static void (*m1_1)() = m1_;

long atol();

static void numb_()
{
    linkcb_t *p, *pz, *p1;
    char str[12], zn;
    register i;
    long l;
    p = refal.preva->next;
    zn = p->info.infoc;
    pz = p;
    if((p->tag == TAG_O) && ((zn == '-') || (zn == '+'))) {
        p = p->next;
        if(zn == '+')
            pz = p;
    }
    p1 = p;
    while((p->tag == TAG_O) && (p->info.infoc == '0'))
        p = p->next;
    for(i = 0; p != refal.nexta; i++) {
        if((p->tag != TAG_O) || (i == 11)) {
        NEOT:
            refal.upshot = 2;
            return;
        } else {
            str[i] = p->info.infoc;
            if(str[i] < '0' || str[i] > '9')
                goto NEOT;
            if((i == 9) && (strcmp(str, "2147483647") > 0))
                goto NEOT;
            p = p->next;
        }
    }
    str[i] = '\0';
    if(strlen(str) == 0) {
        if(!slins(refal.nexta->prev, 1))
            return;
        p1 = refal.nexta->prev;
        pz = p1;
    }
    l = atol(str);
    p1->tag = TAG_N;
    if(l > 16777215l) {
        pcoden(p1, l >> 24);
        p1 = p1->next;
        p1->tag = TAG_N;
        l = l & 0xffffffL;
    }
    pcoden(p1, l);
    rftpl(refal.prevr, pz->prev, p1->next);
    return;
}
static char numb_0[] = { 'N', 'U', 'M', 'B', '\004' };
G_L_B char numb asm("rnumb") = '\122';
static void (*numb_1)() = numb_;

static void symb_()
{
    linkcb_t *p, *pz, *p1;
    char str[12], zn;
    register i;
    long l;
    int j;
    p = refal.preva->next;
    zn = p->info.infoc;
    pz = p;
    if((p->tag == TAG_O) && ((zn == '-') || (zn == '+'))) {
        p = p->next;
        if(zn == '+')
            pz = p;
    }
    p1 = p;
    while((p->tag == TAG_N) && (gcoden(p) == 0l))
        p = p->next;
    for(i = 0; p != refal.nexta; i++, p = p->next)
        if((p->tag != TAG_N) || (i == 2))
            goto NEOT;
    p = p->prev;
    if((i == 2) && (gcoden(p1) >= 128)) {
    NEOT:
        refal.upshot = 2;
        return;
    }
    l = gcoden(p);
    if(i == 2)
        l = l + 16777216l * gcoden(p1);
    if((i == 0) || (l == 0l)) {
        pz = p1;
        l = 0l;
    }
    sprintf(str, "%ld", l);
    j = strlen(str);
    if(!lrqlk(j))
        if(!lincrm()) {
            refal.upshot = 3;
            return;
        }
    if(pz != refal.nexta)
        lins(p1, j);
    else {
        pz = pz->prev;
        lins(pz, j);
        pz = pz->next;
        p1 = pz;
    }
    for(i = 0, p = p1; i < j; i++, p = p->next) {
        p->tag = TAG_O;
        p->info.codep = NULL;
        p->info.infoc = str[i];
    }
    rftpl(refal.prevr, pz->prev, p);
    return;
}
static char symb_0[] = { 'S', 'Y', 'M', 'B', '\004' };
G_L_B char symb asm("rsymb") = '\122';
static void (*symb_1)() = symb_;

static void first_()
{
    linkcb_t *p, *pn;
    int k;
    long n; /*eg*/
    pn = refal.preva->next;
    if((pn == refal.nexta) || (pn->tag != TAG_N)) {
        refal.upshot = 2;
        return;
    }; /* FAIL */
    n = gcoden(pn);
    p = pn;
    for(k = 1; k <= n; k++) {
        p = p->next;
        if(p == refal.nexta) {
            pn->info.codep = NULL;
            pn->info.infoc = '*';
            pn->tag = TAG_O;
            rftpl(refal.prevr, refal.preva, refal.nexta);
            return;
        }
        if(p->tag == TAG_LB)
            p = p->info.codep;
    }
    p = p->next;
    refal.preva->tag = TAG_LB;
    refal.preva->info.codep = pn;
    pn->tag = TAG_RB;
    pn->info.codep = refal.preva;
    rftpl(refal.preva, pn, p);
    rftpl(refal.prevr, refal.nextr, refal.nexta);
    return;
}
static char first_0[] = { 'F', 'I', 'R', 'S', 'T', '\005' };
G_L_B char first asm("rfirst") = '\122';
static void (*first_1)() = first_;

static void last_()
{
    linkcb_t *pn, *p;
    int k;
    long n; /*eg*/
    pn = refal.preva->next;
    if((pn == refal.nexta) || (pn->tag != TAG_N)) {
        refal.upshot = 2;
        return;
    }; /* FAIL */
    n = gcoden(pn);
    p = refal.nexta;
    for(k = 1; k <= n; k++) {
        p = p->prev;
        if(p == pn) {
            pn->tag = TAG_O;
            pn->info.codep = NULL;
            pn->info.infoc = '*';
            rftpl(refal.prevr, pn, refal.nexta);
            p = refal.nextr->prev;
            rftpl(p, refal.preva, refal.nexta);
            return;
        }
        if(p->tag == TAG_RB)
            p = p->info.codep;
    }
    p = p->prev;
    refal.preva->tag = TAG_LB;
    refal.preva->info.codep = pn;
    pn->tag = TAG_RB;
    pn->info.codep = refal.preva;
    rftpl(refal.preva, p, refal.nexta);
    rftpl(refal.prevr, pn, refal.nexta);
    p = refal.nextr->prev;
    rftpl(p, refal.nextr, refal.nexta);
    return;
}
static char last_0[] = { 'L', 'A', 'S', 'T', '\004' };
G_L_B char last asm("rlast") = '\122';
static void (*last_1)() = last_;

static void lengr_()
{
    linkcb_t* p;
    long n; /* kras */
    n = 0l;
    p = refal.preva->next;
    while(p != refal.nexta) {
        n++;
        p = p->next;
    }
    refal.preva->tag = TAG_N;
    pcoden(refal.preva, n);
    rftpl(refal.prevr, refal.nextr, refal.nexta);
    return;
}
static char lengr_0[] = { 'L', 'E', 'N', 'G', 'R', '\005' };
G_L_B char lengr asm("rlengr") = '\122';
static void (*lengr_1)() = lengr_;

static void lengw_()
{
    linkcb_t* p;
    long n; /* kras  */
    n = 0l;
    p = refal.preva->next;
    while(p != refal.nexta) {
        n++;
        if(p->tag == TAG_LB)
            p = p->info.codep;
        p = p->next;
    }
    refal.preva->tag = TAG_N;
    pcoden(refal.preva, n);
    rftpl(refal.prevr, refal.nextr, refal.nexta);
    return;
}
static char lengw_0[] = { 'L', 'E', 'N', 'G', 'W', '\005' };
G_L_B char lengw asm("rlengw") = '\122';
static void (*lengw_1)() = lengw_;

static void multe_()
{
    linkcb_t *p, *pn, *q;
    long n, k;
    pn = refal.preva->next;
    if((pn == refal.nexta) || (pn->tag != TAG_N)) {
        refal.upshot = 2;
        return;
    }; /* FAIL */
    n = gcoden(pn);
    if(n == 0)
        return;
    p = pn->next;
    if(p == refal.nexta)
        return;
    if(p->next != refal.nexta) {
        do {
            p = refal.nextr->prev;
            if(!lcopy(p, pn, refal.nexta)) {
                refal.upshot = 3;
                return;
            }; /* LACK */
            n--;
        } while(n >= 1);
    } else {
        if(!slins(refal.prevr, n))
            return; /*  LACK */
        q = refal.prevr;
        for(k = 0; k < n; k++) {
            q = q->next;
            q->tag = p->tag;
            pcoden(q, gcoden(p));
        }
    }
    return;
}
static char multe_0[] = { 'M', 'U', 'L', 'T', 'E', '\005' };
G_L_B char multe asm("rmulte") = '\122';
static void (*multe_1)() = multe_;

static void delf_()
{
    linkcb_t *dot, *dot1, *sk, *nd;
    if(refal.preva->next != refal.nexta) {
        refal.upshot = 2;
        return;
    } /*FAIL*/
    dot = refal.nexta;
    dot1 = refal.nextr->info.codep;
REPEAT:
    sk = dot->info.codep;
    if(sk == NULL)
        rfabe("delf: sign '#' missing ");
    dot = sk->info.codep;
    nd = dot->next;
    if(nd->info.infoc != '#')
        goto REPEAT;
    while(dot1 != dot) {
        sk = dot1->info.codep;
        nd = sk->info.codep;
        rfdel(sk->prev, dot1->next);
        dot1 = nd;
    }
    sk = dot1->info.codep;
    nd = sk->info.codep;
    dot1 = dot1->next;
    rfdel(sk->prev, dot1->next);
    refal.nextr->info.codep = nd;
    return;
}
static char delf_0[] = { 'D', 'E', 'L', 'F', '\004' };
G_L_B char delf asm("rdelf") = '\122';
static void (*delf_1)() = delf_;

static void crel_()
{
    char c;
    linkcb_t *p, *p1, *q, *q1;
    p = refal.preva->next;
    if(p->tag != TAG_LB)
        goto FAIL;
    p1 = p->info.codep;
    p = p->next;
    q = p1->next;
    q1 = refal.nexta;
    c = '=';
    for(; c == '=' && p != p1 && q != q1; p = p->next, q = q->next) {
        if(p->tag == TAG_LB) {
            if(q->tag != TAG_LB)
                goto FAIL;
            else
                break;
        }
        if(p->tag == TAG_RB) {
            if(q->tag != TAG_RB)
                goto FAIL;
            else
                break;
        }
        if(p->info.coden < q->info.coden)
            c = '<';
        if(p->info.coden > q->info.coden)
            c = '>';
    }
    if(p == p1 && q != q1)
        c = '<';
    if(q == q1 && p != p1)
        c = '>';
    for(; p != p1; p = p->next)
        if(p->tag == TAG_LB)
            goto FAIL;
    for(; q != q1; q = q->next)
        if(q->tag == TAG_LB)
            goto FAIL;
    p = refal.preva->next;
    p->tag = TAG_O;
    p->info.codep = NULL;
    p->info.infoc = c;
    q = p->next;
    rftpl(refal.prevr, refal.preva, q);
    return;
FAIL:
    refal.upshot = 2;
    return;
}
static char crel_0[] = { 'C', 'R', 'E', 'L', '\004' };
G_L_B char crel asm("rcrel") = '\122';
static void (*crel_1)() = crel_;

/*-------------------- end of file  XMO.C ----------------*/
