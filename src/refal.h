#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum tags_t { TAG_O = 0, TAG_LB, TAG_F, TAG_RB, TAG_N, TAG_K, TAG_R, TAG_D } tags_t;

typedef struct refw_t {
    struct refw_t* next;
    int numb[6];
} refw_t;

typedef struct linkcb_t {
    struct linkcb_t* prev;
    struct linkcb_t* next;
    tags_t tag;
    union {
        struct linkcb_t* codep;
        char* codef;
        long coden;
        char infoc;
    } info;
} linkcb_t;

typedef struct st {
    struct st* stprev;
    struct st* stnext;
    linkcb_t* dot;
    linkcb_t* view;
    linkcb_t* store;
    long step;
    long stop;
    int state;
} st;

typedef struct identifier_t {
    union {
        int infon;
        struct identifier_t* infop;
    } info;
    char mode;
    /* mode field :                      */
    /*  00 - no defined;                          */
    /*  01 - internal; infon = offset from start  */
    /*  10 - external; infon = member or extern   */
    /*       reference;                           */
    /*  11 - equivalent; infop =  reference on    */
    /*       other label;                         */
    /*  xx1 - entry point;                        */
    /*  xxx1 - too many definition;               */
    /*                                            */
    char type;              /* type field : 00 - unknown type  */
                            /*              01 - function      */
                            /*              10 - specifier     */
    int l;                  /* identifier length */
    struct identifier_t* i; /* left reference */
    struct identifier_t* j; /* right reference */
    refw_t* last_ref;       /* on the end of using list */
    refw_t ref;             /* where used */
    int def;                /* where defined */
    char k;                 /* kren feature:      '00'B - kren no    */
                            /*                    '01'B - left kren  */
                            /*                    '10'B - right kren */
    char* id;               /* identifier */
} identifier_t;

typedef struct refalproc_t {
    st* crprev;
    st* crnext;
    st* currst;
    linkcb_t* preva;
    linkcb_t* nexta;
    linkcb_t* prevr;
    linkcb_t* nextr;
    linkcb_t* flhead;
    linkcb_t* svar;
    linkcb_t* dvar;
    long tmintv;
    int upshot;
    int stmnmb;
    int nostm;
    int tmmode;
} refalproc_t;

/* BLF - DBLF for prokrutka - trace debug mode, see rfintf.c */
/*#define DBLF*/

/* BLF -  PRINT_CODE - for print operation code, see rfrun1.c */
/*#define PRINT_CODE*/

#define gcoden(p) (p->info.coden)
#define pcoden(p, W) p->info.coden = W;
#define LBLL sizeof(void*)
#define SMBL (sizeof(long) + sizeof(void*))

#define G_L_B