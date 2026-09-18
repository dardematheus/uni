#include <stdio.h>

int sqrt_nr(int x, int i);

int
main(void)
{
    int x, i;
    printf("Programa de Raiz Quadrada -- Newton-Raphson\n"
           "Desenvolvedores: Matheus Darde Holdefer, Pedro Henrique Corral Livi, Eduardo Castilhos de Castilho\n");

    x = i = 0;
    while(1){
        printf("Digite os parametros x e i para calcular sqrt_nr(x, i) ou -1 para abortar a execucao\n");

        scanf("%d", &x);
        if(x < 0) return 0;
        scanf("%d", &i);
        if(i < 0) return 0;

        printf("sqrt_nr(%d, %d) = %d\n", x, i, sqrt_nr(x, i));
    }
    return 0;
}

int
sqrt_nr(int x, int i)
{
    if(i == 0) return 1;
    
    return (sqrt_nr(x, i - 1) + (x / sqrt_nr(x, i - 1))) >> 1;
}
