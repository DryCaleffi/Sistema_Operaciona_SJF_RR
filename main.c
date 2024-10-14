#include  <stdio.h>
#include  <funcao.h>


int main() {
    inicializar();
    int OP = 0;
    int quan;
    enum{
        INSERIR = 1
       ,LISTAR = 2
       ,EXECUTAR_SJF = 3
       ,EXECUTAR_CIRCULAR = 4
       ,SAIR = 5
       };
do {
    printf("\n\n1- Inserir processo\t2- Listar processos\n");
    printf("3- Executar por SJF\t4- Executar por Circular\n5- Sair\n\n");
    printf("Escolha uma opcao: ");
    scanf("%d",&OP);

    switch (OP) {
        case INSERIR:
            entrada();
        break;


        case LISTAR:
            imprimir();
        break;


        case EXECUTAR_SJF:
            executar_sjf();
        break;


        case EXECUTAR_CIRCULAR:
            printf("\nDefina o Quantum em inteiro: ");
                scanf("%d",&quan);
            executar_circular(quan);
        break;


        case SAIR:
            sair();
        break;


        default:
            printf("Opcao nao encontrada");
        break;
    }

}while(OP!=5);

}
