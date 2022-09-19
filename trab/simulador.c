#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulador.h"

int main()
{
    Processos *ptmp, *h_cpu = NULL, *h_entrada_saida = NULL;

    printf("Simulador com que simula o tempo de execução dos processos com 3 algoritmos diferes, esse simulador apresenta os dados a partir de calculos\n");

    char procss[MAX];
    printf("Digite a entrada (ex: A, B, a, A , b, A): ");
    scanf("%[^\n]", procss);

    int i, aux1 = 1, aux2 = 1, tipo, id, surto, priori;
    for (i = 0; i < strlen(procss); i++)
    {
        tipo = 0;
        if (procss[i] == 'A')
            id = aux1++, surto = 1, priori = 1, tipo = 1;
        else if (procss[i] == 'B')
            id = aux1++, surto = 2, priori = 2, tipo = 1;
        else if (procss[i] == 'a')
            id = aux2++, surto = 1, priori = 1, tipo = 2;
        else if (procss[i] == 'b')
            id = aux2++, surto = 2, priori = 2, tipo = 2;
        if (tipo == 1)
            insere_lista(&h_cpu, id, surto, priori);
        else if (tipo == 2)
            insere_lista(&h_entrada_saida, id, surto, priori);
    }

    printf("----CPU----\n");
    if (h_cpu == NULL)
    {
        printf("Sem processos\n");
    }
    else
    {
        listprocs(h_cpu);
        fifo(h_cpu);
        sjf(h_cpu);
        prioridade(h_cpu);
    }
    printf("----E/S----\n");
    if (h_entrada_saida == NULL)
    {
        printf("Sem processos\n");
    }
    else
    {
        listprocs(h_entrada_saida);
        fifo(h_entrada_saida);
        sjf(h_entrada_saida);
        prioridade(h_entrada_saida);
    }

    while (h_cpu != NULL)
    {
        ptmp = h_cpu;
        h_cpu = h_cpu->prox;
        free(ptmp);
    }
    while (h_entrada_saida != NULL)
    {
        ptmp = h_entrada_saida;
        h_entrada_saida = h_entrada_saida->prox;
        free(ptmp);
    }
    return 0;
}

void insere_lista(Processos **lista, int id, int surto, int prioridade)
{
    Processos *aux, *proc = (Processos *)malloc(sizeof(Processos));

    if (proc)
    {
        proc->id = id;
        proc->surto = surto;
        proc->prioridade = prioridade;
        proc->execucao = 0;
        proc->espera = 0;
        proc->prox = NULL;

        if (*lista == NULL)
            *lista = proc;
        else
        {
            aux = *lista;
            while (aux->prox)
                aux = aux->prox;
            aux->prox = proc;
        }
    }
    else
    {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
}

void listprocs(Processos *proc)
{
    struct processos *tmp = proc;
    printf("Processos\n");
    printf("\n");
    while (tmp != NULL)
    {
        printf("Processo: %d\tPrioridade: %d\tSurto: %d\n", tmp->id, tmp->prioridade, tmp->surto);
        tmp = tmp->prox;
    };
    printf("\n\n");
}

Processos *inicia_processos(int id, int surto, int prioridade)
{
    Processos *proc;
    proc = (Processos *)malloc(sizeof(Processos));
    if (proc == NULL)
    {
        printf("Falha ao alocacao de memoria. O programa vai ser finalizado\n");
        exit(1);
    }
    proc->id = id;
    proc->surto = surto;
    proc->prioridade = prioridade;
    proc->execucao = 0;
    proc->espera = 0;
    proc->prox = NULL;
    return (proc);
}

void fifo(Processos *proc)
{
    int tempo = 0, inicio, fim;
    Processos *tmp = proc;
    printf("fifo\n");
    printf("\n");
    while (tmp != NULL)
    {
        inicio = tempo;
        tempo += tmp->surto;
        fim = tempo;
        printf("Processo: %d\tSurto: %d\tEspera: %d\tRetorno: %d\n", tmp->id, tempo, inicio, fim);
        tmp = tmp->prox;
    }
    printf("\n\n");
}

void prioridade(Processos *proc)
{
    int tempo, inicio, fim, maior;
    Processos *aux, *tmpsrc, *tmp, *maiorprimeiro;
    printf("\nPrioridade\n");
    printf("\n");

    tmpsrc = proc;
    aux = tmp = NULL;
    while (tmpsrc != NULL)
    {
        if (aux == NULL)
        {
            aux = inicia_processos(tmpsrc->id, tmpsrc->surto, tmpsrc->prioridade);
            tmp = aux;
        }
        else
        {
            tmp->prox = inicia_processos(tmpsrc->id, tmpsrc->surto, tmpsrc->prioridade);
            tmp = tmp->prox;
        }
        tmpsrc = tmpsrc->prox;
    }
    tempo = 0;
    while (aux != NULL)
    {
        maiorprimeiro = NULL;
        maior = aux->prioridade;
        tmp = aux->prox;
        tmpsrc = aux;
        while (tmp != NULL)
        {
            if (tmp->prioridade < maior)
            {
                maior = tmp->prioridade;
                maiorprimeiro = tmpsrc;
            }
            tmpsrc = tmp;
            tmp = tmp->prox;
        }
        if (maiorprimeiro == NULL)
        {
            inicio = tempo;
            tempo += aux->surto;
            fim = tempo;
            printf("Processo: %d\tSurto: %d\tEspera: %d\tRetorno: %d\n", aux->id, tempo, inicio, fim);
            tmpsrc = aux->prox;
            free(aux);
            aux = tmpsrc;
        }
        else
        {
            tmp = maiorprimeiro->prox;
            inicio = tempo;
            tempo += tmp->surto;
            fim = tempo;
            printf("Processo: %d\tSurto: %d\tEspera: %d\tRetorno: %d\n", tmp->id, tempo, inicio, fim);
            maiorprimeiro->prox = tmp->prox;
            free(tmp);
        }
    }
    printf("\n\n");
}

void sjf(Processos *proc)
{
    int tempo, inicio, fim, shortest;
    Processos *copia, *tmpsrc, *tmp, *beforeshortest;
    printf("SJF\n");
    printf("\n");
    tmpsrc = proc;
    copia = tmp = NULL;
    while (tmpsrc != NULL)
    {
        if (copia == NULL)
        {
            copia = inicia_processos(tmpsrc->id, tmpsrc->surto, tmpsrc->prioridade);
            tmp = copia;
        }
        else
        {
            tmp->prox = inicia_processos(tmpsrc->id, tmpsrc->surto, tmpsrc->prioridade);
            tmp = tmp->prox;
        }
        tmpsrc = tmpsrc->prox;
    }
    tempo = 0;
    while (copia != NULL)
    {
        beforeshortest = NULL;
        shortest = copia->surto;
        tmp = copia->prox;
        tmpsrc = copia;
        while (tmp != NULL)
        {
            if (tmp->surto < shortest)
            {
                shortest = tmp->surto;
                beforeshortest = tmpsrc;
            }
            tmpsrc = tmp;
            tmp = tmp->prox;
        }
        if (beforeshortest == NULL)
        {
            inicio = tempo;
            tempo += copia->surto;
            fim = tempo;
            printf("Processo: %d\tSurto: %d\tEspera: %d\tRetorno: %d\n", copia->id, tempo, inicio, fim);
            tmpsrc = copia;
            copia = copia->prox;
            free(tmpsrc);
        }
        else
        {
            tmp = beforeshortest->prox;
            inicio = tempo;
            tempo += tmp->surto;
            fim = tempo;
            printf("Processo: %d\tSurto: %d\tEspera: %d\tRetorno: %d\n", tmp->id, tempo, inicio, fim);
            beforeshortest->prox = tmp->prox;
            free(tmp);
        }
    }
}