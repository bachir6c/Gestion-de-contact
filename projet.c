#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter un contact
#define MAX_CONTACTS 100

struct Contact {
    char nom[50];
    char prenom[50];
    char numero[10];
};

// Fonction pour ajouter un contact
void ajouterContact(struct Contact contacts[], int *nbContacts) {
    FILE *file = fopen("fichiercontact.txt", "a");
    if (file == NULL) {
        printf("\033[31mErreur lors de l'ouverture du fichier.\033[0m\n"); // Texte rouge
        return;
    }

    if (*nbContacts < MAX_CONTACTS) {
        struct Contact nouveauContact;
        int numeroInt;

        printf("Nom: ");
        scanf("%s", nouveauContact.nom);
        fprintf(file, "Nom: %s, ", nouveauContact.nom);

        printf("Prénom: ");
        scanf("%s", nouveauContact.prenom);
        fprintf(file, "Prenom: %s, ", nouveauContact.prenom);

        do {
            printf("Numéro de téléphone: ");
            scanf("%s", nouveauContact.numero);
            numeroInt = atoi(nouveauContact.numero);
            if (numeroInt < 761000000 || numeroInt > 789999999) {
                printf("\033[33mSaisissez un vrai numéro Sénégalais\n Qui doit commencer par 76, 77 ou 78 suivis de 7 chiffres\033[0m\n"); // Texte jaune
            }
        } while (numeroInt < 761000000||numeroInt > 789999999);

        fprintf(file, "Numero: %s\n", nouveauContact.numero);
        contacts[*nbContacts] = nouveauContact;
        (*nbContacts)++;
        printf("\033[32mContact ajouté avec succès.\033[0m\n"); // Texte vert
        fclose(file);
    } else {
        printf("\033[31mLa liste de contacts est pleine.\033[0m\n"); // Texte rouge
    }
}

// Fonction pour afficher les contacts
void afficherContacts(struct Contact contacts[]) {
    FILE *file = fopen("fichiercontact.txt", "r");
    if (file == NULL) {
        printf("\033[31mErreur lors de l'ouverture du fichier.\033[0m\n"); // Texte rouge
        return;
    }

    // Vérifie si le fichier est vide
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == 0) {
        printf("\033[33mIl n'y a pas de contacts à afficher.\033[0m\n"); // Texte jaune
        fclose(file);
        return;
    }

    // Remet le curseur au début du fichier pour la lecture
    fseek(file, 0, SEEK_SET);

    char c;
    while ((c = fgetc(file)) != EOF) {
        printf("%c", c);
    }
    fclose(file);
}

// Fonction pour rechercher un contact
void rechercherContact(struct Contact contacts[], int nbContacts, char recherche[50]) {
    FILE *file = fopen("fichiercontact.txt", "r");
    if (file == NULL) {
        printf("\033[31mErreur lors de l'ouverture du fichier.\033[0m\n"); // Texte rouge
        return;
    }

    char ligne[256];
    int ligne_trouver = 0;

    while (fgets(ligne, sizeof(ligne), file)) {
        if (strstr(ligne, recherche) != NULL) {
            printf("%s", ligne);
            ligne_trouver = 1;
        }
    }

    if (!ligne_trouver) {
        printf("\033[33mAucune ligne contenant '%s' n'a été trouvée dans le fichier contact.\033[0m\n", recherche); // Texte jaune
    }

    fclose(file);
}

// Fonction pour supprimer un contact
void supprimerContact(struct Contact contacts[], int *nbContacts, char nomSuppression[50]) {
    FILE *file = fopen("fichiercontact.txt", "r");
    FILE *temp_file = fopen("fichiercontact_temp.txt", "w");
    if (file == NULL || temp_file == NULL) {
        printf("\033[31mErreur lors de l'ouverture des fichiers.\033[0m\n"); // Texte rouge
        return;
    }

    char ligne[256];
    int doublon = 0;

    while (fgets(ligne, sizeof(ligne), file)) {
        if (strstr(ligne, nomSuppression) != NULL) {
            char nomLigne[50];
            sscanf(ligne, "Nom: %[^,],", nomLigne);
            if (strcmp(nomLigne, nomSuppression) == 0) {
                doublon = 1;
                continue;
            }
        }
        fputs(ligne, temp_file);
    }

    fclose(file);
    fclose(temp_file);

    if (doublon) {
        if (remove("fichiercontact.txt") != 0) {
            perror("Erreur de suppression de l'ancien fichier");
            exit(EXIT_FAILURE);
        }

        if (rename("fichiercontact_temp.txt", "fichiercontact.txt") != 0) {
            perror("Erreur de renommage du fichier temporaire");
            exit(EXIT_FAILURE);
        }

        printf("\033[32mLigne contenant '%s' supprimée avec succès du fichier fichiercontact.txt.\033[0m\n", nomSuppression); // Texte vert
        (*nbContacts)--;
    } else {
        remove("fichiercontact_temp.txt");
        printf("\033[33mAucune ligne contenant '%s' n'a été trouvée dans le fichier fichiercontact.txt.\033[0m\n", nomSuppression); // Texte jaune
    }
}

// Fonction pour afficher le menu et gérer les choix de l'utilisateur
void afficherMenu(struct Contact contacts[], int *nbContacts) {
    int choix;
    int continuer;

    do {
        printf("\n\033[34m========================================\033[0m\n"); // Bordure bleue
        printf("\033[34m|               Menu                   |\033[0m\n"); // Texte bleu
        printf("\033[34m========================================\033[0m\n");
        printf("\033[34m| 1. Ajouter un contact                |\033[0m\n");
        printf("\033[34m| 2. Afficher les contacts             |\033[0m\n");
        printf("\033[34m| 3. Rechercher un contact par nom     |\033[0m\n");
        printf("\033[34m| 4. Supprimer un contact par nom      |\033[0m\n");
        printf("\033[34m| 5. Création de groupe                |\033[0m\n");
        printf("\033[34m| 6. Affichage de groupe                |\033[0m\n");
        printf("\033[34m| 7. Quitter                           |\033[0m\n");
        printf("\033[34m========================================\033[0m\n");
        printf("Votre choix: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterContact(contacts, nbContacts);
                break;
            case 2:
                afficherContacts(contacts);
                break;
            case 3: {
                char recherche[50];
                printf("Rechercher: ");
                scanf("%s", recherche);
                rechercherContact(contacts, *nbContacts, recherche);
                break;
            }
            case 4: {
                char nomSuppression[50];
                printf("Nom à supprimer: ");
                scanf("%s", nomSuppression);
                supprimerContact(contacts, nbContacts, nomSuppression);
                break;
            }
            case 5: {
                 
                char groupname[50];
                int nb_de_contact;
                char nom_contact[50];
                printf("entrer le nom du groupe !\n");
                scanf("%s", groupname);
                printf("entrer le nombre de contact à ajouter !\n");
                scanf("%d", &nb_de_contact);
                printf("\n voici la liste des contacts");
                afficherContacts(contacts);
                for(int i=0;i<nb_de_contact; i++){
                   printf("\n entrer le nom du contact num %d : ", i+1);
                   scanf("%s", nom_contact);
                   groupe(contacts, groupname, nom_contact);
                }
                break;
                  
            }
            case 6 :
            {   char *groupename[50];
                affichergroupe(groupename[50]);
            }
            case 7:
                printf("Au revoir !\n");
                return;
            default:
                printf("\033[31mChoix invalide. Veuillez choisir à nouveau.\033[0m\n"); // Texte rouge
        }

        printf("\n\033[34mVoulez-vous faire autre chose ?\033[0m\n\033[34m1. Oui\033[0m\n\033[34m2. Non\033[0m\nVotre choix: ");
        scanf("%d", &continuer);

        if (continuer == 2) {
            printf("\033[32mAu revoir, à la prochaine !\033[0m\n"); // Texte vert
        }
    } while (continuer == 1);
}

void groupe(struct Contact contacts[], char *groupname[50], char name[50]){

    FILE *file = fopen("fichiercontact.txt", "r");


    FILE *temp_file = fopen("fichiercontact_grp.txt", "a");

    char copi[256];
    int doublon = 0;
    const char *src = ", Groupe : ";

    const char *fin = "\n";
    while (fgets(copi, sizeof(copi), file)) {
            size_t len = strlen(copi);
            if (len > 0 && copi[len - 1] == '\n') {
                copi[len - 1] = '\0';
            }

        if (strstr(copi, name) != NULL) {

            strcat(copi,src);
            strcat(copi,groupname);
            strcat(copi,fin);
            fputs(copi, temp_file);

        }


    }

    fclose(file);
    fclose(temp_file);



}
void affichergroupe(char *groupname[50])
{
   FILE *file = fopen("fichiercontact.txt", "r");
   FILE *temp1_file = fopen("fichiercontact_grp.txt", "a");
   char copie[256];
   while (fgets(copie, sizeof(copie), file)) {
     if(strstr(copie, groupname) != NULL) 
     {
       printf("%s", copie);
     }
   }fputs(copie, temp1_file);
    fclose(file);
    if (temp1_file == NULL) {
        printf("\033[31mErreur lors de l'ouverture du fichier.\033[0m\n"); // Texte rouge
        return;
    }

    // Vérifie si le fichier est vide
    fseek(temp1_file, 0, SEEK_END);
    long size = ftell(temp1_file);
    if (size == 0) {
        printf("\033[33mIl n'y a pas de contacts à afficher.\033[0m\n"); // Texte jaune
        fclose(temp1_file);
        return;
    }

    // Remet le curseur au début du fichier pour la lecture
    fseek(file, 0, SEEK_SET);
    fopen("fichiercontact_grp.txt", "a");
    char d;
    while ((d = fgetc(temp1_file)) != EOF) {
        printf("%c", d);
    }
    fclose(temp1_file);
    
}
int main() {
    char motDePasseCorrect[] = "cps2024";
    char motDePasse[100];
    int tentative = 0;
    struct Contact contacts[MAX_CONTACTS];
    int nbContacts = 0;

    do {
        printf("Saisir le mot de passe: ");
        scanf("%s", motDePasse);
        tentative++;
    } while (strcmp(motDePasseCorrect, motDePasse) != 0 && tentative < 5);

    if (strcmp(motDePasseCorrect, motDePasse) == 0 && tentative < 5) {
        afficherMenu(contacts, &nbContacts);
    } else {
        printf("\033[31mAccès refusé.\033[0m\n"); // Texte rouge
    }

   return 0;
}