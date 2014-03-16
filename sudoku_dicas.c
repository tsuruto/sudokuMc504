#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define N 18
 
volatile int interesse[N];
volatile int ultimo[N-1];

typedef struct {
  char valores[9];
  char val;
}dica;

dica *matriz[9][9];

void* f_linha(void *v){
  int linha = *(int*)v;
  int i,j,k;
  char vetor[9];
  char val[1];
  int l = 0;

  for(i=0;i<N-1;i++){
    interesse[linha] = i;
    ultimo[i] = linha;
    for(k=0;k < N && ultimo[i] == linha;k++)
      if(k != linha)
        while(i <= interesse[k] && ultimo[i] == linha);
  }

  for(j=0;j<9;j++){
    if(matriz[linha][j]->val != 'X')
    {
      val[0] = matriz[linha][j]->val;
      vetor[l] = val[0];
      l++;
    }
  }
  for(j=0;j<9;j++){
    if(matriz[linha][j]->val == 'X')
      strcat(matriz[linha][j]->valores,vetor);
  }

  interesse[linha] = -1;
  return (void*) 0;
}

void* f_coluna(void *v){
  int coluna = *(int*)v;
  int linha;
  int i,j,k;
  char vetor[9];
  char val[1];
  
  linha = coluna+9;

  for(j=0;j<N-1;j++){
    interesse[linha] = j;
    ultimo[j] = linha;
    for(k=0;k < N && ultimo[j] == linha;k++)
      if(k != linha)
        while(j <= interesse[k] && ultimo[j] == linha);
  }

  for(i=0;i<9;i++){
    if(matriz[i][coluna]->val != 'X')
    {
      val[0] = matriz[i][coluna]->val;
      strcat(vetor,val);
    }
  }
  for(i=0;i<9;i++){
    if(matriz[i][coluna]->val == 'X')
      strcat(matriz[i][coluna]->valores,vetor);
  }

  interesse[linha] = -1;
  return (void*) 0;
}

int main() {
  FILE *fmatriz;
  char name[80];
  char read;
  char base[9] = "123456789";
  char *pch;
  pthread_t linha[9], coluna[9];
  int i,j,retorno;
  int *p_id;
  
  for(i=0;i<18;i++)
    interesse[i] = -1;

  printf("Digite o nome do arquivo .txt que contem a matriz para ser analisada: ");
  scanf("%79s",name);
  strcat(name,".txt");
  fmatriz = fopen(name,"r");
  if(fmatriz==NULL)
  {
    printf("Erro: arquivo não encontrado!");
    exit(EXIT_FAILURE);
  }

  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      matriz[i][j] = (dica*)malloc(sizeof(dica));
    }
  }

  for(i=0;i<9;i++){
    for(j=0;j<9;j++){
      read=fgetc(fmatriz);
      matriz[i][j]->val=read;
      matriz[i][j]->valores[0] = '\0';
      read=fgetc(fmatriz);
    }
  }
  
  for(i=0;i<9;i++){
    p_id = (int*)malloc(sizeof(int));
    *p_id = i;
    pthread_create(&linha[i], NULL, f_linha, (void*) p_id);
  }
  
  for(j=0;j<9;j++){
    p_id = (int*)malloc(sizeof(int));
    *p_id = j;
    pthread_create(&coluna[j], NULL, f_coluna, (void*) p_id);
  }
  
  for(i=0;i<9;i++){
    pthread_join(linha[i],(void**) &retorno);
    if(retorno != 0)
    {
      printf("A thread da linha %d apresentou erro.\n",i+1);
      exit(EXIT_FAILURE);
    }
  }
  
  for(j=0;j<9;j++){
    pthread_join(coluna[j],(void**) &retorno);
    if(retorno != 0)
    {
      printf("A thread da coluna %d apresentou erro.\n",i+1);
      exit(EXIT_FAILURE);
    }
  }
  
  for(i=0;i<9;i++){
    if(i != 0)
      printf("\n");
    for(j=0;j<9;j++){
      if(j != 0)
        printf(" ");
      if(matriz[i][j]->val != 'X'){
        printf("%c",matriz[i][j]->val);
      }else{
        pch = strtok(base,matriz[i][j]->valores);
        printf("(");
        while(pch != NULL){
          printf("%s",pch);
          pch = strtok(NULL,matriz[i][j]->valores);
        }
        printf(")");
      }
    }
  }

  return 0;
}
  
  
    
    
  
  
