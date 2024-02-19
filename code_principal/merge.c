#include "exos.h"

// 11.1
WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts) {

  WorkTree *merge_res = initWorkTree();

  for (int i = 0; i < (wt1->n); i++) {

    int test_wt2 = inWorkTree(wt2, (wt1->tab[i]).name);

    // si le nom est dans wt1 et si ils n'ont pas le mm contenu -> conflit 
    if ((test_wt2 != -1) &&(strcmp((wt2->tab[test_wt2]).hash, (wt1->tab[i]).hash) != 0)) {
      Cell *c = buildCell((wt1->tab[i]).name);
      insertFirst(*conflicts, c);
    }
    // sinon pas de conflit
    else appendWorkTree(merge_res, (wt1->tab[i]).name,(wt1->tab[i]).hash,(wt1->tab[i]).mode);
  }

  // on refait la meme chose pour wt2

  for (int i = 0; i < (wt2->n); i++) {

    int test_wt1 = inWorkTree(wt1, (wt2->tab[i]).name);

    // si le nom est dans wt2  : si ils n'ont pas le mm contenu -> conflit deja ajouté a la liste
    //                           sinon il a deja été ajouté au worktree
    if (test_wt1 != -1) { // insertion deja faite je pense !!!
      continue;
    }
    // sinon pas de conflit 
    else
      appendWorkTree(merge_res, (wt2->tab[i]).name, (wt2->tab[i]).hash,(wt2->tab[i]).mode);
  }
  return merge_res;
}


// 11.2

WorkTree *branchToWorkTree(char *branch) {

  char *hashCommit = getRef(branch);
  if(hashCommit == NULL) printf("hashCommit de %s NULL \n", branch);
  char *pathCommit = hashToPathCommit(hashCommit);
  Commit *commit = ftc(pathCommit); //commit associe a la branche
  
  char *pathTree = (char *)malloc(1000 * sizeof(char));
  char *hashTree = commitGet(commit, "tree"); //acceder au Worktree
  char *path_sans_t_Tree = hashToPath(hashTree);
  sprintf(pathTree, "%s.t", path_sans_t_Tree);
  WorkTree *wt = ftwt(pathTree); 

  free(pathTree);
  if(hashCommit && strlen(hashCommit)>0) free(hashCommit);
  free(pathCommit);
  free(path_sans_t_Tree);
  freeCommit(commit);

  return wt;
}

List *merge(char *remote_branch, char *message) {
  // fusionner la branche courante avec celle donnee en parametre

  char *current_branch = getCurrentBranch();

  WorkTree *wt_curr = branchToWorkTree(current_branch);
  if (!wt_curr)
    printf("worktree current branch NULL\n"); // pour verifier

  WorkTree *wt_rem = branchToWorkTree(remote_branch);
  if (!wt_rem)
    printf("worktree remote branch NULL\n"); // pour verifier

  // apres avoir cree wt_curr et wt_rem
  List *conflicts = initList();

  WorkTree *merged_wts = mergeWorkTrees(wt_curr, wt_rem, &conflicts);
  printf("LA LISTE DES CONFLITS \n");
  printList(conflicts);
  
  if (*conflicts == NULL) { 

    char *new_hash_wt = blobWorkTree(merged_wts); 
    
    char *current_hash = getRef(current_branch);
    char *remote_hash = getRef(remote_branch);

    Commit *new_commit = createCommit(new_hash_wt);
    commitSet(new_commit, "predecessor", current_hash);
    commitSet(new_commit, "merged_predecessor", remote_hash);
    if (message != NULL) commitSet(new_commit, "message", message);
 

    char *new_hash_commit = blobCommit(new_commit);

    createUpdateRef(current_branch, new_hash_commit);
    createUpdateRef("HEAD", new_hash_commit);
    printf("deleteRef(dans merge remote_branch) %s\n" , remote_branch);
    deleteRef(remote_branch);
  
    restoreWorkTree(merged_wts, ".");

    if(current_hash && strlen(current_hash)>0) free(current_hash);
    if(remote_hash && strlen(remote_hash)>0) free(remote_hash);
    free(new_hash_commit);
    free(new_hash_wt);
    if(conflicts) freeList(conflicts);
    freeCommit(new_commit);
  }

  free(current_branch);
  freeWorkTree(wt_rem);
  freeWorkTree(wt_curr);
  freeWorkTree(merged_wts);

  return conflicts;
}

// 11.3
void createDeletionCommit(char *branch, List *conflicts, char *message){

  char *branch_depart = getCurrentBranch();

  myGitCheckoutBranch(branch); // on se deplace sur la branche passee en parametre

  WorkTree *wt_current = branchToWorkTree(branch); // on recupere le worktree associé au dernier commit de cette branche

  if(fileExists(".add")) system("rm .add"); // on vide la zone de preparation

  for (int i = 0; i < (wt_current->n);i++) { // on parcourt les fichiers/repertoires du worktree

    if (searchList(conflicts, (wt_current->tab[i]).name) ==NULL)
    {
      // si ce n'est pas un conflit on l'ajoute a la zone de preparation
      myGitAdd((wt_current->tab[i]).name);
    }
  }
  myGitCommit(branch, message); // on crée le commit de supression

  myGitCheckoutBranch(branch_depart); // retour a la branche de depart
  
  free(branch_depart);
  freeWorkTree(wt_current);

  return;
}

