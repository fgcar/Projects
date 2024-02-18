//Author: Filipe Gonzaga de Carvalho
//Purpose: Este programa tem como objetivo organizar os horários diários de 
//estudos feitos por Filipe Gonzaga, de modo que se registra-rá o nome do 
//da disciplina, o dia do estudo, hora de início, hora do fim.
//Copyright © 2023

#include <stdio.h>   // Para usar entradas e saídas   
#include <stdlib.h>  // Para usar alocação dinâmica
#include <string.h>

typedef struct Subject_reg {

   int Delta;
   char Book[100],
        Date[9],
        Start[9],
        End[9],
        Select;
   unsigned long ID;

} Subject;

Subject * aloca(int Size) {

   Subject * Vector_Subject;
   Vector_Subject = malloc(Size * sizeof(Subject));

   if(Vector_Subject != NULL)
      return Vector_Subject;
   else {
      printf("Falha na alocação de memória.\n");
      exit(0);
   }
}

int Time_Variation(char * Inicio, char * Fim) {

   int hini,
       mini,
       hfim,
       mfim;
   char hora[6],
        HH[3],
        MM[3];
   char *Start,
        *End,
        *Pinicio,
        *Pfim,
        *hhmm;

   // hora inicial
   Start = strtok(Inicio, " "); // Separa HH:MM de PM/AM
   hhmm = Start;  // recebe HH:MM
   Start = strtok(NULL, " "); // recebe AM/PM
   Pinicio = Start; // recebe AM/PM
   strcpy(hora, hhmm);  // Copia de ponteiro para vetor

   Start = strtok(hhmm, ":"); // Separa HH de HH:MM
   strcpy(HH, Start);   // Coloca string horas em HH
   hini = atoi(HH);  // transforma string horas em inteiro
   Start = strtok(NULL, ":"); // Recebe string minutos
   strcpy(MM, Start);   // coloca string minutos em MM
   mini = atoi(MM);  // Transforma string minutos em inteiro

   // Repete o processo para a hora final
   End = strtok(Fim, " ");
   hhmm = End;
   End = strtok(NULL, " ");
   Pfim = End;
   strcpy(hora, hhmm);

   End = strtok(hhmm, ":");
   strcpy(HH, End);
   hfim = atoi(HH);
   End = strtok(NULL, ":");
   strcpy(MM, End);
   mfim = atoi(MM);

   int saldo;
   if(strcmp(Pinicio, Pfim) == 0) {
      //printf("Mesmo periodo.\n");
      saldo = (((hfim - hini) * 60 ) + (mfim - mini));
   }
   else {
      //printf("Periodo distinto.\n");
      if(strcmp(Pinicio, Pfim) != 0) {
         if(hfim == 12)
            saldo = (((hfim - hini) * 60) + (mfim - mini));
         else
            saldo = ((((hfim + 12) - hini) * 60) + (mfim - mini));
      }
   }

   return saldo;
}

unsigned long hash_djb2(const char *str) {
   unsigned long hash = 5381; // Valor aleatório
   int c;                      

   // Itera sore cada caractere da string
   while((c = *str++)) {
      hash = ((hash << 5 ) + hash) + c; // hash * 33 + c
   }

   return hash;
}

void Write_Subject(Subject * Vet, int Size) {

   int Count = 0;
   char Inicio[9],
        Fim[9], 
        inputString[100];
   
   system("clear");
   do{
      printf("Cadastro da %dª disciplina: \n", Count + 1);
      printf("Data (dd/mm/aa): ");
      scanf(" %8s", Vet[Count].Date);
      printf("Disciplina: ");
      scanf(" %[^\n]", Vet[Count].Book);
      printf("Início (hh:mm AM/PM): ");
      scanf(" %[^\n]", Vet[Count].Start);
      printf("Fim (hh:mm AM/PM): ");
      scanf(" %[^\n]", Vet[Count].End);

      strcpy(Inicio, Vet[Count].Start);
      strcpy(Fim, Vet[Count].End);
      
      // Removendo expaço de string para geração de ID
      strcpy(inputString, Vet[Count].Book);
      size_t len = strlen(inputString);
      if(len > 0 && inputString[len - 1] == '\n') {
         inputString[len - 1] = '\0';
      }
      Vet[Count].ID = hash_djb2(inputString);
      Vet[Count].Delta = Time_Variation(Inicio, Fim);
      
      Count++;
      printf("\n");
  
   } while(Count < Size);

}

void Print_Suject(Subject * Vet, int Size) {

   printf("| %-30s | %-9s |", "Disciplina", "Data");
   printf(" %-9s | %-9s | %-9s |\n", "Inicio", "Fim", "Delta");
   printf("----------------------------------------");
   printf("--------------------------------------\n");
   
   for(int i = 0; i < Size; i++) {     
    
      printf("| %-30s | %-9s |", Vet[i].Book, Vet[i].Date);
      printf(" %-9s | %-9s |", Vet[i].Start, Vet[i].End);
        
      printf(" %-9.02d |\n", Vet[i].Delta);
   }

}

void Create_File(Subject * Vet, int Size) {

   FILE * fp; 
            
   if( !(fp = fopen("planilha.ods", "a")) ) {
      printf("Erro ao abrir a planilha!\n");
      exit(0);
   }
   
   int size = ftell(fp);   // Obtem o tamanho do arquivo
   if(size == 0) {   // Verifica se o arquivo está vazio
      fprintf(fp, "%s,", "ID");
      fprintf(fp, "%s,%s,%s,", "Data", "Discplina", "Início");
      fprintf(fp, "%s,%s\n", "Fim", "Saldo (min)");
   }

   for(int i = 0; i < Size; i++) {        
      fprintf(fp, ",%s,%s,%s,", Vet[i].Date, Vet[i].Book, Vet[i].Start);
      fprintf(fp, "%s,%.02d\n", Vet[i].End, Vet[i].Delta);
   }

   fclose(fp);
   printf("Cadastro feito com Sucesso.\n");
       
}

void Create_Banco(Subject * Vet, int Size) {
   
   FILE * fpb;

   if( !(fpb = fopen("banco.txt", "a")) ) {
      printf("Erro ao abrir o banco de horas!\n");
      exit(0);
   }
   
   int size = ftell(fpb);
   if(size == 0) 
      fprintf(fpb, "%s,%s\n", "ID", "Saldo de Horas");

   for(int i = 0; i < Size; i++) {
      fprintf(fpb, "%lu,%.02d\n", Vet[i].ID, Vet[i].Delta);
   }

   fclose(fpb);
   printf("Banco gerado com Sucesso.\n");
}

int main() {
   int saldo, 
       Size;
   Subject * Vector_Subject;

   printf("Quantas disciplinas deseja cadastrar: ");
   scanf("%d", &Size);

   Vector_Subject = aloca(Size);

   Write_Subject(Vector_Subject, Size);

   Print_Suject(Vector_Subject, Size);

   Create_File(Vector_Subject, Size);
   
   Create_Banco(Vector_Subject, Size);

   free(Vector_Subject);
   return 0;
}
