/*-------------- file -- xar_blf.c ---------*/
/*              Multi-digits MO:            */
/*       add, sub, mul, div, dr, nrel       */
/*       addn, subn, muln, divn, drn,       */
/*      Last edition date : 23.02.2005 (BLF)*/
/*------------------------------------------*/
#include "../refal.h"
#include <stdio.h>
extern refalproc_t refal;

#define Oadd 1
#define Osub 2
#define Omul 3
#define Odr 5

#define HMAX 4096L
#define MAX 16777216L
#define SMAX 24
static void oper();

static void add_()
{
    oper(Oadd, 0);
}

static char add_0[] = { 'A', 'D', 'D', '\003' };
G_L_B char add __asm__("radd") = '\122';

static void (*add_1)() = add_;

static void sub_()
{
    oper(Osub, 0);
}

static char sub_0[] = { 'S', 'U', 'B', '\003' };
G_L_B char sub __asm__("rsub") = '\122';
static void (*sub_1)() = sub_;

static void mul_()
{
    oper(Omul, 0);
}

/* BLF */
static char mul_0[] = { 'M', 'U', 'L', '\003' };
G_L_B char mul __asm__("rmul") = '\122';
static void (*mul_1)() = mul_;

static void dr_()
{
    oper(Odr, 0);
}
static char dr_0[] = { 'D', 'R', '\002' };
G_L_B char dr __asm__("rdr") = '\122';
static void (*dr_1)() = dr_;

static void divv_()
{
    oper(Odr, 2);
}

/* BLF */
static char div_0[] = { 'D', 'I', 'V', '\003' };
G_L_B char div_ __asm__("rdiv") = '\122';
static void (*div_1)() = divv_;

static void addn_()
{
    oper(Oadd, 1);
}
static char addn_0[] = { 'A', 'D', 'D', 'N', '\004' };
G_L_B char addn __asm__("raddn") = '\122';
static void (*addn_1)() = addn_;

static void subn_()
{
    oper(Osub, 1);
}
static char subn_0[] = { 'S', 'U', 'B', 'N', '\004' };
G_L_B char subn __asm__("rsubn") = '\122';
static void (*subn_1)() = subn_;

static void muln_()
{
    oper(Omul, 1);
}
static char muln_0[] = { 'M', 'U', 'L', 'N', '\004' };
G_L_B char muln __asm__("muln") = '\122';
static void (*muln_1)() = muln_;

static void drn_()
{
    oper(Odr, 1);
}
static char drn_0[] = { 'D', 'R', 'N', '\003' };
G_L_B char drn __asm__("rdrn") = '\122';
static void (*drn_1)() = drn_;

static void divn_()
{
    oper(Odr, 3);
}
static char divn_0[] = { 'D', 'I', 'V', 'N', '\004' };
G_L_B char divn __asm__("rdivn") = '\122';
static void (*divn_1)() = divn_;

static linkcb_t *x, *y, *nach, *kon, *Xn, *Yn, *Xk, *Yk;
static int dl, Xdl, Ydl;
static char zn, Xzn, Yzn;

static dajch()
{
    zn = '+';
    kon = y->prev;
    if(x == kon) { /* pustoe chislo */
        dl = 0;
        return (true);
    }
    x = x->next;
    if((x->tag == TAG_O) && ((x->info.infoc == '+') || (x->info.infoc == '-'))) {
        zn = x->info.infoc;
        x = x->next;
        if(x == y)
            return (false); /*  w chisle - lish znak */
    }
    for(; (x->tag == TAG_N) && (gcoden(x) == 0l); x = x->next)
        ;
    if(x == y)
        dl = 0; /*  wse cifry - nuli */
    else {
        for(dl = 0, nach = x; (x->tag == TAG_N); x = x->next, dl++)
            ;
        if(x != y)
            return (false); /* ne makrocifra */
    }
    return (true);
}

static dajarg()
{
    x = refal.preva->next;
    if(x->tag != TAG_LB)
        return (false);
    y = x->info.codep;
    if(dajch()) {
        Xn = nach;
        Xk = kon;
        Xzn = zn;
        Xdl = dl;
    } else
        return (false);
    x = y;
    y = refal.nexta;
    if(dajch()) {
        Yn = nach;
        Yk = kon;
        Yzn = zn;
        Ydl = dl;
        return (true);
    } else
        return (false);
}

static void obmen()
{
    linkcb_t* p;
    int i;
    char c;

    p = Xn;
    Xn = Yn;
    Yn = p;
    p = Xk;
    Xk = Yk;
    Yk = p;
    i = Xdl;
    Xdl = Ydl;
    Ydl = i;
    c = Xzn;
    Xzn = Yzn;
    Yzn = c;
}

static xmy()
{ /*  if X < Y then true  ( po modulju) */
    if(Xdl < Ydl)
        return (true);
    if(Xdl > Ydl)
        return (false);
    for(x = Xn, y = Yn; (x != Xk->next); x = x->next, y = y->next) {
        if(gcoden(x) < gcoden(y))
            return (true);
        if(gcoden(x) > gcoden(y))
            return (false);
    }
    return (2); /* x=y */
}

static void ymn(a, b) long *a, *b;
{ /* rez.: a - st., b - ml */
    int a1, a2, b1, b2, r1, r2, r3, r4;
    long r;
    if(*a == 0l) {
        *b = 0l;
        return;
    }
    if(*b == 0l) {
        *a = 0l;
        return;
    }
    a1 = (*a) >> 12;
    b1 = (*b) >> 12;
    a2 = (*a) & 0xFFF;
    b2 = (*b) & 0xFFF;
    r = a2 * (long)b2;
    *b = r & 0xFFF;
    r3 = r >> 12;
    r = a1 * (long)b2;
    r3 += r & 0xFFF;
    r2 = r >> 12;
    r = a2 * (long)b1;
    r3 += r & 0xFFF;
    r2 += r >> 12;
    r = a1 * (long)b1;
    r2 += r & 0xFFF;
    r1 = r >> 12;
    r4 = r3 >> 12;
    *a = r1 * HMAX + r2 + r4;
    *b += (r3 & 0xFFF) * HMAX;
}

static void norm(X, dl, j) /*  normaliz. posledov. makrocifr */
    linkcb_t* X;
int dl, j;
{ /*  X - ukaz. na konec            */
    long a, g, m, peren;
    int i, ip;
    peren = 0l;
    ip = 24 - j;
    m = 0xFFFFFFl >> j; /* maska */
    for(i = 0; i < dl; i++) {
        g = gcoden(X);
        a = (g & m) << j;
        pcoden(X, (long)(a | peren));
        peren = g >> ip;
        X = X->prev;
    }
}

static void oper(o, prn) int o, prn;
{
    linkcb_t *p, *r, *f, *Xt, *Yt;
    long j, peren;
    int i, n, a11, b11, a22, b22, r1, r2, r3, r4;
    long a, a1, b, b1, c, d, x1, x2, y1, y2;

    if(!dajarg()) {
        refal.upshot = 2;
        return;
    }
    switch(o) {
    case Osub: /* izmenim znak i skladywaem  */
        if(Yzn == '-')
            Yzn = '+';
        else
            Yzn = '-';
    case Oadd:
        if((Xdl == 0) && (Ydl == 0)) {
        REZ0:
            if(prn == 1)
                return; /* dlja n-operacij */
            x = refal.preva->next;
            x->tag = TAG_N;
            pcoden(x, 0l);
            rftpl(refal.prevr, x->prev, x->next);
            return;
        }
        if(Xdl == 0) { /*  rezult - wtoroe chislo  */
            Xn = Yn;
            Xk = Yk;
            Xzn = Yzn;
        } else {
            if(Ydl != 0) {       /* summiruem  */
                if(Xzn == Yzn) { /*  skladywaem  */
                    if(Xdl < Ydl)
                        obmen();
                    /*  X  dlinnee  Y  (ili =)  */
                    Xn = Xn->prev; /*  pripisywaem  0  */
                    Xn->tag = TAG_N;
                    pcoden(Xn, 0l);
                    peren = 0L;
                    for(x = Xk, y = Yk; (x != Xn->prev); x = x->prev) {
                        if(y != Yn->prev) {
                            pcoden(x, gcoden(x) + gcoden(y) + peren);
                            y = y->prev;
                        } else
                            pcoden(x, gcoden(x) + peren);
                        if(gcoden(x) >= MAX) {
                            pcoden(x, gcoden(x) - MAX);
                            peren = 1L;
                        } else
                            peren = 0L;
                    }    /*for*/
                } else { /* wychitaem  */
                    if(xmy() == 2)
                        goto REZ0;
                    if(xmy())
                        obmen();   /*  menjaem x i y  */
                    Xn = Xn->prev; /*  pripisywaem 0  */
                    Xn->tag = TAG_N;
                    pcoden(Xn, 0l);
                    peren = 0L;
                    for(x = Xk, y = Yk; (x != Xn->prev); x = x->prev) {
                        j = gcoden(x);
                        if(y != Yn->prev) {
                            j -= (gcoden(y) + peren);
                            y = y->prev;
                        } else
                            j -= peren;
                        if(j < 0L) {
                            j += MAX;
                            peren = 1;
                        } else
                            peren = 0L;
                        pcoden(x, j);
                    } /*for*/
                }     /*if*/
            }
        }
        break;
    case Omul:
        if((Xdl == 0) || (Ydl == 0))
            goto REZ0;
        if(lrqlk(Xdl + Ydl + 1) == 0) {
            refal.upshot = 3;
            return;
        }
        p = refal.preva;
        r = p->next;
        lins(p, Xdl + Ydl + 1); /*  1 zweno dlja znaka  */
        p = p->next;
        r = r->prev;
        for(x = p; x != r->next; x = x->next) {
            x->tag = TAG_N;
            pcoden(x, 0l);
        } /*  zanulen rezultat  */
        if(Xdl < Ydl)
            obmen();
        /*  dobawim 0 k X dlja summir. s perenosom  */
        Xn = Xn->prev;
        Xn->tag = TAG_N;
        pcoden(Xn, 0l);
        for(f = r, y = Yk; y != Yn->prev; y = y->prev, f = f->prev) {
            d = gcoden(y);
            if(d != 0l) { /* umn. na 1 cifru  */
                peren = 0L;
                b11 = d >> 12;
                b22 = d & 0xFFF;
                for(x = Xk, p = f; x != Xn->prev; x = x->prev, p = p->prev) {
                    a = gcoden(x);
                    if(a == 0l) {
                        b = 0l;
                        goto ret;
                    }
                    a11 = a >> 12;
                    a22 = a & 0xFFF;
                    c = a22 * (long)b22;
                    b = c & 0xFFF;
                    r3 = c >> 12;
                    c = a11 * (long)b22;
                    r3 += c & 0xFFF;
                    r2 = c >> 12;
                    c = a22 * (long)b11;
                    r3 += c & 0xFFF;
                    r2 += c >> 12;
                    c = a11 * (long)b11;
                    r2 += c & 0xFFF;
                    r1 = c >> 12;
                    r4 = r3 >> 12;
                    a = r1 * HMAX + r2 + r4;
                    b += (r3 & 0xFFF) * HMAX;
                    /* ymn (&a,&b); */ /* rez:a-st, b-ml  */
                ret:
                    j = gcoden(p) + b + peren;
                    peren = 0L;
                    if(j >= MAX) {
                        j -= MAX;
                        peren++;
                    }
                    peren += a;
                    pcoden(p, j);
                } /*for*/
            }
        }
        if(Xzn != Yzn)
            Xzn = '-';
        else
            Xzn = '+';
        Xn = p;
        Xk = r;
        break;
    case Odr:
        if(Ydl == 0) {
            refal.upshot = 2;
            return;
        }
        if(Xdl == 0) {
            a = 0L;
            b = 0L;
            Xzn = '+';
            Yzn = '+';
            goto odnc;
        }
        if(xmy() == 2) { /*  rawny  */
            a = 0L;
            b = 1L;
            goto odnc;
        }
        if(xmy()) {              /*  delimoe < delitelja  */
            if((prn & 2) == 2) { /* DIV, DIVN */
                a = 0L;
                b = 0L;
                Xzn = '+';
                Yzn = '+';
                goto odnc;
            }
            if(Xzn == '-') {
                Xn = Xn->prev;
                Xn->tag = TAG_O;
                Xn->info.codep = NULL;
                Xn->info.infoc = '-';
            }
            Xn = Xn->prev;
            Xk = Xk->next;
            Xn->tag = TAG_LB;
            Xk->tag = TAG_RB;
            Xn->info.codep = Xk;
            Xk->info.codep = Xn;
            if(prn == 0) {
                Xn = Xn->prev;
                Xn->tag = TAG_N;
                pcoden(Xn, 0l);
            }
            rftpl(refal.prevr, Xn->prev, Xk->next);
            return;
        }
        /*   delimoe > delitelja */
        if(Xdl == 1) { /*  oba po odnoj cifre  */
            a = (gcoden(Xn)) % (gcoden(Yn));
            b = (gcoden(Xn)) / (gcoden(Yn));
            goto odnc;
        }
        /*  delenie mnogih  cifr  */
        if(lrqlk((Xdl - Ydl) + 2) == 0) {
            refal.upshot = 3;
            return;
        }
        /* t.k. k chastnomu dob. 0 i zweno na znak */
        p = refal.preva;
        lins(p, (Xdl - Ydl) + 2);
        p = p->next; /*  dlja znaka  */
        r = p->next; /*  dlja  perwoj  cifry  */
        nach = r;
        Xn = Xn->prev;
        Xn->tag = TAG_N;
        pcoden(Xn, 0l);
        Xdl++;
        for(i = 0, x = Xn; i < Ydl; i++, x = x->next)
            ;
        y = Yn->prev;
        y->tag = TAG_N;
        pcoden(y, 0l);
        if(Ydl != 0) { /* wozmovna normalizacija */
            b = gcoden(Yn);
            for(n = 0; b < 8388608; n++, b += b)
                ;
            if(n != 0) {
                norm(Xk, Xdl, n);
                norm(Yk, Ydl, n);
            }
        }
        do {
            a = gcoden(Xn), a1 = gcoden(Xn->next);
            b = gcoden(Yn);
            /*printf("\na=%ld_%ld b=%ld b1=%ld",a,a1,
                                b,(long)gcoden(Yn->next));*/
            if((a == 0l) && (a1 < b))
                c = 0l;
            else {
                if((a == 0l) && (a1 >= b)) {
                    c = 1l; /*  t.k. b - normalizowano */
                    a = a1;
                } else { /* delim a,a1 na b */
                    a = (a * 128) + (a1 >> 17);
                    c = (a / b) << 17;
                    b1 = a1 >> 10;
                    a = ((a % b) * 128) + (/*(a1/1024)*/ b1 & 0x7F);
                    c = c + ((a / b) * 1024);
                    b1 = a1 >> 3;
                    a = ((a % b) * 128) + (/*(a1/8)*/ b1 & 0x7F);
                    c = c + ((a / b) * 8);
                    a = ((a % b) * 8) + (a1 & 7);
                    c = c + a / b;
                }
                /*printf("\nc=%ld oct=%ld",c,(long)(a%b));*/
                if((Ydl > 1) && ((b1 = gcoden(Yn->next)) != 0l)) {
                    x1 = b1;
                    x2 = c;
                    ymn(&x1, &x2);
                    y1 = a % b;
                    y2 = gcoden(Xn->next->next);
                    /*printf("\nBegin: c=%ld ",c);
                    printf(" x=%lx_%lx (b1*c)",x1,x2);
                    printf(" y=%lx_%lx (o..a2)",y1,y2);*/
                    i = 0;
                    while((x1 > y1) || (x1 == y1 && x2 > y2)) {
                        c--;
                        i = 1;
                        x1 = b1;
                        x2 = c;
                        ymn(&x1, &x2);
                        y1 += b;
                        /*printf("\nc=%lx ",c);
                        printf(" x=%lx_%lx (b1*c)",x1,x2);
                        printf(" y=%lx_%lx (o..a2)",y1,y2);*/
                    }
                    if(i == 1)
                        c++; /* na wcjakij sluchaj */
                }
            }
            /* umnovenie  delitelja  na 'c' i wychit. iz X */
            if(c != 0L) {
                Yt = Yk;
                Xt = x;
                peren = 0L;
                for(; Yt != y->prev; Xt = Xt->prev, Yt = Yt->prev) {
                    b = gcoden(Yt);
                    a = c;
                    ymn(&a, &b);
                    b += peren;
                    peren = (b >> SMAX);
                    b = b & (MAX - 1);
                    j = gcoden(Xt);
                    if(j < b) {
                        j += MAX;
                        peren += 1L;
                    }
                    pcoden(Xt, j - b);
                    peren += a;
                }
                if(peren != 0L) {   /* cifra welika  */
                    /*long jj=0l;*/ /* !!! wremenno !!! */
                    do {
                        /*jj++;*/
                        c -= 1L;
                        Xt = x;
                        Yt = Yk;
                        j = 0L;
                        for(; Yt != y->prev; Xt = Xt->prev, Yt = Yt->prev) {
                            a = gcoden(Xt) + gcoden(Yt) + j;
                            j = 0L;
                            if(a >= MAX) {
                                a -= MAX;
                                j = 1L;
                            }
                            pcoden(Xt, a);
                        }
                        peren -= j;
                    } while(peren != 0L);
                    /*printf("\nc veliko jj=%ld",jj);*/
                }
            }
            r->tag = TAG_N;
            pcoden(r, c);
            r = r->next;
            x = x->next;
            Xn = Xn->next;
        } while(x != Xk->next);
        Xn = Xn->prev;
        r = r->prev;
        if(n != 0) { /* denormalizacija ostatka */
            peren = 0L;
            i = 24 - n;
            c = 0xFFFFFFl >> i;
            for(x = Xn; x != Xk->next; x = x->next) {
                a = gcoden(x);
                b = (a >> n) | (peren << i);
                peren = a & c;
                pcoden(x, b);
            }
        }
        /*for(i=0,x=Xn; x != Xk->next; x=x->next,i++)
           printf("\n ostat(%d)=%ld",i,gcoden(x));
        for(i=0,x=nach; x != r->next; x=x->next,i++)
           printf("\n chast(%d)=%ld",i,gcoden(x));
        */
        for(x = Xn; (x != Xk->next) && (gcoden(x) == 0l); x = x->next)
            ;
        x = x->prev;
        if(x != Xk) {
            if(Xzn != Yzn) {
                x->tag = TAG_O;
                x->info.codep = NULL;
                x->info.infoc = '-';
            } else
                x = x->next;
        }
        Xn = x;
        for(x = nach; gcoden(x) == 0l; x = x->next)
            ;
        if(Xzn == '-') {
            x = x->prev;
            x->tag = TAG_O;
            x->info.codep = NULL;
            x->info.infoc = '-';
        }
        if((prn & 1) == 0 || Xn != Xk || gcoden(Xn) != 0l)
            Xn = Xn->prev;
        Xn->tag = TAG_LB;
        Xn->info.codep = Xk->next;
        Xk = Xk->next;
        Xk->tag = TAG_RB;
        Xk->info.codep = Xn;
        if(r->next != Xn)
            rftpl(r, Xn->prev, Xk->next);
        if((prn & 2) == 0)
            rftpl(refal.prevr, x->prev, Xk->next);
        else
            rftpl(refal.prevr, x->prev, Xn);
        return;
    } /* end case */
    /*  wozwratim X */
    /* podawim wed. nuli */
    for(x = Xn; (gcoden(x) == 0l); x = x->next)
        ;
    if(prn == 1 && x == Xk && gcoden(x) == 0l)
        return;
    if(Xzn == '-') {
        x = x->prev;
        x->tag = TAG_O;
        x->info.codep = NULL;
        x->info.infoc = '-';
    }
    /*  perenosim reultat */
    rftpl(refal.prevr, x->prev, Xk->next);
    return;
odnc: /* wywod rezultata delenija, kogda ostatok i chastnoe */
    /* rawno po odnoj makrocifre a - ost., b - chastnoe  */
    if(slins(refal.preva, 2) == 0) {
        refal.upshot = 3;
        return;
    }
    /* in bad case: /1/() - 3 zwena est uje + name */
    x = refal.preva;
    if(Xzn != Yzn) {
        x->tag = TAG_O;
        x->info.codep = NULL;
        x->info.infoc = '-';
        x = x->next;
    }
    if(b != 0l || ((prn & 1) == 0)) { /* div/dr */
        x->tag = TAG_N;
        pcoden(x, b);
        x = x->next;
    }
    y = x->next;
    if(a != 0L) {
        if(Xzn != '+') {
            y->tag = TAG_O;
            y->info.codep = NULL;
            y->info.infoc = '-';
            y = y->next;
        }
    }
    if(a != 0l || ((prn & 1) == 0)) { /* div/dr */
        y->tag = TAG_N;
        pcoden(y, a);
        y = y->next;
    }
    x->tag = TAG_LB;
    y->tag = TAG_RB;
    x->info.codep = y;
    y->info.codep = x;
    if((prn & 2) == 0) { /* dr/n */
        rftpl(refal.prevr, refal.preva->prev, y->next);
    } else { /* div/n */
        rftpl(refal.prevr, refal.preva->prev, x);
    }
    return;
}

static void nrel_()
{
    char c;
    if(!dajarg()) {
        refal.upshot = 2;
        return;
    }
    if((Xdl == 0) && (Ydl == 0))
        c = '=';
    else {
        if((Xzn == Yzn) && (xmy() == 2))
            c = '=';
        else {
            if(((Xzn == '-') && (Yzn == '+')) || ((Xzn == '-') && (Yzn == '-') && !xmy()) ||
                ((Xzn == '+') && (Yzn == '+') && xmy()))
                c = '<';
            else
                c = '>';
        }
    }
    refal.preva->tag = TAG_O;
    refal.preva->info.codep = NULL;
    refal.preva->info.infoc = c;
    rftpl(refal.prevr, refal.preva->prev, refal.nexta);
    return;
}
static char nrel_0[] = { 'N', 'R', 'E', 'L', '\004' };
G_L_B char nrel __asm__("rnrel") = '\122';
static void (*nrel_1)() = nrel_;

/*-------------------- end of file  XAR.C ----------------*/
