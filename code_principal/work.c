#include "exos.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define TAILLE 100



// q4.1
WorkFile *createWorkFile(char *name) {
  WorkFile *new = (WorkFile *)malloc(sizeof(WorkFile));
  new->name = strdup(name);
  new->hash = NULL;
  new->mode = 0;
  return new;
}

// pour liberer le wf

void freeWorkFile(WorkFile *wf) {
  if (wf == NULL)
    return;
  free(wf->name);
  free(wf->hash);
  free(wf);
}

// q4.2

char *wfts(WorkFile *wf) {
  if (wf == NULL) {
    return "";
  }
  char temp[1000];
  sprintf(temp, "%s\t%s\t%o%c", wf->name, wf->hash, wf->mode, '\0');
  char *res = strdup(temp);
  return res;
}

// q4.3
WorkFile *stwf(char *ch) {
  char buff[100];
  char buff2[100];
  int buff3;
  sscanf(ch, "%s\t%s\t%o", buff, buff2, &buff3);
  WorkFile *res = createWorkFile(buff);
  res->hash = strdup(buff2);
  res->mode = buff3;
  return res;
}

// q4.4
WorkTree *initWorkTree() {
  WorkTree *new = (WorkTree *)malloc(sizeof(WorkTree));
  new->tab = (WorkFile *)malloc(sizeof(WorkFile) * TAILLE);
  new->size = TAILLE;
  new->n = 0;
  return new;
}

// pour liberer la memoire
void freeWorkTree(WorkTree *wt) {
  if (wt == NULL)
    return;

  if (wt->tab) {
    WorkFile *tab = wt->tab;
    for (int i = 0; i < wt->n; i++) {
      free(tab[i].name);
      free(tab[i].hash);
    
    }
    free(wt->tab);
  }
  free(wt);
}

// q4.5
int inWorkTree(WorkTree *wt, char *name) {
  if (wt == NULL) return -1;
  WorkFile *tab = wt->tab;
  for (int i = 0; i < (wt->n); i++) {
    if (strcmp(name, (tab[i]).name) == 0) {
      return i;
    }
  }
  return -1;
}

// q4.6
int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode) {
  // retourne 0 si il etait deja present, -1 si il n'a pas pu etre ajouté (par manque d'espace) et 1 si tout s'est bien passé
  if (inWorkTree(wt, name) != -1)
    return 0;
  
  if ((wt->n) >= (wt->size)) //capacite max depasse en cas d'ajout !!
    return -1;
  
  wt->tab[wt->n].name = strdup(name);
  
  if (hash) wt->tab[wt->n]. hash = strdup(hash);
  else wt->tab[wt->n]. hash = NULL;
  
  wt->tab[wt->n].mode = mode;
  wt->n += 1; // car on a ajoute un element

  return 1;
}

// q4.7
char *wtts(WorkTree *wt) { //WorkTree to string
  WorkFile *tab = wt->tab;
  char *res;
  res = malloc(1000);
  strcpy(res, "");
  
  for (int i = 0; i < (wt->n); i++) {
    char *tmp = wfts(&tab[i]);
    strcat(res, tmp);
    strcat(res, "\n");
    free(tmp);
  }
  
  return res;
}

// q4.8

WorkTree *stwt(char *ch) { // ch de la forme name\thash\tmode\n

  WorkTree *wt = initWorkTree();

  char *name = strtok(ch, "\t");
  char *hash = strtok(NULL, "\t");
  char *mode = strtok(NULL, "\n");
  
  while (name && hash && mode && wt->n < wt->size) {
    if (appendWorkTree(wt, name, hash, atoi(mode)) != 1) {
      printf("Erreur : impossible d'ajouter %s.\n", name);
      freeWorkTree(wt);
      return NULL;
    }

    name = strtok(NULL, "\t");
    hash = strtok(NULL, "\t");
    mode = strtok(NULL, "\n");
  }

  return wt;
}


// q4.9
int wttf(WorkTree *wt, char *file) {
  // retourne 0 si wt est NULL, -1 en cas d'erreur d'ouverture et 1 sinon
  FILE *fw;
  
  if (wt == NULL)
    return 0;
  
  if ((fw = fopen(file, "w")) == NULL)
    return -1;
  
  char *wts = wtts(wt);
  fprintf(fw, "%s", wts);
  
  free(wts);
  fclose(fw);
  
  return 1;
}

// affichage
void afficherWorkFile(WorkFile *wf) {
  if (wf == NULL)
    return;
  char *mot = malloc(sizeof(wf->hash));
  if (!(wf->hash))
    strcpy(mot, wf->hash);
  else
    mot = "null";
  printf("name : %s, hash : %s\n", wf->name, wf->hash);
}

void afficherWorkTree(WorkTree *wt) {
  if (wt == NULL)
    return;

  if (wt->tab) {
    for (int i = 0; i < wt->n; i++) {
      afficherWorkFile(&(wt->tab[i]));
    }
  }
}

// q4.10
WorkTree *ftwt(char *file) { //file to worktree
  FILE *fr;
  fr = fopen(file, "r");
  
  if (fr == NULL) {
    printf("Erreur d'ouverture ftwt!\n");
    return NULL;
  }
  
  WorkTree *wt = initWorkTree();
  char buf[256];
  
  while (fgets(buf, 256, fr) != NULL) { //on lit ligne par ligne (chaucune represente un string de WorkFile) -> on transforme en WorkFile
    WorkFile *wf = stwf(buf);
    if (wf != NULL) {
      appendWorkTree(wt, wf->name, wf->hash, wf->mode);
      freeWorkFile(wf);
    }
  }
  
  fclose(fr);
  
  return wt;
}


char *hashToFile(char *hash) { //cree un repertoire avec les 2 premieres lettres et renvoie le chemin associe au hash
  char *ch2 = strdup(hash);
  ch2[2] = '\0';
  
  struct stat st;
  if (stat(ch2, &st) == -1) { //si ch n'existe pas 
    mkdir(ch2, 0700);
  }
  
  free(ch2);
  return hashToPath(hash);
}

char *blobWorkTree(WorkTree *wt) {
  char fname[100] = "/tmp/myfileXXXXXX";
  mkstemp(fname); // crée un fichier temporaire
  wttf(wt, fname); // y stocke le contenu de wt
  
  char *hash = sha256file(fname); // hash son contenu
  char *ch = hashToFile(hash); // crée le répertoire
  
  strcat(ch, ".t");
  cp(ch, fname); //copie le contenu stocke dans fname dans ch
  
  setMode(0777, ch); //donner les droits d'accés
  free(ch);
  
  return hash;
}

int isDirectory(const char *path) { //renvoie 1 si path correspond a un repertoire
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

char *concat_paths(char *path1, char *path2) {
  char *result = malloc(strlen(path1) + strlen(path2) + 2);
  strcpy(result, path1);
  
  if (strcmp(path1, "") != 0) strcat(result, "/");
  strcat(result, path2);
  
  return result;
}

char *saveWorkTree(WorkTree *wt, char *path) {
  for (int i = 0; i < wt->n; i++) {

    char *absPath = concat_paths(path, wt->tab[i].name);
    if (isDirectory(absPath) == 1) { // repertoire

      WorkTree *newWT = initWorkTree();
      List *L = listdir(absPath);
      Cell *tmp = *L; //on parcourt le contenu du repertoire qu'on va ajouter a newWt surlequel on va faire l'appel recursif 
      while (tmp) {
        if ((tmp->data)[0] != '.')
          appendWorkTree(newWT, tmp->data, NULL, 0);
        tmp = tmp->next;
      }
      if ((wt->tab[i]).hash) free((wt->tab[i]).hash);
      (wt->tab[i]).mode = getChmod(absPath);
      (wt->tab[i]).hash = saveWorkTree(newWT, absPath);
      
      freeWorkTree(newWT);
      freeList(L);
    } 
    else { // fichier

      (wt->tab[i]).mode = getChmod(absPath);
      if ((wt->tab[i]).hash) free((wt->tab[i]).hash);
      (wt->tab[i]).hash = sha256file(absPath);
      blobFile(absPath); //on cree l'enregistrement du fichier
    }
    
    free(absPath);
  }
  
  return blobWorkTree(wt);
}

int isWorkTree(char *path) { //teste si path correspond a WorkTree
  char rep[4];
  sprintf(rep, "%c%c%c", path[0], path[1], '\0');
  if (fileExists(rep)) {
    List *l = listdir(rep);
    if (searchList(l, &path[3]) != NULL) {
      freeList(l);
      return 0;
    } else if (searchList(l, strcat(path, ".t")) == NULL) {
      freeList(l);
      return 1;
    }
    freeList(l);
  }
  return -1;
}
// q5.3

// if (access(refs_path, F_OK) != 0) fichier n'existe pas, access > fileExists qui elle marche uniquement sur le repertoire courant

void restoreWorkTree(WorkTree *wt, char *path) {
  char commande[200];
  sprintf(commande, "mkdir -p %s", path); 
  system(commande);

  for (int i = 0; i < wt->n; i++) {
    char *hash_path = hashToPath(wt->tab[i].hash);
    char *buff = concat_paths(path, wt->tab[i].name);
    
    if (access(hash_path, F_OK) == 0) { //si on peut ouvrir -> pas de .t -> fichier
      cp(buff, hash_path);
    } 
    else { //repertoire ->worktree
      strcat(hash_path, ".t");
      WorkTree *new_wt = ftwt(hash_path);
      restoreWorkTree(new_wt, wt->tab[i].name); //appel reccursif
      freeWorkTree(new_wt);
    }
    
    free(buff);
    free(hash_path);
  }
}
