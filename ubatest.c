#include <stdio.h>
#include <stdlib.h>
#include "uba.h"

void printinfo(uba *u){
  printf("nobj: %d\n",(int)u->nobj);
  printf("objsize: %d\n",(int)u->objsize);
  printf("limit: %d\n",(int)u->limit);
  printf("sizeof(data): %d\n",(int)sizeof(u->data));
  printf("---------\n");
}

int main(){
  uba *u= newuba(sizeof(int), 1250);
  //printinfo(u);
  for(int i = 0;i<5000;i++) {
    // int *e = (int *)malloc(sizeof(int));
    // *e = i;
    // addelem(u, (elem *)e);
    // free(e);
    addelem(u,&i);
    //printinfo(u);
  }
  uba *u2= newuba(sizeof(int), 1250);
  //printinfo(u);
  for(int i = 0;i<5000;i++) {
    int *e = (int *)malloc(sizeof(int));
    *e = i;
    addelem(u2, (elem *)e);
    free(e);
    //printinfo(u);
  }
  printinfo(u2);
  printf("\n");

  addelems(u2,u->data,u->nobj);

  printinfo(u2);
  printf("\n");

  for(size_t i = 0;i<u2->nobj;i++)printf("%d, ",*(int *)getelem(u2,i));
  printf("\n");

  freeuba(u,NULL);
  freeuba(u2,NULL);
  return 0;
  
}