#ifndef SIMULADOR_H_INCLUDED
#define SIMULADOR_H_INCLUDED

#define MAX 50

typedef struct processos
{
    int id;
    int surto;
    int prioridade;
    int execucao;
    int espera;
    struct processos *prox;
} Processos;

void insere_lista(Processos **lista, int id, int surto, int prioridade);
void listprocs(Processos *proc);
Processos *inicia_processos(int id, int surto, int prioridade);
void fifo(Processos *proc);
void prioridade(Processos *proc);
void sjf(Processos *proc);

#endif // SIMULADOR_H_INCLUDED