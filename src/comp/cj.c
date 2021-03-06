/*-----------------  file  --  cj_blf.C  ---------------*/
/*             generator of object module               */
/*                (or assembler text)                   */
/*           Last edition date :  21.05.2005 (BLF)      */
/*------------------------------------------------------*/

/* BLF - changes was made for Windows - COFF or GNU and
        for  Unix - ELF or GNU format */

#include "../refal.h"

extern FILE* syslin;

struct ent { /* entry table element */
    struct ent* next;
    identifier_t* p;
    char e[8];
    int le;
};
typedef struct ent T_ENT;

struct ext { /* external pointer table element */
    struct ext* next;
    identifier_t* p;
    char e[8];
    int le;
    int noms;
};
typedef struct ext T_EXT;

typedef struct T_RL {
    identifier_t* point;
    int delta;
} T_RL;

extern struct {
    unsigned source : 1;
    unsigned mincomp : 1;
    unsigned stmnmb : 1;
    unsigned extname : 1;
    unsigned multmod : 1;
    unsigned asmb : 1;
    unsigned names : 1;
} options;

static union {
    char b[2];
    short w;
} d;

extern char parm_i[]; /* sourse file name */
extern char vers_i[]; /* compiler version */
extern char mod_i[];  /* module name      */
extern FILE* systxt;  /* module names */
extern short nommod;

struct BU_ {
    unsigned len;
    unsigned tek;
    char* nam;
    char* buf;
    FILE* fil;
};
typedef struct BU_ BU;

static BU sysut1 = { 0, 0, NULL, NULL, NULL };
static BU sysut2 = { 0, 0, NULL, NULL, NULL };
BU sysl = { 0, 0, NULL, NULL, NULL };
int curr_r;
int new_r;
int equal_r; /* feature that new_r = curr_r */
char new_f[4];
T_ENT *q, *r;
T_EXT *qx, *rx;
char* ccc;

/*    MODE field value:
   00 - undefined;
   01 - internal,   INFO = shift against begin;
   10 - external,   INFO = external pointer number;
   11 - equivalent, INFO = another label pointer;
   XX1 - entry;
   XXX1 - still defined.
      TYPE field value:
   00 - unknown type;
   01 - function;
   10 - specifier.
*/

static T_ENT* first_ent;
static T_ENT* last_ent;
static long mod_length;
static char mod_name[9];
static int lnmmod;
static T_EXT* first_ext;
static T_EXT* last_ext;
static long curr_addr; /* module generation files  */
static int n_ext;
static T_RL rl;
static int k;
static int delta;

extern void error_message(char* s);

static void oshi1()
{
    fputs("Can't open (in) file sysut1\n", stdout);
    exit(8);
    return;
}
static void oshi2()
{
    fputs("Can't open (in) file sysut2\n", stdout);
    exit(8);
    return;
}
static void osho1()
{
    fputs("Can't open (out) file sysut1\n", stdout);
    exit(8);
    return;
}
static void osho2()
{
    fputs("Can't open (out) file sysut2\n", stdout);
    exit(8);
    return;
}
static void oshd1()
{
    fputs("Unsufficient disk space for sysut1\n", stdout);
    exit(8);
    return;
}
static void oshd2()
{
    fputs("Unsufficient disk space for sysut2\n", stdout);
    exit(8);
    return;
}
static void oshdl()
{
    fputs("Unsufficient disk space for syslin\n", stdout);
    exit(8);
    return;
}
static void osher1()
{
    fputs("I/o error in file sysut1\n", stdout);
    exit(8);
    return;
}
static void osher2()
{
    fputs("I/o error in file sysut2\n", stdout);
    exit(8);
    return;
}

static void ksmn();

void sfop_w(char* s, BU* b)
{
    long un;
    long lon;
    if(b->nam != NULL) {
        free(b->nam);
    }
    b->nam = e_malloc(strlen(s) + 1);
    strcpy(b->nam, s);
    b->len = 0;
    if(b->buf == NULL) {
        b->len = 65536;
        b->buf = (char*)malloc(b->len);
    }
    b->tek = 0;
    b->fil = NULL;
}

void sfop_r(b) BU* b;
{
    if(b->fil != NULL) {
        if((b->fil = fopen(b->nam, "rb")) == NULL) {
            printf("Can't open for read %s\n", b->nam);
            exit(8);
        }
        if(fread(b->buf, b->len, 1, b->fil) <= 0) {
            printf("Read i/o error in %s\n", b->nam);
            exit(8);
        }
    }
    b->tek = 0;
}

void sfcl(b) BU* b;
{
    if(b->fil != NULL) {
        if(fwrite(b->buf, b->tek, 1, b->fil) <= 0) {
            printf("Write i/o error in %s\n", b->nam);
            exit(8);
        }
        fclose(b->fil);
    }
}

void sfclr(b) BU* b;
{
    if(b->fil != NULL)
        remove(b->nam);
    free(b->nam);
    free(b->buf);
    b->nam = NULL;
    b->buf = NULL;
}

void sfwr2()
{
    unsigned ost;
    while(true) {
        ost = sysut2.len - sysut2.tek;
        if(ost >= 6) {
            memcpy(sysut2.buf + sysut2.tek, &rl, 6);
            sysut2.tek += 6;
            break;
        }
        if(sysut2.fil == NULL) {
            if((sysut2.fil = fopen(sysut2.nam, "wb")) == NULL) {
                printf("Can't open for write sysut2\n");
                exit(8);
            }
        }
        if(fwrite(sysut2.buf, sysut2.len, 1, sysut2.fil) <= 0) {
            printf("Write i/o error in sysut2\n");
            exit(8);
        }
        sysut2.tek = 0;
    } /*while*/
} /*sfwr2*/

void sfwr(char* c, unsigned n, BU* b)
{
    unsigned ost;
    while(true) {
        ost = b->len - b->tek;
        if(ost >= n) {
            memcpy(b->buf + b->tek, c, n);
            b->tek += n;
            break;
        }
        memcpy(b->buf + b->tek, c, ost);
        if(b->fil == NULL) {
            if((b->fil = fopen(b->nam, "wb")) == NULL) {
                printf("Can't open for write %s\n", b->nam);
                exit(8);
            }
        }
        if(fwrite(b->buf, b->len, 1, b->fil) <= 0) {
            printf("Write i/o error in %s\n", b->nam);
            exit(8);
        }
        b->tek = 0;
        n -= ost;
        c += ost;
    } /*while*/
} /*sfwr*/

void sfrd1(c, n) char* c;
unsigned n;
{
    unsigned ost;
    while(true) {
        ost = sysut1.len - sysut1.tek;
        if(ost >= n) {
            memcpy(c, sysut1.buf + sysut1.tek, n);
            sysut1.tek += n;
            break;
        }
        memcpy(c, sysut1.buf + sysut1.tek, ost);
        if(fread(sysut1.buf, sysut1.len, 1, sysut1.fil) <= 0) {
            /*printf("Read i/o error in sysut1\n"); exit(8);*/
        }
        sysut1.tek = 0;
        n -= ost;
        c += ost;
    } /*while*/
} /*sfrd1*/

void sfrd2()
{
    unsigned ost;
    while(true) {
        ost = sysut2.len - sysut2.tek;
        if(ost >= 6) {
            memcpy(&rl, sysut2.buf + sysut2.tek, 6);
            sysut2.tek += 6;
            break;
        }
        if(fread(sysut2.buf, sysut2.len, 1, sysut2.fil) <= 0) {
            /*printf("Read i/o error in sysut2\n");
            exit(8);*/
        }
        sysut2.tek = 0;
    } /*while*/
} /*sfrd2*/

void sfobmen(n) int n;
{
    unsigned ost1, ost2;
    while(true) {
        ost1 = sysut1.len - sysut1.tek;
        /*printf("\obmen: n=%d ost1=%d",n,ost1);*/
        if(n > ost1) {
            n -= ost1;
        } else {
            ost1 = n;
            n = 0;
        }
        while(ost1 > 0) {
            ost2 = sysl.len - sysl.tek;
            if(ost2 >= ost1) {
                memcpy(sysl.buf + sysl.tek, sysut1.buf + sysut1.tek, ost1);
                ksmn(sysut1.buf + sysut1.tek, ost1);
                sysl.tek += ost1;
                sysut1.tek += ost1;
                break;
            }
            memcpy(sysl.buf + sysl.tek, sysut1.buf + sysut1.tek, ost2);
            ksmn(sysut1.buf + sysut1.tek, ost2);
            if(sysl.fil == NULL) {
                if((sysl.fil = fopen(sysl.nam, "wb")) == NULL) {
                    printf("Can't open for write syslin\n");
                    exit(8);
                }
            }
            if(fwrite(sysl.buf, sysl.len, 1, sysl.fil) <= 0) {
                printf("Write i/o error in syslin\n");
                exit(8);
            }
            ost1 -= ost2;
            sysl.tek = 0;
            sysut1.tek += ost2;
        } /*while*/
        if(n != 0) {
            if(fread(sysut1.buf, sysut1.len, 1, sysut1.fil) <= 0) {
                /*printf("Read i/o error in sysut1\n");
                exit(8);*/
            }
            sysut1.tek = 0;
        } else
            break;
    } /*while*/
} /*sfomen*/

void jstart(char* ee, int ll)
{
    delta = 0;
    strncpy(mod_name, ee, ll);
    lnmmod = ll;
    sfop_w("sysut1.rf", &sysut1);
    sfop_w("sysut2.rf", &sysut2);
    first_ent = e_malloc(sizeof(T_ENT));
    last_ent = first_ent;
    first_ent->next = NULL;
    first_ext = e_malloc(sizeof(T_EXT));
    last_ext = first_ext;
    first_ext->next = NULL;
    curr_addr = 0;
    n_ext = 1;
}

unsigned jwhere()
{
    if(curr_addr > 65535) {
        printf("Module too long\n");
        exit(1);
    }
    return (curr_addr);
}

void jbyte(char bb)
{
    if(sysut1.tek != sysut1.len) {
        *(sysut1.buf + sysut1.tek) = bb;
        sysut1.tek++;
    } else {
        if(sysut1.fil == NULL) {
            if((sysut1.fil = fopen(sysut1.nam, "wb")) == NULL) {
                printf("Can't open for write sysut1\n");
                exit(8);
            }
        }
        if(fwrite(sysut1.buf, sysut1.len, 1, sysut1.fil) <= 0) {
            printf("Write i/o error in sysut1\n");
            exit(8);
        }
        *(sysut1.buf) = bb;
        sysut1.tek = 1;
    }
    delta++;
    curr_addr++;
}

void j3addr(pp) identifier_t* pp;
{
    rl.point = pp;
    rl.delta = delta;
    delta = 0;
    sfwr2();
    curr_addr += 4;
}

void jentry(identifier_t* pp, char* ee, int ll)
{
    r = first_ent;
    while(r != last_ent) {
        r = r->next;
        if((r->le == ll) && (strncmp(r->e, ee, ll < r->le ? ll : r->le) == 0)) {
            return;
        }
    }
    r = e_malloc(sizeof(T_ENT));
    last_ent->next = r;
    last_ent = r;
    r->p = pp;
    r->next = NULL;
    r->le = 8 < ll ? 8 : ll;
    strncpy(r->e, ee, r->le);
    pp->mode |= '\040';
}

void jextrn(identifier_t* pp, char* ee, int ll)
{
    rx = e_malloc(sizeof(T_EXT));
    last_ext->next = rx;
    last_ext = rx;
    rx->p = pp;
    rx->next = NULL;
    rx->le = 8 < ll ? 8 : ll;
    strncpy(rx->e, ee, rx->le);
    pp->mode |= '\220';
    n_ext++;
    pp->info.infon = n_ext;
}

void jlabel(pp) identifier_t* pp;
{
    pp->mode |= '\120';
    pp->info.infon = curr_addr;
}

void jequ(pp, qq) identifier_t* pp;
identifier_t* qq;
{
    pp->info.infop = qq;
    pp->mode |= '\320';
}

static void zakon()
{
    rl.delta = delta;
    rl.point = NULL;
    sfwr2();
    sfcl(&sysut1);
    sfcl(&sysut2);
    mod_length = curr_addr;
    if(mod_length < 0)
        mod_length = 65536L + mod_length;
} /*zakon*/

void jend()
{
    identifier_t *p, *pp;
    char bufs[81];
    int i;
    zakon();
    char tmp[256];
    strcpy(tmp, parm_i);
    fputs(tmp, systxt);
    char* pos = strrchr(tmp, '/');
    if(pos == NULL) {
        pos = strrchr(tmp, '\\');
    }
    if(pos == NULL) {
        pos = tmp - 1;
    }
    *(pos + 1) = '\0';
    strcat(tmp, mod_i);
    strcat(tmp, ".asm");
    syslin = fopen(tmp, "w");
    if(syslin == NULL) {
        printf("Can't open %s\n", tmp);
        exit(8);
    }
    d.w = 0;

    /* heading generating */

    fputs(".data\n", syslin); /* BLF */

    /* BLF fputc('_',syslin); for(i=0;i<lnmmod;i++) fputc(mod_name[i],syslin); */
    /* BLF fputs ("\tsegment\tbyte public 'CODE'\n",syslin); */
    /* BLF sprintf(bufs,"_d%d@\tlabel\tbyte\n",nommod); fputs (bufs,syslin); */
    /* BLF */
    sprintf(bufs, "_d%d$:\n", nommod); /* BLF */

    fputs(bufs, syslin);

    /*  empty module test    */
    if(mod_length == 0)
        goto JTERM;

    /* text generating */

    sfop_r(&sysut1);
    sfop_r(&sysut2);

GEN_TXT:
    sfrd2();
    delta = rl.delta;
    for(k = 0; k < delta; k++) {
        sfrd1(&d.b[0], 1);
        if((k % 60) == 0) {
            if(k != 0)
                fputc('\n', syslin);

            /* BLF */
            fputs("\t.byte\t", syslin);
        }
        sprintf(bufs, "%d", d.w);
        fputs(bufs, syslin);
        if(((k % 60) != 59) && (k != (delta - 1)))
            fputc(',', syslin);
    }
    fputc('\n', syslin);
    p = rl.point;
    if(p != NULL) {
        while(((p->mode) & '\300') == '\300')
            p = p->info.infop;
        if(((p->mode) & '\300') != '\200') {
            /*    internal label   */

            sprintf(bufs, "\t.long\t_d%d$+%u\n", nommod, p->info.infon);
            fputs(bufs, syslin);
        } else {
            /*     external   label   */
            fputs("\t.long\t", syslin);

            qx = first_ext;
            for(i = 1; i < p->info.infon; i++)
                qx = qx->next;

            fputc('r', syslin);
            for(i = 0; i < qx->le; i++)
                /* BLF    fputc (*((qx->e) + i),syslin);*/
                fputc(tolower(*((qx->e) + i)), syslin); /* BLF */
            fputs("\n", syslin);
        }
        goto GEN_TXT;
    } /*if*/

    /* end text generating */

    /*   external label generating    */

    qx = first_ext->next;
    while(qx != NULL) {

        fputs("\t.extern\t", syslin); /* BLF */
        fputc('r', syslin);
        for(i = 0; i < qx->le; i++)
            /* BLF fputc (*((qx->e) + i),syslin);*/
            fputc(tolower(*((qx->e) + i)), syslin); /* BLF */
        fputs(":byte\n", syslin);
        qx = qx->next;
    } /*while*/

    /* BLF */
    fputs(".data\n", syslin); /* BLF */

    /* entry label generating */

    q = first_ent->next;
    while(q != NULL) {
        fputc('r', syslin);
        for(i = 0; i < q->le; i++)
            /* BLF translate name to lower case */
            fputc(tolower(*((q->e) + i)), syslin);
        pp = q->p;
        while(((pp->mode) & '\300') == '\300')
            pp = pp->info.infop;
        /* BLF */
        sprintf(bufs, "\t=_d%d$+%d\n\t.globl\t", nommod, pp->info.infon);
        fputs(bufs, syslin);
        fputc('r', syslin);
        for(i = 0; i < q->le; i++)
            /* BLF translate name to lower case */
            fputc(tolower(*((q->e) + i)), syslin);
        fputc('\n', syslin);
        q = q->next;
    };

/* termination */

JTERM:

    sfclr(&sysut1);
    sfclr(&sysut2);
    if(options.multmod == 1) {
        fclose(syslin);
        if(mod_length != 0) {
            fputc(' ', systxt);
            fputs(mod_i, systxt);
        } else {
            nommod--;
            remove(mod_i);
        }
    }
    q = first_ent;
    while(q != NULL) {
        r = q->next;
        free(q);
        q = r;
    }
    qx = first_ext;
    while(qx != NULL) {
        rx = qx->next;
        free(qx);
        qx = rx;
    }
} /*jend*/

/*--------------------- OBJ -------------------*/
static union {
    char cc[2];
    short ww;
} stm, ksm;

static unsigned char c;

static void sfwrc()
{
    /*  sfwr(&c,1,&sysl);  */
    if(sysl.tek != sysl.len) {
        *(sysl.buf + sysl.tek) = c;
        sysl.tek++;
    } else {
        if(sysl.fil == NULL) {
            if((sysl.fil = fopen(sysl.nam, "wb")) == NULL) {
                printf("Can't open for write %s\n", sysl.nam);
                exit(8);
            }
        }
        if(fwrite(sysl.buf, sysl.len, 1, sysl.fil) <= 0) {
            printf("Write i/o error in %s\n", sysl.nam);
            exit(8);
        }
        *(sysl.buf) = c;
        sysl.tek = 1;
    }
} /*sfwrc*/

static void ksmb(b) char b;
{
    stm.cc[1] = 0;
    stm.cc[0] = b;
    ksm.ww += stm.ww;
    ksm.cc[1] = '\0';
}

static void ksmn(b, n) char* b;
int n;
{
    int i;
    stm.cc[1] = 0;
    for(i = 0; i < n; i++) {
        stm.cc[0] = *(b + i);
        ksm.ww += stm.ww;
    }
    ksm.cc[1] = '\0';
}

static void ksum()
{
    stm.cc[0] = d.b[0];
    ksm.ww += stm.ww;
    stm.cc[0] = d.b[1];
    ksm.ww += stm.ww;
    ksm.cc[1] = '\0';
}

static void zakr()
{
    d.w = 256 - ksm.ww;
    c = d.b[0];
    sfwrc();
    ksm.ww = 0;
}

static void imja(n, l) char* n;
int l;
{
    int i;
    d.w = l;
    c = d.b[0];
    sfwrc();
    ksmb(c);
    if(l != 0) {
        sfwr(n, l, &sysl);
        ksmn(n, l);
    }
}

static void imj_a(n, l) char* n;
int l;
{
    int i;
    d.w = l + 1;
    c = d.b[0];
    sfwrc();
    ksmb(c);
    c = '_';
    sfwrc();
    ksmb(c);
    if(l != 0) {
        sfwr(n, l, &sysl);
        ksmn(n, l);
    }
}

static void golowa(len, typ) int len, typ;
{
    d.w = typ;
    c = d.b[0];
    sfwrc();
    ksmb(c);
    d.w = len;
    sfwr(&d, 2, &sysl);
    ksum();
}