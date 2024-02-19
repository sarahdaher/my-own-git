#include "exos.h"

// exercice 7

// 7.1
void initRefs() {
  if (!fileExists(".refs")) {
    system("mkdir .refs");
    system("touch .refs/HEAD");
    system("touch .refs/master");
  }
}

// 7.2

void createUpdateRef(char *ref_name, char *hash) {
  char refs_path[256];
  sprintf(refs_path, ".refs/%s", ref_name);

  // fichier n'existe pas
  List *l = listdir(".refs");
  if (searchList(l, ref_name) == NULL) {
    if(!fileExists(".refs")) system("mkdir .refs");
    char touch_command[256];
    sprintf(touch_command, "touch %s", refs_path);
    system(touch_command);
  }
  // si le hash n'est pas vide ou null on l'ecrit
  if (hash != NULL && strlen(hash) > 0) {
    char echo_command[256];
    sprintf(echo_command, "echo %s > %s", hash,
            refs_path); // echo permet d'ecrire hash dans refs_path en écrasant
                        // son contenu
    system(echo_command);
  }
  //createUpdateRef("HEAD", hash);
  freeList(l);
}

// 7.3
void deleteRef(char *ref_name) {
  List *l = listdir(".refs");

  // on cherche si ref_name est dans .refs
  if (searchList(l, ref_name) != NULL) {
    char buff[256];
    sprintf(buff, "rm .refs/%s ", ref_name);
    system(buff);
  }
  freeList(l);
}

// 7.4
char *getRef(char *ref_name) {

  List *l = listdir(".refs");
  if (searchList(l, ref_name) == NULL) {
    freeList(l);
    printf("erreur : %s n'est pas dans le repertoire .refs \n", ref_name);
    return NULL;
  }
  freeList(l);

  char *buff = (char *)malloc(sizeof(char) * 256);
  char path[1000];

  sprintf(path, ".refs/%s", ref_name);

  FILE *fr = fopen(path, "r");
  if (fgets(buff, 1000, fr) == NULL) {
    free(buff);
    fclose(fr);
    return ""; // fichier vide
  } else{
    fgets(buff, 1000, fr);
    fclose(fr);
    if (buff[strlen(buff) - 1] == '\n') buff[strlen(buff) - 1] = '\0';
    return buff;
  }
}

// 7.5
void myGitAdd(char *file_or_folder) {
  WorkTree *wt;

  // si .add existe pas, on le cree
  if (!fileExists(".add")) {
    char buff[100];
    sprintf(buff, "touch .add");
    system(buff);
    wt = initWorkTree();
  }

  // sinon on transforme .add en WorkTree
  else
    wt = ftwt(".add");

  // si file_or_folder existe on l'ajoute dans le WorkTree puis on ecrit la
  // chaine de caracteres dans le .add qui correspond au WorkTree modifié
  if (fileExists(file_or_folder)) {
    appendWorkTree(wt, file_or_folder, NULL,0); 
    wttf(wt, ".add");

  } else
    printf("%s n'existe pas!\n", file_or_folder);
  freeWorkTree(wt);
}

// 7.6
void myGitCommit(char *branch_name, char *message) {

  //-
  if (!fileExists(".refs")) {
    printf("Initialiser d'abord les references du projet.\n");
    return;
  }
  
  List *l = listdir(".refs");
  
  if (branch_name == NULL || searchList(l, branch_name) == NULL) {
    printf("La branche n'existe pas.\n");
    freeList(l);
    return;
  }
  
  char *branch_hash = getRef(branch_name);
  char *head_hash = getRef("HEAD");
  printf("Head pointe sur %s\n", head_hash);
  if (((branch_hash == NULL) && (head_hash != NULL)) ||
      ((branch_hash != NULL) && (head_hash == NULL))){ 
    return;
  }
  
  if (((branch_hash) && (head_hash))) {
    if (strcmp(branch_hash, head_hash) != 0) {
      printf("HEAD doit pointer sur le dernier commit de la branche %s.\n", branch_hash);
      freeList(l);
      return;
    }
  }

  //-
  
  if(!fileExists(".add")){
    char buff[100];
    sprintf(buff, "touch .add");
    system(buff);
  }
  WorkTree *wt = ftwt(".add");

  //-
  char *hashwt = saveWorkTree(wt, ".");
  
  //-
  Commit *c = createCommit(hashwt);

  //-
  // lecture deja faite dans getRef
  if (strlen(branch_hash) != 0){
    commitSet(c, "predecessor", branch_hash);
  }
  //-
  if (message != NULL){
    commitSet(c, "message", message);
  }
  //-
  char *hashCommit = blobCommit(c);
  //-
  createUpdateRef(branch_name, hashCommit);

  //-
  createUpdateRef("HEAD", hashCommit);
printf("myGitCommit s'est bien passée\n");
  
  // supprimer .add
  system("rm .add");
  
  free(hashwt);
  free(hashCommit);
  freeWorkTree(wt);
  freeCommit(c);
  freeList(l);
  if (branch_hash && strlen(branch_hash)>0) free(branch_hash);
  if (head_hash && strlen(head_hash)>0) free(head_hash);
  
  return;
}

// exercice 8

//8.1

void initBranch(){
  if (!fileExists(".current_branch")) {
    system("touch .current_branch");
  }
  char echo_command[256];
  sprintf(echo_command, "echo %s > %s","master",".current_branch");
  system(echo_command); 
}


//8.2

int branchExists(char* branch){
  
  //retourne 0 si n'existe pas et 1 sinon
  if(branch == NULL || strlen(branch)==0) return 0;
  List *l = listdir(".refs");
  if (searchList(l, branch) == NULL) {
    freeList(l);
    return 0;
  }
  freeList(l);
  return 1;
  
}

//8.3
void createBranch(char* branch){
  if(branch == NULL || strlen(branch)==0) return;
  
  //on recupere la reference pointee par HEAD
  char* hash = getRef("HEAD");
  
  //on cree une nouvelle reference branch qui pointe vers le meme commit
  createUpdateRef(branch, hash);
  if (hash && strlen(hash)>0) free(hash);
}

//8.4
char* getCurrentBranch(){
  
  FILE* f = fopen(".current_branch","r");
  if(f==NULL)printf("erreur d'ouverture de .current_branch \n");
  char* branch_name = malloc(sizeof(char)*1000);
  
  //on recupere le contenu de current branch (donc le nom de la branche courante)
  fscanf(f, "%s",branch_name);
  if(branch_name==NULL)printf("erreur dans la lecture de .current_branch \n");
  fclose(f);
  return branch_name;
}

//8.5
char* hashToPathCommit(char*hash){
  char * path = malloc(sizeof(char)*1000);
  char* tmp = hashToPath(hash);
  //if(tmp[strlen(tmp)-1] == '\n') tmp[strlen(tmp)-1] = '\0';
  sprintf(path, "%s.c", tmp);
  free(tmp);
  return path;
}

void printBranch(char* branch){
  if(!branch) return;
  
  //on recupere le hash du dernier commit
  char * commit_hash = getRef(branch);
  if ((commit_hash == NULL)||strlen(commit_hash)==0){
    //free(commit_hash);
    return;
  } 
  
  //on recupere le path associe
  char * commit_path =hashToPathCommit(commit_hash);
  
  //on cree le commit a partir de ce fichier

  if(!commit_path) {
    printf("path est null printbranch\n"); return;
  }
  Commit * c = ftc(commit_path);
  char* message;

  //on parcourt tous les commits
  while(c){
    message = commitGet(c,"message");
    if(message!=NULL) printf("%s -> %s \n",commit_hash, message);
    else printf("%s \n", commit_hash);
    
    if(commitGet(c, "predecessor")==NULL){//on termine
      break;
      
    }else{//on continue
      char *ref = commit_hash;
      commit_hash= commitGet(c, "predecessor");
      free(ref);

      char *path = commit_path;
      commit_path= hashToPathCommit(commit_hash);
      free(path);
      
      c =ftc(commit_path);
    
    }
  }
  freeCommit(c);
  free(commit_hash);
  free(commit_path);
}

//8.6
List *branchList(char *branch) {
  List* L =initList();
  char * commit_hash = getRef(branch);
  if (commit_hash == NULL){
    freeList(L);
    //free(commit_hash);
    return NULL;
  }
  else if (strlen(commit_hash)==0){
    //free(commit_hash);
    return L;
  }
  
  //on recupere le path associe
  char * commit_path =hashToPathCommit(commit_hash);
  Commit * c = ftc(commit_path);
 
  
  //on parcourt tous les commits
  while(c){
    insertFirst(L, buildCell(commit_hash));
    
    if(commitGet(c, "predecessor")==NULL){//on termine
      break;

    }else{//on continue
      char *ref = commit_hash;
      commit_hash= commitGet(c, "predecessor");
      free(ref);

      char *path = commit_path;
      commit_path= hashToPathCommit(commit_hash);
      free(path);
      if(c) freeCommit(c);
      c =ftc(commit_path);
       
    }
   
  }
  free(commit_hash);
  free(commit_path);  
  if(c) freeCommit(c);
  return L;
}

//8.7
List *getAllCommits(){
  
  List *L = initList();
  List* list = listdir(".refs");
  Cell *ptr = *list;
  
  while (ptr){ //on parcourt les références
    
    if (ptr->data[0]!='.'){
      List* list2 = branchList(ptr->data);
      Cell *cell = *list2;
      
      while (cell){
        if (!searchList(L, cell->data)) { //si le hash n'est pas deja présent, on l'ajoute
          Cell* cellule = buildCell(cell->data);
          insertFirst(L, cellule);
        }
        
        cell = cell->next;
      }
      
      freeList(list2);
    }
    ptr = ptr->next;
    
  }
  freeList(list);
  
  return L;
}

//9.1
void restoreCommit(char * hash_commit){
  char* path_commit = hashToPathCommit(hash_commit);

  //on cree le commit (dont on connait le hash)
  Commit* c = ftc(path_commit); 
  
  //on cherche le WorkTree associe a c ('tree' = wt)
  char* tree_path = (char*)malloc(1000*sizeof(char));
  char* hash = commitGet(c, "tree");

  char* path_sans_t = hashToPath(hash);
  sprintf(tree_path, "%s.t", path_sans_t);

  //on cree le wt (on a son path)
  WorkTree * wt = ftwt(tree_path);
  restoreWorkTree(wt, ".");

  
  free(path_commit);
  freeCommit(c);
  free(tree_path);
  free(path_sans_t);
  freeWorkTree(wt);
}

//9.2
void myGitCheckoutBranch(char* branch){
  //-
    FILE * f = fopen(".current_branch", "w");
    fprintf(f, "%s", branch);
    fclose(f);
  //-
    char* hash_commit =  getRef(branch);
    createUpdateRef("HEAD", hash_commit);
  //-
   if(hash_commit && strlen(hash_commit) >0) {
     restoreCommit(hash_commit);
     free(hash_commit);}
}


//9.3
List* filterList(List* L, char* pattern){
  
  if (pattern == NULL) {
    printf("PATTERN NULL\n"); return L;}
  
  int len_pattern = strlen(pattern);
  Cell* c = *L;
  Cell* c2;
  List* List_filtre = initList();
  
  while(c){
    if (strncmp(pattern, c->data, len_pattern) == 0){ //commence par pattern, on l'ajoute a la liste
      c2 = buildCell(c->data);
      insertFirst(List_filtre, c2);
      
    }
    c = c->next;
  }
  
  return List_filtre;
}

//9.4
void myGitCheckoutCommit(char* pattern){
  List* L = getAllCommits();
  List* L_filtered = filterList(L, pattern);

  Cell* c = *L_filtered;

  //si il ne reste aucun hash!
  if(c == NULL){
    printf("ERREUR plus aucun hash apres le filtre !\n");
    return;
  }

  //si il reste un seul hash
  if(c->next == NULL){
    Cell* c = listGet(L_filtered, 0);
    char * commit_hash = c->data ;
    createUpdateRef ("HEAD", commit_hash) ;
    restoreCommit (commit_hash) ;
    freeList(L_filtered);
    freeList(L);
    return;
  }
  //si on a plusieurs hash on les affiche
  char* str = ltos(L_filtered);
  printf("%s\n", str);
  
  //on demande dans ce cas a l utilisateur de preciser sa requete
  printf("Il y a plusieurs hash avec ce pattern. Merci de preciser votre requete.\n");
  freeList(L_filtered);
  freeList(L);
  free(str);
  return;
}
