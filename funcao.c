#include <stdio.h>
#include <stdlib.h>
#include <funcao.h>
#include <stdbool.h>
#include <string.h>

// Define uma struct para guardar os dados dos processos
typedef struct {
    int TempoChegada;
    int TempoExecut;
    char NomeProcesso[20];
    int Turnaround;
    int Espera;
    int Final;
} processo;

// Clock, para caucular o tempo de processamento e contar cada tick de tempo, acaba sendo o tempo de processamento
// Indice, para guardar o numero de de processos adicionados
int Clock = 0, indice = 0, tam = 5, controle = 0;
float TempoRetorno = 0, TempoEspera = 0;
processo *vetor;

// Inicializa um ponteiro com alocação dinâmica
void inicializar()
{
    vetor = malloc(tam*sizeof(processo));
}


// Função para aumentar o tamanho da alocação do vetor
void resize()
{
    tam += tam/2;
    processo *aux;
    aux = malloc(tam*sizeof(processo));

    for (int i = 0; i<indice; i++){
        aux[i] = vetor[i];
    }
    free(vetor);
    vetor = aux;
}


// Função para entrada de dados dos processos
void entrada ()
{
    if (indice == tam){
        resize();
    }
    printf("Digite o nome do processo, o tempo de entrada e o tempo de execucao\n");
    printf("Nome: ");
        scanf("%s",&vetor[indice].NomeProcesso);
    printf("\nTempoChegada: ");
        scanf("%d",&vetor[indice].TempoChegada);
    printf("\nTempoExecucao: ");
        scanf("%d",&vetor[indice].TempoExecut);

    indice++;
}


// Imprime os processos existentes
void imprimir()
{
    if (indice == 0) {
        printf("Nenhum processo existente");
    }else{
        for (int i = 0; i < indice; i++){
            printf("\nProcesso %d\n", (i+1));
            printf("Nome: %s\t\tTempo de chegada: %d\tTempo de execuxao: %d\n", vetor[i].NomeProcesso, vetor[i].TempoChegada, vetor[i].TempoExecut);
        }
    }
}


// Ordena os processos por tempo de chegada e por tempo de execução
void ordenacao_sjf()
{
    processo aux;

    /* Utiliza-se i começando em controle pois a cada processo finalizado o controle recebe ++,
    logo esse processo não precisa se mais ordenado na proxima vez que a função for chamada */
    for (int i=controle;i<(indice-1);i++){
        for (int j=(i+1);j<indice;j++){
            if (((vetor[i].TempoChegada <= Clock) && (vetor[j].TempoChegada <= Clock) && (vetor[i].TempoExecut > vetor[j].TempoExecut)) || ((vetor[i].TempoChegada > Clock) && (vetor[j].TempoChegada <= Clock))){
                aux = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = aux;
            }
        }
    }
}


// Ordena os processos por tempo de chegada
void ordenacao_circular()
{
    processo aux;

    for (int i=controle;i<(indice-1);i++){
        for (int j=(i+1);j<indice;j++){
            if (vetor[i].TempoChegada > vetor[j].TempoChegada){
                aux = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = aux;
            }
        }
    }
}


// Libera a memória alocada
void sair()
{
    free(vetor);
}


// Volta todos os valores para os definidos inicialmente e libera memória para poder executar novamente
void reset()
{
    indice = 0;
    controle = 0;
    Clock = 0;
    TempoEspera = 0;
    TempoEspera = 0;
    free(vetor);
    inicializar();
}


// Executa pelo metodo sjf :)
void executar_sjf()
{
    if (indice == 0) {
        printf("Nenhum processo existente");

    }else {

        do {
            // Ordena os processos antes de inicializar o o processamento
            ordenacao_sjf();

            /* Se o processo tiver o tempo de chegada <= que o Clock (ele ja foi criado neste ponto do tempo)
            então é adicionado o tempo de execução do processo ao Clock e feito os cálculos de espera e turnaroud */
            if(vetor[controle].TempoChegada<=Clock){
                Clock = Clock + vetor[controle].TempoExecut;
                vetor[controle].Turnaround = (Clock - vetor[controle].TempoChegada);
                vetor[controle].Espera = (vetor[controle].Turnaround - vetor[controle].TempoExecut);
                controle++;

            /*Se não existir processos criados no tempo em que o Clock está,
            é adicionado uma unidade de tempo ao Clock */
            }else {
                Clock++;
            }
        // Quando o controle for igual ao índice, significa que o último processo foi processado
        }while(controle < indice);

        for (int i = 0; i < indice; i++){

            // Soma-se os tempos de espera e retorno para fazer calcular o tempo médio de cada
            TempoEspera = (TempoEspera + vetor[i].Espera);
            TempoRetorno = (TempoRetorno + vetor[i].Turnaround);

            // Imprimi os valores individuais de cada processo
            printf("\nNome: %s\t\tTempo de chegada: %d\tTempo de execucao: %d\n", vetor[i].NomeProcesso, vetor[i].TempoChegada, vetor[i].TempoExecut);
            printf("Tunaround: %d\tTempo de espera: %d\n", vetor[i].Turnaround, vetor[i].Espera);
        }
            /* Imprime tempos médios e de processamento, dividi-se pelo índice pois ele indica o número de
            processos existentes, o clock é o próprio tempo de processamento total  */
            printf("\nTempo medio de retorno: %.2f\tTempo de processamento: %d\tTempo medio de espera: %.2f\n", (TempoRetorno/indice), Clock, (TempoEspera/indice));

        reset();
    }
}


// Executa pelo metodo circular :), o quantum é definido pelo usuário
void executar_circular(int qunatum)
{
    /* O teste é utilizado para testar se existem processos para serem executados no tempo atual do Clock,
    vetor.final indica se o processo já foi finalizado */
    bool teste = false;
    for (int i = 0; i < indice; i++) {
        vetor[i].Final = false;
    }
    // Ordena os processos por ordem de chegada
    ordenacao_circular();

    // Cria um ponteiro para auxiliar
    processo *cop = malloc(indice*sizeof(processo));

    // Copia os dados de um ponteiro para o outro, e não a posição onde ele está alocado
    memcpy(cop, vetor, indice*sizeof(processo));

    if (indice == 0) {
        printf("Nenhum processo existente");

    }else {

        do {
            // Antes de iniciar o for, deixo as duas variaveis com seus valores iniciais
            controle = 0;
            teste = false;
            for (int i = 0; i < indice; i++){

                // Se o processo já existe no tempo atual do Clock, e ele ainda não foi finalizado, inicio ele
                if (vetor[i].Final == false && vetor[i].TempoChegada <= Clock) {

                    /* Se Tempo de execução restante for maior que o quantum, retiro a diferença
                    e aumento o Clock, teste verdadeiro foi realizado um processamento */
                    if (vetor[i].TempoExecut > qunatum) {
                        vetor[i].TempoExecut = vetor[i].TempoExecut - qunatum;
                        Clock = Clock + qunatum;
                        teste = true;

                    /* Se tempo de execução restante for igual ao quantum, a diferença vai zerar o tempo de execução,
                    aumento o Clock, calcula-se espera e turnaround, teste verdadeiro */
                    }else if(vetor[i].TempoExecut == qunatum) {
                        vetor[i].TempoExecut = 0;
                        Clock = Clock + qunatum;
                        vetor[i].Turnaround = Clock - vetor[i].TempoChegada;
                        vetor[i].Espera = vetor[i].Turnaround - cop[i].TempoExecut;
                        teste = true;

                    // Se tempo de execução restante igual a 0, defino vetor.final com verdadeiro
                    }else if(vetor[i].TempoExecut == 0){
                        vetor[i].Final = true;
                        teste = true;

                    /* Se tempo de execução menor que quantum e diferente de 0, aumento o clock pelo tempo
                    restante que tinha de execução e não pelo quantum, faço os calculos, teste verdadeiro */
                    }else {
                        Clock = Clock + vetor[i].TempoExecut;
                        vetor[i].TempoExecut = 0;
                        vetor[i].Turnaround = Clock - vetor[i].TempoChegada;
                        vetor[i].Espera = vetor[i].Turnaround - cop[i].TempoExecut;
                        teste = true;
                    }
                }
                /* Se todos os processos forem finalizados neste for, a soma do controle será igual ao numero
                de processos existentes, assim finalizando a execução */
                if (vetor[i].Final == true) {
                    controle++;
                }
            }
            // Se o teste for falso, não existia processo para ser executado, então aumenta-se uma unidade de tempo do Clock
            if (teste == false) {
                Clock++;
            }
        }while(controle != indice);

        for (int i = 0; i < indice; i++){

            TempoEspera = (TempoEspera + vetor[i].Espera);
            TempoRetorno = (TempoRetorno + vetor[i].Turnaround);

            // Imprimi os valores individuais de cada processo
            printf("\nNome: %s\t\tTempo de chegada: %d\tTempo de execucao: %d\n", vetor[i].NomeProcesso, vetor[i].TempoChegada, cop[i].TempoExecut);
            printf("Tunaround: %d\tTempo de espera: %d\n", vetor[i].Turnaround, vetor[i].Espera);
        }
        /* Imprime tempos médios e de processamento, dividi-se pelo índice pois ele indica o número de
        processos existentes, o clock é o próprio tempo de processamento total  */
        printf("\nTempo medio de retorno: %.2f\tTempo de processamento: %d\tTempo medio de espera: %.2f\n", (TempoRetorno/indice), Clock, (TempoEspera/indice));

        free(cop);
        reset();
    }
}