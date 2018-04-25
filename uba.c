#include "uba.h"
#define ADDR(U,I) ((U)->data + (U)->objsize*(I))

uba *newuba(size_t objsize, size_t initlimit){
  uba *n = (uba *)malloc(sizeof(uba));
  n->limit = initlimit;
  n->nobj=0;
  n->objsize=objsize;
  n->data = (char *) malloc(objsize*initlimit);
  return n;
}

void addelem(uba *u, const elem *e){
  memcpy(ADDR(u,u->nobj),e,u->objsize);
  u->nobj++;
  if(u->nobj < u->limit) return;
  u->limit =( u->limit*2 <= u->limit? SIZE_MAX : u->nobj*2);
  u->data = realloc(u->data,u->limit*u->objsize);
}

void addelems(uba *u, const elem *e, size_t enobj){
  //calculate whether enobj goes over
  if(enobj + u->nobj >= u->limit ){
    u->limit =( u->limit*2 <= u->limit? SIZE_MAX : (u->nobj + enobj)*2);
    u->data = realloc(u->data,u->limit*u->objsize);    
  }
  memcpy(ADDR(u,u->nobj),e,enobj*u->objsize);
  u->nobj+=enobj;
  return;
}

elem *getelem(uba *u, size_t i){
  if(i >= u->nobj) return NULL;
  return u->data + u->objsize*i;
}

void freeuba(uba *u, uba_elem_free_fn* efree){
  if(efree) for(size_t i = 0;i < u->nobj;i++)efree(ADDR(u,i));
  free(u->data);
  free(u);
}