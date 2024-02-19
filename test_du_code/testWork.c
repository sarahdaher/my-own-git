#include "exos.h"

// char* blobWorkTree(WorkTree* wt);
int main() {
  // 4.4
  WorkTree *wt = initWorkTree();

  if (inWorkTree(wt, "maissa") == -1)
    printf("maissa n'est pas present dans wt\n");
  else
    printf("maissa est present dans wt\n");

  if (appendWorkTree(wt, "sarah", "hashsarah", 777) == 1)
    printf("Ajout de sarah effectue\n");
  else
    printf("Ajout de sarah non effectue\n");

  if (appendWorkTree(wt, "sarah", "hashsarah", 777) == 1)
    printf("Ajout de sarah effectue\n");
  else
    printf("Ajout de sarah non effectue\n");

  if (inWorkTree(wt, "sarah") == -1)
    printf("sarah n'est pas present dans wt\n");
  else
    printf("sarah est present dans wt a la position %d\n", inWorkTree(wt, "sarah"));

  appendWorkTree(wt, "fichier1.txt", "hash1", 777);
  appendWorkTree(wt, "fichier2.txt", "hash2", 777);
  appendWorkTree(wt, "dossier1", NULL, 777);

  char *wts = wtts(wt);
  printf("chaine :\n%s\n", wts);
  WorkTree *wt2 = stwt(wts);
  char *wts2 = wtts(wt2);
  printf("chaine 2\n%s\n", wts2);
  wttf(wt, "testexercice4.txt");
  WorkTree *wt3 = ftwt("testexercice4.txt");
  char *wts3 = wtts(wt3);
  printf("chaine3\n%s\n", wts3);

  char *s = blobWorkTree(wt);
  printf("%s \n", s);

  char *test = saveWorkTree(wt, ".");
  FILE* fw = fopen("fichier1.txt", "w");
	fprintf(fw, "je ne suis plus fichier1\n");
  fclose(fw);
  restoreWorkTree(wt, ".");
  printf("%s test\n", test);
  freeWorkTree(wt);
  freeWorkTree(wt2);
  freeWorkTree(wt3);
  free(wts);
  free(wts2);
  free(wts3);
  free(test);
  free(s);
  return 0;
}
