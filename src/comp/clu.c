/*----------------  file  --  CLU.C  -------------------*/
/*    AVL-tree construction for identifier table        */
/*           Last edition date : 11.01.92               */
/*------------------------------------------------------*/
#include "../refal.h"

extern struct {
    int nomkar;
    char modname_var[40];
    int modnmlen;
    int curr_stmnmb;
} scn_;

void Uns_sto()
{
    printf("\nNo memory for identifier table");
    exit(1);
}
identifier_t* korenj = NULL; /* tree koren */

identifier_t* nov_uzel(idp, lid) char* idp;
int lid;
{
    int m;
    identifier_t* p;
    char* q;
    p = (identifier_t*)calloc(1, sizeof(identifier_t));
    if(p == NULL)
        Uns_sto();
    p->i = p->j = NULL;
    p->k = '\000';
    p->mode = '\000';
    p->type = '\000';
    p->last_ref = &(p->ref);
    p->ref.next = NULL;
    for(m = 1; m <= 5; m++)
        p->ref.numb[m] = 0;
    p->ref.numb[0] = scn_.nomkar;
    p->def = 0;
    q = calloc(1, lid);
    if(q == NULL)
        Uns_sto();
    p->id = q;
    strncpy(q, idp, lid);
    p->l = lid;
    return (p);
}

identifier_t* lookup(idp, lid) int lid; /* identifier length */
char* idp;
{
    refw_t* r1;
    refw_t* q1;
    identifier_t *p, *q, *r, *isk_uz;
    int k;
    identifier_t* verquz;
    char kren, nruk;
    identifier_t* adruz[36]; /* stack for tree work */
    int otnuz[36];
    int tgld;            /* current  tree depth */
    if(korenj == NULL) { /* empty tree */
        korenj = nov_uzel(idp, lid);
        isk_uz = korenj;
        return (isk_uz);
    }
    /* tree is't empty,begin push. */
    /* remember path in stack */
    tgld = 0;
    p = korenj;
SHAG: /* search step */
    if(strncmp(idp, p->id, (lid < p->l) ? lid : p->l) == 0)
        if(lid == p->l) { /* include usage number to list */
            q1 = (*p).last_ref;
            k = 5;
            while((*q1).numb[k] == 0)
                k--;
            if((*q1).numb[k] != scn_.nomkar)
                /* include number to list */
                if((*q1).numb[5] == 0)
                    /* it's free field in current item */
                    (*q1).numb[k + 1] = scn_.nomkar;
                else { /* create new item */
                    r1 = (refw_t*)calloc(1, sizeof(refw_t));
                    if(r1 == NULL)
                        Uns_sto();
                    (*p).last_ref = (*q1).next = r1;
                    (*r1).next = NULL;
                    for(k = 0; k <= 5; k++)
                        (*r1).numb[k] = 0;
                    (*r1).numb[0] = scn_.nomkar;
                };
            while((((*p).mode) & '\300') == '\300')
                p = (*p).info.infop;
            return (p);
        } else {
            if(lid > p->l)
                kren = '\100';
            else
                kren = '\200';
            goto FINT;
        }
    if(strcmp(idp, p->id) > 0)
        kren = '\100';
    else
        kren = '\200';
FINT:
    adruz[tgld] = p;
    otnuz[tgld] = kren;
    tgld++;
    /* step down in tree */
    if(kren == '\100')
        q = (*p).j;
    else
        q = (*p).i;
    if(q != NULL) {
        p = q;
        goto SHAG;
    };
    /* include new node to tree */
    isk_uz = nov_uzel(idp, lid);
    q = isk_uz;
    if(kren == '\100')
        (*p).j = q;
    else
        (*p).i = q;
/* necessary node is new */
ISPRB: /* move up and correct */
    /* balance  features */
    tgld--;
    p = adruz[tgld];
    kren = (*p).k;
    if(kren == '\000') {
        (*p).k = otnuz[tgld];
        if(tgld != 0)
            goto ISPRB;
        return (isk_uz);
    };
    /* in this point kren != '\000' */
    if(kren != otnuz[tgld]) {
        (*p).k = '\000';
        return (isk_uz);
    };
    /* tree turn */
    /* if kren = '\100' -- left turn */
    /* if ( kren = '\200' -- right turn */
    if(kren == '\100')
        q = (*p).j;
    else
        q = (*p).i;
    if(kren == (*q).k) {
        if(kren == '\100') { /* once turn */
            (*p).j = (*q).i;
            (*q).i = p;
        } else {
            (*p).i = (*q).j;
            (*q).j = p;
        };
        (*p).k = (*q).k = '\000';
        verquz = q;
    } else { /* twos turn */
        if(kren == '\100') {
            r = (*q).i;
            (*p).j = (*r).i;
            (*q).i = (*r).j;
            (*r).i = p;
            (*r).j = q;
        } else {
            r = (*q).j;
            (*p).i = (*r).j;
            (*q).j = (*r).i;
            (*r).j = p;
            (*r).i = q;
        };
        nruk = !((*r).k) & '\300';
        if((*r).k == '\000')
            (*q).k = (*p).k = '\000';
        else if(nruk == kren) {
            (*p).k = '\000';
            (*q).k = nruk;
        } else {
            (*q).k = '\000';
            (*p).k = nruk;
        };
        (*r).k = '\000';
        verquz = r;
    }; /* end of twos turn */
    /* correct upper reference */
    if(tgld == 0)
        korenj = verquz;
    else {
        tgld--;
        if(otnuz[tgld] == '\100')
            (*adruz[tgld]).j = verquz;
        else
            (*adruz[tgld]).i = verquz;
    };
    return (isk_uz);
}

void traverse(identifier_t* ptr, int (*prog)())
{
    identifier_t *q, *r;
    q = ptr;
    do {
        r = (*q).i;
        if(r != NULL)
            traverse(r, prog);
        (*prog)(q);
        q = (*q).j;
    } while(q != NULL);
    return;
}

void kil_tree(identifier_t* p)
{
    identifier_t *r, *q;
    refw_t* r1;
    refw_t* r2;
    q = p;
    do {
        r = (*q).i;
        if(r != NULL)
            kil_tree(r);
        r2 = (*q).ref.next;
        while(r2 != NULL) {
            r1 = (*r2).next;
            free(r2);
            r2 = r1;
        }
        r = (*q).j;
        free(q->id);
        free(q);
        q = r;
    } while(q != NULL);
    return;
}
