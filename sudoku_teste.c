#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char matriz[9][9];
pthread_mutex_t lock;

void* f_linha(void *i){
  int linha = *(int*)i;
  int end = 0;
  int count = end+1;
  while(end < 8)
  {
    pthread_mutex_lock(&lock);
    if(matriz[linha][end] == matriz[linha][count])
    {
      pthread_mutex_unlock(&lock);
      return (void*) end;
    }
    pthread_mutex_unlock(&lock);
    count++;
    if(count==9){
      end++;
      count = end+1;
    }
  }
  return (void*) 0;
}
void* f_coluna(void *j){
  int coluna = *(int*)j;
  int count=0,end=0;
  while(end < 8)
  {
    pthread_mutex_lock(&lock);
    if(matriz[end][coluna] == matriz[count][coluna])
    {
      pthread_mutex_unlock(&lock);
      return (void*) end;
    }
    pthread_mutex_unlock(&lock);
    count++;
    if(count==9){
      end++;
      count = end+1;
    }
  }
  return (void*) 0;
}

int main() {
  FILE *fmatriz;
  char name[80];
  char read;
  pthread_t linha[9], coluna[9];
  int i,j,retorno;
  int *p_id;
  
  if (pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("mutex init failed!\n");
    exit(EXIT_FAILURE);
  }
  

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
      read=fgetc(fmatriz);
      matriz[i][j]=read;
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
      printf("A linha %d contem duas ocorrencias do numero %c.\n",i+1,matriz[i][retorno]);
      exit(EXIT_FAILURE);
    }
  }
  
  for(j=0;j<9;j++){
    pthread_join(coluna[j],(void**) &retorno);
    if(retorno != 0)
    {
      printf("A coluna %d contem duas ocorrencias do numero %c.\n",j+1,matriz[retorno][j]);
      exit(EXIT_FAILURE);
    }
  }
  
  printf("Sudoku em perfeita condicao, sem erros!\n");
  return 0;
}
  
  
    
    
  
  
