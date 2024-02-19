#include <stdio.h>
#include "exos.h"

int main() {

  //le fichier myGit.c teste les autres fonctions des exercices 
  
  // test de la fonction initBranch
  printf("Test initBranch\n");
  initBranch();

  // test de la fonction branchExists
  printf("Test branchExists\n");
  printf("master exists: %d\n", branchExists("master"));
  printf("unknown exists: %d\n", branchExists("unknown"));

  // test de la fonction createBranch
  printf("Test createBranch\n");
  createBranch("new_branch");
  printf("new_branch exists: %d\n",                
  branchExists("new_branch"));

  // test de la fonction getCurrentBranch
  printf("Test getCurrentBranch\n");
  char* s = getCurrentBranch();
  printf("Current branch: %s\n", s);

  // test de la fonction printBranch
  printf("Test printBranch\n");
  printBranch("HEAD");

  //test de la fonction branchList
  printf("Test branchList\n");
  List* L = branchList("HEAD");
  printList(L);
  freeList(L);

  // test de la fonction getAllCommits
  printf("Test getAllCommits\n");
  List* L1 = getAllCommits();
  printList(L1);
  freeList(L1);

  free(s);
  return 0;
}
