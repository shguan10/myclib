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
  printinfo(u);
  for(int i = 0;i<5000;i++) {
    int *e = (int *)malloc(sizeof(int));
    *e = i;
    addelem(u, (elem *)e);
    free(e);
    printinfo(u);
  }
  printf("\n");
  for(size_t i = 0;i<u->nobj;i++)printf("%d, ",*(int *)getelem(u,i));

  freeuba(u,NULL);
  return 0;
  
}