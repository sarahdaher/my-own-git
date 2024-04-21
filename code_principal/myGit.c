#include "exos.h"

int main(int argc, char *argv[]) {

  if (strcmp(argv[1], "init") == 0) {
    initRefs();
    initBranch();
  }

  if (strcmp(argv[1], "list-refs") == 0) {
    if (fileExists(".refs")) {
      List *l = listdir(".refs");
      printf("Refs : \n");
      Cell *c = *l;
      while (c) {
        if ((c->data)[0] != '.')
          printf("%s \n", c->data);
        c = c->next;
      }
      freeList(l);
    }
  }

  if (strcmp(argv[1], "create-ref") == 0) {
    if (argc == 4) {
      createUpdateRef(argv[2], argv[3]);
      createUpdateRef("HEAD", argv[3]); // il a donne name et hash
    } else {
      if (argc == 3) {
        createUpdateRef(argv[2], argv[3]);
        createUpdateRef("HEAD", NULL);
      } else
        printf("Vous devez taper au moins 2 arguments apres la commande "
               "'create-ref'.\n");
    }
  }

  if (strcmp(argv[1], "delete-ref") == 0) {
    if (argc == 3)
      deleteRef(argv[2]);
    if (argc == 2)
      printf("Entrez le nom de la reference que vous voulez supprimer\n");
  }

  if (strcmp(argv[1], "add") == 0) {
    if (argc == 2)
      printf("tapez le nom des fichiers ou repertoires a ajouter\n");
    else {
      for (int i = 2; i < argc; i++) {
        myGitAdd(argv[i]);
      }
    }
  }

  if (strcmp(argv[1], "list-add") == 0) {
    printf("Zone de préparation :\n");
    if (fileExists(".add")) {
      WorkTree *wt = ftwt(".add");
      printf("%s\n", wtts(wt));
      freeWorkTree(wt);
    }
  }

  if (strcmp(argv[1], "clear-add") == 0) {
    if(fileExists(".add")) system("rm .add");
  }

  if (strcmp(argv[1], "commit") == 0) {
    printf("argv[4] est %s\n", argv[4]);
    if ((argv[3] != NULL) && (argv[3][0] == '-') && (argv[3][1] == 'm')) {
      printf("msg %s\n", argv[4]);
      myGitCommit(argv[2], argv[4]); // on a un message
    } else
      myGitCommit(argv[2], NULL);
  }

  // exo10
  if (strcmp(argv[1], "get-current-branch") == 0) {
    char *b = getCurrentBranch();
    printf("%s\n", b);
    free(b);
  }

  if (strcmp(argv[1], "branch") == 0) {
    if (argc == 3) {
      if (!branchExists(argv[2]))
        createBranch(argv[2]);
      else
        printf("Erreur: La branche existe deja\n");
    }
    // printf("Erreur: il faut 1 argument apres la commande branch\n");
  }

  if (strcmp(argv[1], "branch-print") == 0) {
    if (argc != 3) {
      printf("Erreur: il faut 1 argument apres la commande branch-print\n");
    } else if (!branchExists(argv[2]))
      printf("Erreur: La branche n'existe pas!\n");
    else
      printBranch(argv[2]);
  }

  if (strcmp(argv[1], "checkout-branch") == 0) {
    if (argc != 3) {
      printf("Erreur: il faut 1 argument apres la commande checkout-branch\n");
    } else if (!branchExists(argv[2]))
      printf("Erreur: La branche n'existe pas!\n");
    else
      myGitCheckoutBranch(argv[2]);
  }

  if (strcmp(argv[1], "checkout-commit") == 0) {
    if (argc != 3) {
      printf("Erreur: il faut 1 argument apres la commande checkout-commit\n");
    } else
      myGitCheckoutCommit(argv[2]);
    // les msgs d erreur sont deja dans la fonction
  }

  if (strcmp(argv[1], "merge") == 0) {
    if (argc != 4) {
      printf("Erreur: il faut 2 arguments (le nom d'une branche et un message) apres la commande merge\n");
    } else {
      char *branch = argv[2];
      char *message = argv[3];
      char *curr_branch = getCurrentBranch();
      printf("cherchons les conflits\n");
      List *Liste_conflits = merge(branch, curr_branch);
      if (!Liste_conflits ||(Liste_conflits && *Liste_conflits == NULL)) {
        printf("La fusion s'est bien passée !\n");
        if(Liste_conflits) free(Liste_conflits);
        free(curr_branch);
      } else {
        printf("LA LISTE DES CONFLITS:\n");
        printList(Liste_conflits);
        
        printf("Veuillez choisir entre ces 3 options en tapant le numéro associé à chacune d'elles \nOption 1: Garder les fichiers de la branche courante et créer un commit de suppression de la branche %s ensuite appliquer merge.\n\nOption 2: Garder les fichiers de la branche %s et céeer un commit de suppression de la blanche courante ensuite appliquer merge\n\nOption 3: Choisissez conflit par conflit la branche sur laquelle vous souhaitez supprimer le conflit\n",branch, branch);
        
        int choix;
        scanf("%d", &choix);
        printf("Vous avez choisi : %d \n", choix);
        if (choix == 1) {
          createDeletionCommit(branch, Liste_conflits, message);
          printf("DELETION COMMIT créé\n");
          merge(branch, message);
          printf("Merge fait\n");
        }
        if (choix == 2) {
          createDeletionCommit(curr_branch, Liste_conflits, message);
          printf("DELETION COMMIT créé\n");
          merge(branch, message);
          printf("Merge fait\n");
        }
        if (choix == 3) {
          Cell *c = *Liste_conflits;
          List *list_curr = initList();
          List *list_branch = initList();
          Cell* tmp;
          while (c) {
            tmp = buildCell(c->data);
            printf("choisissez dans quelle branche vous souhaitez supprimer le conflit %s, tapez 1 pour la branche courante et taper 2 pour la branche %s ", c->data, branch );
            int choix_branch;
            scanf("%d", &choix_branch);
            if (choix_branch == 1) {
              insertFirst(list_curr, tmp);
            }
            if (choix_branch == 2) {
              insertFirst(list_branch, tmp);
            }
            c = c->next;
          }
          createDeletionCommit(curr_branch, list_curr, message);
          createDeletionCommit(branch, list_branch, message);
          merge(branch, message);

          freeList(list_curr);
          freeList(list_branch);
        }
        freeList(Liste_conflits);
        free(curr_branch);
      }
      
    }
  }

  if (strcmp(argv[1], "help") == 0) {
    printf("\n./myGit init : crée le répertoire caché .refs contenant HEAD et master et le fichier caché .current_branch.\n\n./myGit list-refs : affiche toutes les références dans .refs.\n\n./myGit create-ref <name> <hash> : crée la référence <name> qui pointe vers <hash>.\n\n./myGit delete-ref <name> : supprime la référence <name>.\n\n./myGit add <elem> [<elem2> <elem3> ...] : ajoute les elem dans .add(zone de préparation).\n\n./myGit list-add : affiche le contenu de .add.\n\n./myGit clear-add : supprime le contenu de .add.\n\n./myGit commit <branch_name> [-m <message>] : effectue un commit sur <branch_name> avec ou sans <message>.\n\n./myGit get-current-branch : affiche le nom de la branche courante.\n\n./myGit branch <branch-name> : crée la branche <branch-name> si elle n'existe pas déjà.\n\n./myGit branch-print <branch-name> : affiche les hashs (et messages si présent) des commits de <branch-name>.\n\n./myGit checkout-branch <branch-name> : réalise un déplacement sur <branch-name>.\n\n./myGit checkout-commit <pattern> : réalise un déplacement sur le commit qui commence par <pattern>.\n\n./myGit merge <branch-name> <message> : réalise une fusion du dernier commit de la branche courante avec celui de <branch-name> et ajoute la clé <message> sur ce commit de fusion.\n");
  }

  return 0;
}