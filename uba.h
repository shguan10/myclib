/*
Unbounded array implementation
*/

#include <stdlib.h>//malloc
#include <stdint.h>//SIZE_MAX
#include <string.h>//memcpy

/******* INTERFACE *******/
typedef void elem;//void, effectively an "INSERT TYPE HERE" since I want uba to be compatible with both primitives and pointers

typedef struct uba_header{
  size_t limit;//to the client, limit is always > nobj
  size_t nobj;
  size_t objsize;
  elem *data; //could be array of pointers or actual values
} uba;

typedef void uba_elem_free_fn(elem *e);

uba *newuba(size_t objsize, size_t nobj);
//requires objsize > 0, nobj > 0

void addelem(uba *u, const elem *e);
//requires u->nobj <= (SIZE_MAX / 2)
//requires e to be pointing to a space of size u->objsize
//NOTE, this function COPIES the bytes pointed to by e

void addelems(uba *u, const elem *e, size_t enobj);
//requires u->nobj <= (SIZE_MAX / 2)
//requires e to be pointing to a space of size enobj * u->objsize
//NOTE, this function COPIES the bytes pointed to by e

elem *getelem(uba *u, size_t i);
//returns NULL if i >= u->nobj

void freeuba(uba *u, uba_elem_free_fn* efree);
//use efree = NULL if free(u->data) also frees the elems (ie the elems are primitives)


/******* IMPLEMENTATION *******/
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
  //nobj == limit
  u->limit =( u->limit*2 <= u->limit? SIZE_MAX : u->nobj*2);//TODO is this correct?
  void *t = malloc(u->limit*u->objsize);
  memcpy(t,u->data,u->nobj*u->objsize);
  free(u->data);
  u->data = t;
}

void addelems(uba *u, const elem *e, size_t enobj){
  //calculate whether enobj goes over
  if(enobj + u->nobj >= u->limit ){
    u->limit =( u->limit*2 <= u->limit? SIZE_MAX : (u->nobj + enobj)*2);//TODO is this correct?
    void *t = malloc(u->limit*u->objsize);
    memcpy(t,u->data,u->nobj*u->objsize);
    free(u->data);
    u->data = t;
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
  if(efree!=NULL)
    for(size_t i = 0;i < u->nobj;i++)efree(ADDR(u,i));
  free(u->data);
  free(u);
}