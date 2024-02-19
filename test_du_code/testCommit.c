#include "exos.h"

int main(void) {
  // Création d'un Commit vide
  Commit *c1 = initCommit();

  // Définition des propriétés du Commit
  commitSet(c1, "author", "John Doe");
  commitSet(c1, "message", "Initial commit");
  commitSet(c1, "date", "2023-04-02");

  // Création d'une clé/valeur à partir d'une chaîne de caractères
  char *str = "key : value";
  kvp *kv = stkv(str);
  printf("Key: %s\n", kv->key);
  printf("Value: %s\n", kv->value);

  ctf(c1, "commit1");

  // Création d'un nouveau Commit à partir du fichier enregistré
  Commit *c2 = ftc("commit1");
  ctf(c2, "commit2");
  
  // Affichage des propriétés du nouveau Commit
  printf("Author: %s\n", commitGet(c2, "author"));
  printf("Message: %s\n", commitGet(c2, "message"));
  printf("Date: %s\n", commitGet(c2, "date"));

  // Création d'un Commit à partir d'un hash
  char *hash = "f1b5be2b5f61d17a2252498dc68d53a5";
  Commit *c3 = createCommit(hash);
  printf("Tree: %s\n", commitGet(c3, "tree"));

  // Création d'un enregistrement instantané d'un Commit
  char *blob = blobCommit(c2);
  printf("Blob: %s\n", blob);
  // Enregistrement du Commit dans un fichier
  FILE *fp = fopen("commit1", "r");
if (fp != NULL) {
  fclose(fp);
  printf("Le commit a été enregistré dans le fichier commit1.\n");
} else {
  printf("Impossible d'enregistrer le commit dans le fichier commit1.\n");
}

  // Nettoyage de la mémoire
  freeKeyVal(kv);
  freeCommit(c1);
  freeCommit(c2);
  freeCommit(c3);
  free(blob);
  
  return 0;
}

