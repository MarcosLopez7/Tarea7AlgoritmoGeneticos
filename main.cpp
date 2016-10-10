#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <unistd.h>

#define CROMO_NUM 4

using namespace std;

int n;

void inicia(int *);
float evaluar(int *);
int *cruza(int *, int *, int);

int main(int argc, const char *argv[]) {

    if (argc != 3) {
        cout << "Se esperaba un argumento n que indica el tamaño del tablero de ajedrez n*n\n";
        cout << " y un 1 o 2, 1 para la mutacion con suma de 1, 2 para dos suma de 3\n";
        exit(-1);
    }

    int cont = 0;
    n = atoi(argv[1]);

    if (n <= 2) {
        cout << "n tiene que ser mayor a 2\n";
        exit(-1);
    }

    if ( atoi(argv[2]) != 1 && atoi(argv[2]) != 2) {
        cout << "Es un numero entre 1 y 2 el segundo parametro\n";
    }

    int **cromosomas = (int **) malloc(CROMO_NUM * sizeof(int *));
    float **fitness = (float **) malloc(CROMO_NUM * sizeof(float *));
    srand((int) time(NULL));

    for (int i = 0; i < CROMO_NUM; ++i) {
        cromosomas[i] = (int *) malloc(n * sizeof(int));
        inicia(cromosomas[i]);
    }

    for (int i = 0; i < CROMO_NUM; ++i) {
        fitness[i] = (float *) malloc(2 * sizeof(float));
    }

    while (true) {

        for (int i = 0; i < CROMO_NUM; ++i) {
            fitness[i][0] = i;
            fitness[i][1] = evaluar(cromosomas[i]);
        }

        cout << "Cromosomas actuales\n";
        for (int i = 0; i < CROMO_NUM; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << cromosomas[i][j] << ", ";
            }
            cout << "fitnes: " << fitness[i][1];
            cout << endl;
        }

        cout << endl;

        for (int i = 1; i < CROMO_NUM ; ++i) {
            for (int j = 0; j < CROMO_NUM - 1; ++j) {
                if (fitness[j][1] < fitness[j+1][1]) {
                    float *temp = fitness[j];
                    fitness[j] = fitness[j + 1];
                    fitness[j + 1] = temp;
                }
            }
        }

        if (fitness[0][1] == 1) {

            cout << "La solucion es: ";
            for (int i = 0; i < n; ++i) {
                cout << cromosomas[(int)fitness[0][0]][i] << ", ";
            }
            cout << endl;

            break;
        }

        int num;
        num = (rand() % 6) + 1;
        int *temp1 = cruza(cromosomas[(int)fitness[0][0]], cromosomas[(int) fitness[1][0]], num);
        int *temp2 = cruza(cromosomas[(int)fitness[1][0]], cromosomas[(int) fitness[0][0]], num);
        num = (rand() % 6) + 1;
        int *temp3 = cruza(cromosomas[(int)fitness[0][0]], cromosomas[(int) fitness[2][0]], num);
        int *temp4 = cruza(cromosomas[(int)fitness[2][0]], cromosomas[(int) fitness[0][0]], num);

        for (int i = 0; i < CROMO_NUM; ++i)
            free(cromosomas[i]);

        cromosomas[0] = temp1;
        cromosomas[1] = temp2;
        cromosomas[2] = temp3;
        cromosomas[3] = temp4;

        if (atoi(argv[2]) == 1) {
            //Sacamos un númeo entre 0 a n, ese número es el índice del cromosoma y se le va sumar 1, si el número random es
            //n, no se hace una mutación al cromosoma
            for (int i = 0; i < CROMO_NUM; ++i) {
                int numrand = rand() % (n + 1);
                if (numrand == n) continue;
                cromosomas[i][numrand] = (cromosomas[i][numrand] + 1) % n;
            }
        }

        if (atoi(argv[2]) == 2) {
            //suma dos elementos del cromosoma con 3
            for (int i = 0; i < CROMO_NUM; ++i) {
                int numrand = rand() % (n);
                cromosomas[i][numrand] = (cromosomas[i][numrand] + 3) % n;
                numrand = rand() % (n);
                cromosomas[i][numrand] = (cromosomas[i][numrand] + 3) % n;
            }
        }

        cont++;
        cout << "Vamos en la iteracion " << cont << ", aun nada\n";
        //sleep(1);
    }

    free(fitness);
    free(cromosomas);
    return 0;
}

void inicia(int *cromosoma) {

    for (int i = 0; i < n; ++i)
        cromosoma[i] = rand() % n;

}

float evaluar(int *cromosoma) {

    int **tablero = (int **) malloc(sizeof(int *) * n);

    for (int i = 0; i < n; ++i) {
        tablero[i] = (int *) malloc(sizeof(int) * n);
        for (int j = 0; j < n; ++j)
            tablero[i][j] = 0;
    }

    for (int i = 0; i < n; ++i)
        tablero[cromosoma[i]][i] = 1;

    int malas = 0;
    
    for (int i = 0; i < n; ++i) {

        bool reinaenlados = false;
        bool reinadiagonalI = false;
        
        //Evaluar hacia los lados
        for (int j = 0; j < n; ++j) {
            if (j == i)
                continue;
            
            if (tablero[cromosoma[i]][j] == 1) {
                malas++;
                reinaenlados = true;
                break;
            }
            
        }
        
        if (reinaenlados) continue;

        int x = i;
        int y = cromosoma[i];

        while (x != 0 && y != 0) {x--; y--;}

        for (int j = 0; j < n; ++j) {
            if((x + j) < n && (y + j) < n && (j + x) != i) {
                if(tablero[y+j][x+j] == 1) {
                    malas++;
                    reinadiagonalI = true;
                    break;
                }
            }
        }

        if (reinadiagonalI) continue;

        x = i;
        y = cromosoma[i];

        while (x != 0 && y != 0) {x--; y++;}

        for (int j = 0; j < n; ++j) {
            if((x + j) < n && (y - j) >= 0 && (y - j < n) && (j + x) != i) {
                if(tablero[y-j][x+j] == 1) {
                    malas++;
                    break;
                }
            }
        }

    }

    free(tablero);
    return (float)(n - malas) /(float) n;
}


int *cruza(int *a, int *b, int num) {

    int *temp = (int *) malloc(sizeof(int) * n);

    for (int i = 0; i < num; ++i)
        temp[i] = a[i];

    for (int i = num; i < n; ++i)
        temp[i] = b[i];

    return temp;
}





























