/*
Unbounded array implementation
*/

#include <stdlib.h>//malloc
#include <stdint.h>//SIZE_MAX
#include <string.h>//memcpy

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
