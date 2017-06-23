/*-------------  file  --  CS.C  ---------------*/
/*         Work with identifier table           */
/*        Last edition date : 29.10.86          */
/*----------------------------------------------*/
#include "../refal.h"
#define N_FAIL '\002'
#define N_NIL '\030'
#define N_SJUMP '\001'
#define N_SWAP '\116'
#define N_SETNOS '\121'

typedef struct i_lbl_t {
    union {
        int infoln;
        struct i_lbl_t* infolp;
    } infol;
    char model;
} i_ibl_t;

typedef struct arr_lbl_t {
    struct arr_lbl_t* nextl;
    struct i_lbl_t lbl[16];
} arr_lbl_t;

extern struct {
    unsigned source : 1;
    unsigned xref : 1;
    unsigned stmnmb : 1;
    unsigned extname : 1;
    unsigned multmod : 1;
    unsigned asmb : 1;
    unsigned names : 1;
} options;

extern struct {
    int nomkar;
    char modname_var[40];
    int modnmlen;
    int curr_stmnmb;
} scn_;

identifier_t* lookup();

static arr_lbl_t* first_arr_lbl = NULL;
static int n_lbl = 15;
static i_ibl_t* pfail = NULL;    /* statememt FAIL label */
static i_ibl_t* next_stm = NULL; /* next statement label */
static i_ibl_t* next_nos = NULL; /* next halfword label with  */
                                 /* a number of statements    */
void func_end();
void fnhead();
extern void error_message();
extern void error_message_label();
#define gop(n) jbyte(n)
void gopl();
void jequ();
void jbyte();
void jextrn();
void jentry();
void jlabel();
extern unsigned jwhere();
void ghw();
void error_message_label();
void through();
void luterm();

i_ibl_t* alloc_lbl()
{
    arr_lbl_t* q;
    i_ibl_t* p;
    if(n_lbl == 15) {
        q = calloc(1, sizeof(arr_lbl_t));
        if(q == NULL)
            Uns_sto();
        q->nextl = first_arr_lbl;
        first_arr_lbl = q;
        n_lbl = -1;
    }
    n_lbl = n_lbl + 1;
    p = &(first_arr_lbl->lbl[n_lbl]);
    p->model = '\000';
    return (p);
}
i_ibl_t* genlbl()
{
    i_ibl_t* p;
    p = alloc_lbl();
    return (p);
}
void fndef(idp, lid) char* idp;
int lid;
{
    identifier_t* p;
    if(lid != 0) { /* new function */
        func_end();
        p = lookup(idp, lid);
        scn_.curr_stmnmb = 0;
        next_stm = alloc_lbl();
        p->type = (p->type) | '\100';
        if((p->mode) & '\020')
            error_message_label("504 label", idp, lid, " is already defined");
        else {
            fnhead(idp, lid);
            p->def = scn_.nomkar;
            jlabel(p);
            if(options.stmnmb == 1) {
                next_nos = alloc_lbl();
                gopl(N_SETNOS, next_nos);
            };
            gopl(N_SJUMP, next_stm);
        }
    } else { /*  next statement in function   */
        if(next_stm != NULL)
            jlabel(next_stm);
        else
            error_message("500 no statement label");
        next_stm = alloc_lbl();
        gopl(N_SJUMP, next_stm);
    };
    return; /*  eg */
}
void func_end()
{
    if(next_stm != NULL) {
        if(pfail != NULL)
            jequ(next_stm, pfail);
        else {
            pfail = next_stm;
            jlabel(next_stm);
            gop(N_FAIL);
        }
        next_stm = NULL;
        if(options.stmnmb == 1) {
            jlabel(next_nos);
            ghw(scn_.curr_stmnmb);
        }
    }
    return;
}
void sempty(idp, lid) char* idp;
int lid;
{
    identifier_t* p;
    p = lookup(idp, lid);
    p->type = (p->type) | '\100';
    if(p->mode & '\020')
        error_message_label("504 label", idp, lid, " is already defined");
    else {
        fnhead(idp, lid);
        p->def = scn_.nomkar;
        jlabel(p);
        gop(N_FAIL);
    }
    return;
}
void sswap(idp, lid) char* idp;
int lid;
{
    identifier_t* p;
    int l0, j0, k0, kk;
    p = lookup(idp, lid);
    p->type = (p->type) | '\100';
    if(p->mode & '\020')
        error_message_label("504 label", idp, lid, " is already defined");
    else { /*  align box head on the word board */
        j0 = jwhere();
        if(options.extname == 1)
            l0 = 255 > (scn_.modnmlen + lid + 1) ? (scn_.modnmlen + lid + 1) : 255;
        else
            l0 = lid;
        j0 = (j0 + l0 + 2) % 4;
        if(j0 != 0)
            j0 = 4 - j0;
        for(k0 = 1; k0 <= j0; k0++)
            jbyte(' ');
        fnhead(idp, lid);
        p->def = scn_.nomkar;
        jlabel(p);
        gop(N_SWAP);
        kk = SMBL + LBLL * 2;
        for(k0 = 1; k0 <= kk; k0++)
            jbyte('\000');
    }
    return;
}

void sentry(idp, lidp, ide, lide) char* idp; /* internal name */
char* ide;                                   /* external name */
int lidp, lide;
{
    identifier_t* p;
    p = lookup(idp, lidp);
    jentry(p, ide, lide);
    return; /* eg */
}
void sextrn(idp, lidp, ide, lide) char* idp; /* internal name */
char* ide;                                   /* external name */
int lidp, lide;
{
    /*  int ind; */ /* eg */
    identifier_t* p;
    p = lookup(idp, lidp);
    if((p->mode) & '\020')
        error_message_label("504 label", idp, lidp, " is already defined");
    else {
        p->def = scn_.nomkar;
        jextrn(p, ide, lide);
    }
    return; /*  eg */
}
identifier_t* fnref(idp, lid) char* idp;
int lid;
{
    identifier_t* p;
    p = lookup(idp, lid);
    p->type = (p->type) | '\100';
    return (p);
}
identifier_t* spref(idp, lid, d) char* idp;
char d;
int lid;
{
    identifier_t* p;
    p = lookup(idp, lid);
    p->type = (p->type) | '\200';
    if((d != ')') && (((p->mode) & '\020') != '\020')) {
        error_message_label("505 label", idp, lid, " is yet not defined");
    }
    return (p);
}
void spdef(idp, lid) char* idp;
int lid;
{
    identifier_t* p;
    if(lid == 0)
        error_message("500 no statement label");
    else { /* label exist */
        p = lookup(idp, lid);
        p->type = (p->type) | '\200';
        if((p->mode) & '\020') {
            error_message_label("504 label", idp, lid, " is already defined");
        } else {
            p->def = scn_.nomkar;
            jlabel(p);
        }
    }
    return; /*  eg */
}
void sequ(id1, lid1, id0, lid0) char *id1, *id0;
int lid0, lid1;
{
    identifier_t *p0, *p1;
    p0 = lookup(id0, lid0);
    if(lid1 == 0) {
        error_message("500 no statement label");
        return;
    }
    p1 = lookup(id1, lid1);
    if(p0 == p1)
        return;
    if(((p1->mode) & '\300') == '\000') {
        p0->type = (p0->type) | (p1->type);
        p1->def = scn_.nomkar;
        jequ(p1, p0);
    } else if(((p0->mode) & '\300') == '\000') {
        p1->type = (p1->type) | (p0->type);
        p0->def = scn_.nomkar;
        jequ(p0, p1);
    } else
        error_message("501 both labels already defined ");
    return; /* eg */
}
void fnhead(idp, lid) char* idp;
int lid;
{
    char* idpm;
    int k0, l0, ll;
    if(options.names == 1) {
        if(options.extname == 1) {
            idpm = scn_.modname_var; /* eg */
            l0 = scn_.modnmlen;
            for(k0 = 0; k0 < l0 && k0 < 8; k0++)
                jbyte(*(idpm + k0));
            jbyte(':');
            ll = k0 + 1;
        } else
            ll = 0;
        l0 = lid;
        for(k0 = 0; k0 < l0; k0++)
            jbyte(*(idp + k0));
        jbyte((char)(255 < ll + l0 ? 255 : ll + l0));
    } else
        jbyte('\0');
    return; /* eg */
}
void check_id(pp) /* check identifier attributes on confirmness */
    identifier_t* pp;
{
    identifier_t* q;
    q = pp;
    /* printf("\nCHECK: pp=%lx q=%lx mode=%o$$$",pp,q,q->mode); */
    while(((q->mode) & '\300') == '\300')
        q = q->info.infop;
    if(((pp->mode) & '\300') == '\000') {
        error_message_label("512 label", pp->id, pp->l, " not defined");
    }
    if((((pp->mode) & '\040') == '\040') && (((pp->mode) & '\300') == '\200'))
        error_message_label("511 label", pp->id, pp->l, " both extern and entry");
    if(((q->mode) & '\300') == '\300')
        error_message_label("502 label", pp->id, pp->l, " boht specifier and function");
}
void s_end()
{
    func_end();
    through(check_id);
}
void s_init()
{ /* module initiation  */
    first_arr_lbl = NULL;
    n_lbl = 15;
    pfail = NULL;
    next_stm = NULL;
    return; /* eg */
}
void s_term()
{ /* module termination */
    arr_lbl_t *p, *p1;
    p = first_arr_lbl;
    while(p != NULL) {
        p1 = p->nextl;
        free(p);
        p = p1;
    }
    luterm();
    return; /* eg */
}
