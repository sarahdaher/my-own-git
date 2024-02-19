#include "exos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  //test des fonctions de l'exo 1 
  // Q.1 : system("ls");
  // Q.2 : cat main.c| sha256sum >file.tmp
  // Q.3 : hashFile("main.c", "dest.txt");
  
  char *hash = sha256file("myGit.c");
  printf("%s\n", hash);
  free(hash);
  
  // test des fonctions de l'exo 2
  List *L = initList();

  Cell *c1 = buildCell("chaine1");
  Cell *c2 = buildCell("chaine2");
  Cell *c3 = buildCell("chaine3");

  insertFirst(L, c3);
  insertFirst(L, c2);
  insertFirst(L, c1);

  printf("c1 : %s, c2: %s, c3: %s \n", ctos(c1), ctos(c2), ctos(c3));
  char *resL = ltos(L);
  printf("Affichage de L: %s\n", resL);

  Cell *c = listGet(L, 1);
  printf("cell 1 : %s\n", ctos(c));

  Cell *c4 = searchList(L, "chaine2");
  if (c4 != NULL)
    printf("on a trouvé %s\n", ctos(c4));

  List *L2 = stol("chaine1|chaine2|chaine3");
  char *resL2 = ltos(L2);
  printf(" Affichage de L2 : %s\n", resL2);
  ltof(L, "writeL.txt");
  printf("On a ecrit L dans le fichier writeL.txt \n");

  List *L3 = ftol("writeL.txt");
  printf("Lecture du fichier et stockage dans L3\n");
  char *resL3 = ltos(L3);
  printf("Affichage de L3: %s\n", resL3);

  printf("on libère les mémoires allouées\n");

  free(resL);
  free(resL2);
  free(resL3);

  freeList(L);
  freeList(L2);
  freeList(L3);

  //exo3
  
  printf("%d\n", fileExists("not_exists"));
  printf("%d\n", fileExists("file_exists"));
  char *ch = hashToPath("22bhgjdlsoo9iendjsoqonc\n");
  printf("res est : %s", ch);
  List *l = listdir("dossier1");
  char *s = ltos(l);
  printf("les fichiers contenus dans le repertoire dossier1 sont : %s\n", s);
  cp("test.txt", "list.txt");
  blobFile("writeL.txt");
  
  free(s);
  free(ch);
  freeList(l);

  return 0;
}
