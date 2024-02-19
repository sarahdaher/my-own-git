#include "exos.h"
//exo6

// q1
kvp *createKeyVal(char *key, char *val) {
  // allouer
  if(!val || !key){
    printf("erreur key ou val null dans create key val \n"); return NULL;
  }
  kvp *new = (kvp *)malloc(sizeof(kvp));
  // initialiser
  new->value = strdup(val);
  new->key = strdup(key);
  // retourne
  return new;
}

// libere la memoire de kvp
void freeKeyVal(kvp *kv) {
  if(kv==NULL) return;
  free(kv->key);
  free(kv->value);
  free(kv);
}

// q2
char *kvts(kvp *k) {
  // taille a allouer
  //int len = strlen(k->key) + strlen(k->value) + 4; // 2 espaces : et \0;
  int len = 1000;
  char *res = (char *)malloc(len * sizeof(char));
  strcpy(res, k->key);
  strcat(res, " :");
  strcat(res, k->value);
  return res;
}

// sens inverse
kvp *stkv(char *str) {
  char key[256], val[256];
  sscanf(str, "%s :%s", key, val);
  kvp * res = createKeyVal(key, val);
  return res;
}

// q3
Commit *initCommit() {
  // allouer commit
  Commit *commit = (Commit *)malloc(sizeof(Commit));
  commit->n = 0;
  commit->size = SIZE_COMMIT;

  // allouer pour T
  commit->T = (kvp **)malloc(sizeof(kvp *) * commit->size);

  for (int i = 0; i < commit->size; i++) {
    // initialiser chaque element de T
    commit->T[i] = NULL;
  }

  return commit;
}

// pour liberer la memoire
void freeCommit(Commit *c) {
  
  int i = 0;
  while (i < c->size) {
   if(c->T[i] != NULL) freeKeyVal(c->T[i]);
    i++;
  }
  free(c->T);
  free(c);
}

// q4
//  on a choisit la fonction suivante :

unsigned long hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; // hash * 33 + c

  return hash;
}

// q5 
void commitSet(Commit *c, char *key, char *value) {
  int i = 0;
  unsigned long h = hash((unsigned char *)key) % c->size;

  while (c->T[h] != NULL && i < c->size) {
    // tant que la case est occupee ou on a pas parcouru
    h = (h + 1) % c->size; // +1 car probing lineaire
    i++;
  }

  // si T est remplie on fait rien
  if (i == c->size) {
    printf("T est remplie ! \n");
    return;
  }

  // on cree notre element a sa place
  kvp* tmp = createKeyVal(key, value);
  c->n++;
  c->T[h] = tmp;
}

// q6
Commit *createCommit(char *hash) {
  Commit *commit = initCommit();
  commitSet(commit, "tree", hash);
  return commit;
}

// q7
char *commitGet(Commit *c, char *key) {

  int i = hash((unsigned char *)key) % c->size;
  int cpt = 0;

  while (c->T[i] != NULL && cpt < c->size) {

    // si on a trouve la cle
    if (strcmp(c->T[i]->key, key) == 0) {
      return c->T[i]->value;
    }
    i = (i + 1) % c->size;
    cpt++;
  }

  // sinon
  return NULL;
}

// q8
char *cts(Commit *c) {

  // allouer l'espace memoire suffisant
  int size = (c->n * 1000) + 1;
  char* str = (char *)malloc(size * sizeof(char));
  str[0] = '\0';

  for (int i = 0; i < c->size; i++) {

    if (c->T[i] != NULL) {

      char *kv_str = kvts(c->T[i]);
      strcat(str, kv_str);
      strcat(str, "\n");
      free(kv_str);
    }
  }
  return str;
}

// sens inverse
Commit *stc(char *ch) {
  Commit *c = initCommit();
  char *line = strtok(ch, "\n");

  while (line != NULL) {
    
    // on extrait la paire cle - valeur de chaque ligne
    
    kvp *kv = stkv(line);
    if (kv != NULL) {
      
      // On ajoute la paire cle valeur a Commit
      commitSet(c, kv->key, kv->value);
      freeKeyVal(kv);
    }
    line = strtok(NULL, "\n");
  }
  return c;
}

// q9
void ctf(Commit *c, char *file) {
  FILE *fw;
  fw = fopen(file, "w");

  if (fw==NULL) {
    printf("erreur ouverture cts de %s!", file);
    return;
  }

  for (int i = 0; i < c->size; i++) {
    if (c->T[i]) {
      char *str = kvts(c->T[i]);
      fprintf(fw, "%s\n", str);
      free(str);
    }
  }
  
  fclose(fw);
}

Commit *ftc(char *file) {
  FILE *f = fopen(file, "r");
  if (!f) {
    printf("Erreur (fonction ftc): Impossible d'ouvrir  %s\n", file);
    return NULL;
  }
  char buff[100]; // on stock dedans chaque ligne
  char s[1000];   // on stock dedans le tout pour apres le transformer en commit
  s[0] = '\0';
  while (fgets(buff, 100, f)) {
    strcat(s, buff);
  }
 
  fclose(f);
  Commit *c = stc(s);
  return c;
}

// 6.10
char *blobCommit(Commit *c) {
  char fname[100] = "/tmp/myfileXXXXXX";
  int fd = mkstemp(fname);
  ctf(c, fname);
  char *hash = sha256file(fname);
  char *ch = hashToFile(hash);
  strcat(ch, ".c");
  cp(ch, fname);
  free(ch);
  return hash;
}