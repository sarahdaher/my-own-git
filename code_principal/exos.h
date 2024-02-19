#ifndef EXOS_H
#define EXOS_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define TAILLE 100
#define SIZE_COMMIT 200

typedef struct key_value_pair {
  char *key;
  char *value;
} kvp;

typedef struct hash_table {
  kvp **T;
  int n;
  int size;
} HashTable;

typedef HashTable Commit;

typedef struct cell {
  char *data;
  struct cell *next;
} Cell;
typedef Cell *List;

typedef struct {
  char *name;
  char *hash;
  int mode;
} WorkFile;

typedef struct {
  WorkFile *tab;
  int size;
  int n;
} WorkTree;

// exo1
//  Q.3 :
int hashFile(char *source, char *dest);

// Q.4 :
char *sha256file(char *file);

// exo 2

// q1
List *initList();

// q2
Cell *buildCell(char *ch);

// q3
void insertFirst(List *L, Cell *C);

// q4
char *ctos(Cell *c);
char *ltos(List *L);

// q5
Cell *listGet(List *L, int i);

// q6
Cell *searchList(List *L, char *str);
List *stol(char *s);

// q7
void ltof(List *L, char *path);
List *ftol(char *path);

// fonction pour libérer la mémoire : utile dans le main
void freeList(List *l);

void printList(List *L);
// exo3

// q1
List *listdir(char *root_dir);

// q2
int fileExists(char *file);

// q3
void cp(char *to, char *from);

// q4
char *hashToPath(char *hash);

// q5
void blobFile(char *file);

// exo4

int getChmod(const char *path);

void setMode(int mode, char *path);

// q4.1
WorkFile *createWorkFile(char *name);

// q4.2
char *wfts(WorkFile *wf);

// q4.3
WorkFile *stwf(char *ch);

// q4.4
WorkTree *initWorkTree();

// q4.5
int inWorkTree(WorkTree *wt, char *name);

// q4.6
int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode);

// q4.7
char *wtts(WorkTree *wt);

// q4.8
WorkTree *stwt(char *ch);

// q4.9
int wttf(WorkTree *wt, char *file);

// q4.10
WorkTree *ftwt(char *file);

// liberer memoire
void freeWorkTree(WorkTree *wt);

// affichage
void afficherWorkFile(WorkFile *wf);
void afficherWorkTree(WorkTree *wt);

// exo5
char *hashToFile(char *hash);
char *blobWorkTree(WorkTree *wt);
int isDirectory(const char *path);
char *concat_paths(char *path1, char *path2);
char *saveWorkTree(WorkTree *wt, char *path);
void restoreWorkTree(WorkTree *wt, char *path);

// exo6
kvp *createKeyVal(char *key, char *val);
void freeKeyVal(kvp *kv);
char *kvts(kvp *k);
kvp *stkv(char *str);
Commit *initCommit();
unsigned long hash(unsigned char *str);
void commitSet(Commit *c, char *key, char *value);
Commit *createCommit(char *hash);
char *commitGet(Commit *c, char *key);
void freeCommit(Commit *c);
char *cts(Commit *c);
Commit *stc(char *ch);
void ctf(Commit *c, char *file);
Commit *ftc(char *file);
char *blobCommit(Commit *c);

// exo7
void initRefs();
void createUpdateRef(char *ref_name, char *hash);
void deleteRef(char *ref_name);
char *getRef(char *ref_name);
void myGitAdd(char *file_or_folder);
void myGitCommit(char *branch_name, char *message);

// exercice 8

// 8.1
void initBranch();
void initBranch2();
// 8.2
int branchExists(char *branch);
// 8.3
void createBranch(char *branch);
// 8.4
char *getCurrentBranch();
// 8.5
char *hashToPathCommit(char *hash);
void printBranch(char *branch);
// 8.6
List *branchList(char *branch);
// 8.7
List *getAllCommits();

// exo9
void restoreCommit(char *hash_commit);
void myGitCheckoutBranch(char *branch);
List *filterList(List *L, char *pattern);
void myGitCheckoutCommit(char *pattern);

// exo11
WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts);
WorkTree *branchToWorkTree(char *branch);
List *merge(char *remote_branch, char *message);
void createDeletionCommit(char *branch, List *conflicts, char *message);

#endif