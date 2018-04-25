#include "uba.h"
#define ADDR(U,I) ((U)->data + (U)->objsize*(I))

//newuba returns a uba pointer, and initializes the uba to have limit initlimit
//requires objsize > 0, nobj > 0
uba *newuba(size_t objsize, size_t initlimit){
  uba *n = (uba *)malloc(sizeof(uba));
  n->limit = initlimit;
  n->nobj=0;
  n->objsize=objsize;
  n->data = (char *) malloc(objsize*initlimit);
  return n;
}

//requires u->nobj <= (SIZE_MAX / 2)
//requires e to be pointing to a space of size u->objsize
//NOTE, this function COPIES the bytes pointed to by e
void addelem(uba *u, const elem *e){
  memcpy(ADDR(u,u->nobj),e,u->objsize);
  u->nobj++;
  if(u->nobj < u->limit) return;
  u->limit =( u->limit*2 <= u->limit? SIZE_MAX : u->nobj*2);
  u->data = realloc(u->data,u->limit*u->objsize);
}

//requires u->nobj <= (SIZE_MAX / 2)
//requires e to be pointing to a space of size enobj * u->objsize
//NOTE, this function COPIES the bytes pointed to by e
void addelems(uba *u, const elem *e, size_t enobj){
  if(enobj + u->nobj >= u->limit ){
    u->limit =( u->limit*2 <= u->limit? SIZE_MAX : (u->nobj + enobj)*2);
    u->data = realloc(u->data,u->limit*u->objsize);    
  }
  memcpy(ADDR(u,u->nobj),e,enobj*u->objsize);
  u->nobj+=enobj;
  return;
}

//getelem: returns the pointer to the elem at index i
//returns NULL if i >= u->nobj
elem *getelem(uba *u, size_t i){
  if(i >= u->nobj) return NULL;
  return u->data + u->objsize*i;
}

//use efree = NULL if the elems do not need to be deep freed (eg the elems are primitives)
void freeuba(uba *u, uba_elem_free_fn* efree){
  if(efree) for(size_t i = 0;i < u->nobj;i++)efree(ADDR(u,i));
  free(u->data);
  free(u);
}