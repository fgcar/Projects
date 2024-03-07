// Author: Filipe Gonzaga de Carvalho
// Purpose: Extimador da poupança do Banco do Brasil
// Compile: gcc poupancaBB.c -o poupancaBB -lm 
// Copyright © 2024

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct submissionsReg {
   double send;
} submissions;

typedef struct savingsReg {
   int amountSubmissions,
       months;
   double selic,
          investment,
          remuneration, 
          inflation;
   submissions * sends;
} savings;

savings * allocate(int months) {
   savings * create;
   create = malloc(sizeof(savings));

   if(create == NULL) {
      printf("Falha ao alocar memória!\n");
      exit(0);
   }

   create->months = months;
   
   create->sends = malloc(months * sizeof(submissions));
   if(create->sends == NULL) {
      printf("Falha ao alocar memória interna!\n");
      free(create);
      exit(0);
   }

   return create;
}

int menu() {
   int months;
   
   remake:
   system("clear");
   printf("***************************************************************\n"); 
   printf("Este programa estima a poupança do Banco do Brasil.\n");
   printf("***************************************************************\n");
   printf("Quantos meses deseja estimar: ");
   scanf("%d", &months);
   
   if (months > 0) {
      return months;   
   }
   else 
      goto remake;
}

void start(savings *s, int amountSubmissions) {

   printf("***************************************************************\n");
   printf("\t\t%s\n\n", "-> INFORMAÇÕES PARA ANÁLISE <-");
   printf("\t%s", "Investimento Inicial: R$ ");
   scanf("%lf", &s->investment);
   printf("\t%s", "Meta selic anual (%): ");
   scanf("%lf", &s->selic);
   printf("\t%s", "Rendimento mensal (%) : ");
   scanf("%lf", &s->remuneration);
   printf("\t%s", "Inflação atual (%): ");
   scanf("%lf", &s->inflation);
   printf("\t%s", "Quantos aportes mensais: ");
   scanf("%d", &s->amountSubmissions);
   if(s->amountSubmissions > s->months)
      printf("\t%s\n", "Warning: Nª Aportes/mês supera meses de extimados.");
   printf("\n\n\t\t%s\n", "-> INFORMAÇÕES PARA ANÁLISE <-");

}

double calculationBasis(const savings *s) {
    double tx;

    if (s->selic > 8.5) {
        return (s->remuneration - s->inflation) / 100;
    } else {
        tx = pow((1.0 + (s->selic / 100)), (1.0 / 12)) - 1.0;
        return ((0.7 * tx) + s->remuneration);
    }
}

void contributions(savings *s) {
   if(s->amountSubmissions == 0) {
      for(int i = 0; i < s->months; i++) 
         s->sends[i].send = 0.0;
   }
   else {
      printf("************************************************************\n");
      printf("Digite os seus aportes:\n");
      printf("************************************************************\n");
      for(int j = 0; j < s->amountSubmissions; j++) {
         printf("\tValor %dª aporte R$ ", j + 1);
         scanf("%lf", &s->sends[j].send);
      }
   }
}

void print(savings * s, double basis) {
   double upstream, 
          sends;
   char $[] = "R$";
   system("clear");
   printf("***************************************************************\n");
   printf("\t\t%s\n", "-> INFORMAÇÕES CONSIDERADAS NA ANÁLISE <-");
   printf("***************************************************************\n");
   printf("\t%s R$ %.02lf\n", "Capital Investido: ", s->investment);
   printf("\t%s %.02lf %\n", "Meta selic anual: ", s->selic);
   printf("\t%s %.04lf %\n", "Rendimento mensal: ", s->remuneration);
   printf("\t%s %.02lf %\n", "Inflação atual: ", s->inflation);
   printf("\t%s %d\n", "Nª Aportes: ", s->amountSubmissions);
   if(s->amountSubmissions > s->months)
      printf("\t%s\n", "Warning: Nª Aportes/mês supera meses de extimados.\n");
   printf("***************************************************************\n");
   printf("\n");
   printf("***************************************************************\n");
   printf("\t\t%s\n", "-> EXTIMATIVA POUPANÇA BB <-");
   printf("***************************************************************\n");

   printf(" %2s  %23s %28s\n", "Mês", "Conta", "Aportes");
   for(int months = 0; months < s->months; months++) {
         
      upstream = s->investment * pow(1.0 + basis, months);
      sends = s->sends[months].send;
      printf(" %2d  %18s %lf %18s %.02lf\n", months + 1, $, upstream, $, sends);
      s->investment += s->sends[months].send;
   }
}

void freeSavings(savings *s) {
   free(s->sends);
   free(s);
}

int main() {
   savings * simulation;
   int months;
   double basis;

   months = menu();
   simulation = allocate(months);
   start(simulation, months);
   contributions(simulation);
   basis = calculationBasis(simulation);
   print(simulation, basis);
   
   freeSavings(simulation);
   return 0;
}
