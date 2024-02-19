#include "exos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


//exo 1

// Q.3 :
int hashFile(char *source, char *dest) {
  char buff[300];
  sprintf(buff, "(cat %s | sha256sum)>%s", source, dest);
  system(buff);
  return 0;
}

// Q.4
char *sha256file(char *file) {

  static char template[] = "/tmp/myfileXXXXXX";
  char fname[1000];
  strcpy(fname, template);
  int fd = mkstemp(fname);
  
  hashFile(file, fname);
  char *buffer = malloc(sizeof(char) * 1000);
  FILE *f = fopen(fname, "r");
  fgets(buffer, sizeof(char) * 1000, f);
  buffer[strlen(buffer) - 4] = '\0';
  
  close(fd);
  fclose(f);
  
  return buffer;
}

// exo 2

// q1
List *initList() {
  List *L = (List *)malloc(sizeof(List));
  *L = NULL;
  
  return L;
}

// q2
Cell *buildCell(char *ch) {
  
  Cell *c = (Cell *)malloc(sizeof(Cell));
  c->data = strdup(ch);
  c->next = NULL;
  
  return c;
}

// q3
void insertFirst(List *L, Cell *C) {
  
  C->next = *L;
  *L = C;
  
}

// q4
char *ctos(Cell *c) {
  return c->data;

}
char *ltos(List *L) {
  if (L == NULL)
    return "\0";
  
  int len = 0;
  Cell *curr = *L;
  
  while (curr) {
    len += strlen(curr->data) + 1; // +1 pour '|'
    curr = curr->next;
  }
  
  len++; // pour \0
  char *res = (char *)malloc(sizeof(char) * len);
  
  strcpy(res, "");

  curr = *L;
  while (curr != NULL) {
    strcat(res, ctos(curr));
    strcat(res, "|"); // ajouter '|'
    curr = curr->next;
  }
  
  res[strlen(res) - 1] = '\0'; // supp le dernier '|'
  
  return res;
}

// q5
Cell *listGet(List *L, int i) { //retourne l'element de L d'indice i
  if (L == NULL) return NULL;
  
  Cell *curr = *L;
  int cpt = 1;
  
  while (curr && cpt < i) {
    cpt++;
    curr = curr->next;
  }
  
  return curr;
}

// q6
Cell *searchList(List *L, char *str) { //retourne l'element de L de meme nom que str
  
  if(!L) return NULL;
  
  Cell *curr = *L;
  while (curr) {
    if (strcmp(ctos(curr), str) == 0)
      return curr;
    curr = curr->next;
  }
  
  return NULL;
}

List *stol(char *s) {
  char *tmp = strdup(s);
  List *L = initList();
  char *ch = strtok(tmp, "|");
  
  while (ch) {

    Cell *c = buildCell(ch);
    insertFirst(L, c);
    ch = strtok(NULL, "|");
  }
  
  free(tmp);
  return L;
}

// q7
void ltof(List *L, char *path) {
  
  FILE *f = fopen(path, "w");
  if(f==NULL) return;
  
  Cell *curr = *L;
  while (curr) {
    fprintf(f, "%s\n", ctos(curr));
    curr = curr->next;
  }
  
  fclose(f);
}

List *ftol(char *path) {
  
  FILE *f = fopen(path, "r");
  char buff[100];
  List *lres = initList();
  
  if(f == NULL) return NULL;
  
  Cell *c;
  while (fgets(buff, 100, f)) {
    buff[strlen(buff) - 1] = '\0';
    c = buildCell(buff);
    insertFirst(lres, c);
  }
  
  fclose(f);
  
  return lres;
}

// fonction pour libérer la mémoire
void freeList(List *l) { 
  
  Cell *curr = *l;
  while (curr) {
    Cell *next = curr->next;
    free(curr->data);
    free(curr);
    curr = next;
  }
  
  free(l);
}

void printList(List* L){
  if(L==NULL) printf("Liste vide !\n");
  Cell *current = *L;
  while(current){
    printf("%s -> ", current->data);
    current = current->next;
  }
}

// exo3
int getChmod(const char *path) {
  struct stat ret;

  if (stat(path, &ret) == -1) {
    return -1;
  }
  return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) |
         (ret.st_mode & S_IXUSR) | /*
owner*/
         (ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) |
         (ret.st_mode & S_IXGRP) | /*
group*/
         (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) |
         (ret.st_mode & S_IXOTH); /*
other*/
}


void setMode(int mode, char *path) { // on a remplacé %d par %o par souci d'incohérence
  
  char buff[100];
  sprintf(buff, "chmod %o %s ", mode, path);
  system(buff);
}

// q1
List *listdir(char *root_dir) { //la liste des fichiers ou repertoires  dans root_dir
  
  List *L = initList();
  
  DIR *dp;
  struct dirent *ep;
  Cell *C;
  
  if ((dp = opendir(root_dir)) != NULL) {
    
    while ((ep = readdir(dp)) != NULL) {
      C = buildCell(ep->d_name);
      insertFirst(L, C); 
    }
    
    closedir(dp);
  }
  
  else printf("Erreur ouverture de %s\n", root_dir);
  
  return L;
}

// q2
int fileExists(char *file) { //dans le repertoire courant uniquement 
  List *L = listdir(".");  // "." repertoire courant
  Cell *curr = *L;
  
  while (curr) {
    if (strcmp(curr->data, file) == 0) {
      freeList(L);
      return 1; //retourne 1 si file existe 
    }
    curr = curr->next;
  }
  
  freeList(L);
  return 0; //0 sinon
}

// q3
void cp(char *to, char *from) { //copie le contenu de to dans from

  FILE *fr = fopen(from, "r");

  if (fr == NULL) {
    printf("erreur ouverture dans cp from: %s!\n", from);
    return;
  }
  
  FILE *fw = fopen(to, "w");
  if (fw == NULL) {
    printf("erreur ouverture dans cp to: %s !\n", to);
    return;
  }
  int mode = getChmod(from);
  setMode(mode, to);
  
  char buffer[1000];
  while (fgets(buffer, 1000, fr)) {
    fprintf(fw, "%s", buffer);
  }
  
  fclose(fr);
  fclose(fw);
}


// q4
char *hashToPath(char *hash) {
  //char *path = (char *)malloc(sizeof(char) * (strlen(hash) + 2));
  char *path = (char *)malloc(sizeof(char) * 1000);
  snprintf(path, strlen(hash) + 2, "%c%c/%s", hash[0], hash[1], hash + 2);
  
  return path;
}

//q5
void blobFile(char *file) {
  char *tmp = sha256file(file);
  char *hashPath = hashToPath(tmp);
  char *commande = malloc(1000 * sizeof(char));

  char rep[3]; //on crée le repertoire associé aux 2 premiers caracteres du path
  rep[0] = hashPath[0];
  rep[1] = hashPath[1];
  rep[2] = '\0';

  if (!fileExists(rep)) {
    sprintf(commande, "mkdir %c%c", hashPath[0], hashPath[1]);
    system(commande);
  }
  
  free(commande);
  
  cp(hashPath, file); //on copie dans hashPath le contenu de file
  
  free(tmp);
  free(hashPath);
}

