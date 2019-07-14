#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/sem.h>
#include <errno.h>
#include <sys/types.h>

int main (int argc, char* argv[])
{
    int n = atoi(argv[1]), nbElementMatrice;
    int i = atoi(argv[2]);
    int j = atoi(argv[3]);
    int a, b;
    int* tabI = NULL, *tabJ = NULL;
    int *matrice = NULL;
    int cIJ = 0;
    
    int shmidMatrice;
    key_t keyMatrice;
    
    nbElementMatrice = n * n;
    
    
    printf("Processus c[%d,%d]: %d\n", i, j, getpid());
    
    tabI = malloc(n * sizeof(int));
    
    if (tabI == NULL) {
        printf("Processus c[%d,%d]: Impossible d'allouer de memoire\n", i, j);
        exit(EXIT_FAILURE);
    }
    
    tabJ = malloc(n * sizeof(int));
    
    if (tabJ == NULL) {
        printf("Impossible d'allouer de memoire\n");
        free(tabI);
        exit(EXIT_FAILURE);
    }
    
    keyMatrice = ftok("shmMatrice", 19);
    shmidMatrice = shmget(keyMatrice, 2 * nbElementMatrice * sizeof(int), 0666);
    matrice = (int*) shmat(shmidMatrice,(void*)0,0);
    
    if (matrice == NULL) {
        printf("Processus c[%d,%d]: Impossible de lire la memoire partagee\n", i, j);
        free(tabI);
        free(tabJ);
        exit(EXIT_FAILURE);
    }
    
//Copie des données a calculer c[i,j]
    for (a = 0; a < n; a++) {
        tabI[a] = matrice[i*n + a];
        tabJ[a] = matrice[j+ n * a];
    }
    
//Affichage des c[i,j] 
    printf("c[i]: ");
    for (a = 0; a < n-1; a++) printf("%d, ", tabI[a]);
    printf("%d", tabI[a]);
    putchar('\n');
    printf("c[j]: ");
    for (a = 0; a < n-1; a++) printf("%d, ", tabJ[a]);
    printf("%d", tabJ[a]);
    printf("\n\n");
    
//Calcul du et enregistrement du c[i,j]
    matrice[i*n + j + nbElementMatrice] = 0;
    for (a = 0; a < n; a++) {
        matrice[i*n + j + nbElementMatrice] += tabI[a] * tabJ[a];  
    }
    printf("%d %d\n", i*n+j, matrice[i*n + j + nbElementMatrice]);
       
    shmdt(matrice);
    
    free(tabI);
    free(tabJ);
    
    return 0;
}
