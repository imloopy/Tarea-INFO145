#include "../headers/arreglo_explicito.h"
#include "../headers/shannon-fano.h"
#include "../headers/gap_coding.h"
#include "../headers/funciones.h"

// Función para medir el tiempo de búsqueda binaria usando Arreglo Explícito
void experimentacion_arreglo_explicito(int* A, int n, int e, int modo) {
    // Iniciar cronómetro
    high_resolution_clock::time_point inicio = high_resolution_clock::now();

    switch(modo) {
        case 1: // Búsqueda de valores crecientes consecutivos desde 0 a n-1
            for (int val = 0; val < n; val++)
                busqueda_binaria(A, n, val);
            break;
        case 2: // Búsqueda de valores aleatorios, cada uno mayor al anterior.
            srand(time(0));
            int val;
            for (int i = 1; i < n; ++i){
                val = rand() % (n * (e-1) - e + 1);
                busqueda_binaria(A, n, val);
            }
            break;
        default:
            break;
    };

    // Finalizar cronómetro
    high_resolution_clock::time_point fin = high_resolution_clock::now();
    duration<double> tiempo_transcurrido = duration_cast<duration<double>>(fin - inicio);

    cout << tiempo_transcurrido.count() << " segundos\n";
}

// Función para medir el tiempo de búsqueda binaria usando GAP-Coding
void experimentacion_gap_coding(short* GC, int* sample, int n, int m, int e, int modo) {
    // Iniciar cronómetro
    high_resolution_clock::time_point inicio = high_resolution_clock::now();

    switch(modo) {
        case 1: // Búsqueda de valores crecientes consecutivos desde 0 a n-1
            for (int val = 0; val < n; val++)
                busqueda_binaria_gap_coding(GC, sample, n, m, val);
            break;
        case 2: // Búsqueda de valores aleatorios, cada uno mayor al anterior.
            srand(time(0));
            int val;
            for (int i = 1; i < n; ++i){
                val = rand() % (n * (e-1) - e + 1);
                busqueda_binaria_gap_coding(GC, sample, n, m, val);
            }
            break;
        default:
            break;
    };

    // Finalizar cronómetro
    high_resolution_clock::time_point fin = high_resolution_clock::now();
    duration<double> tiempo_transcurrido = duration_cast<duration<double>>(fin - inicio);

    cout << tiempo_transcurrido.count() << " segundos\n";
}

// Función para medir el tiempo de búsqueda binaria usando Shannon-Fano
void experimentacion_shannon_fano(short* GCSF, int* sample, unordered_map<short, short> DSFC, unordered_map<short, short> DSFO, int n, int m, int e, int modo) {
    // Iniciar cronómetro
    high_resolution_clock::time_point inicio = high_resolution_clock::now();

    switch(modo) {
        case 1: // Búsqueda de valores crecientes consecutivos desde 0 a n-1
            for (int val = 0; val < n; val++)
                busqueda_binaria_shannon_fano(GCSF, sample, DSFC, DSFO, n, m, val);
            break;
        case 2: // Búsqueda de valores aleatorios, cada uno mayor al anterior.
            srand(time(0));
            int val;
            for (int i = 1; i < n; ++i){
                val = rand() % (n * (e-1) - e + 1);
                busqueda_binaria_shannon_fano(GCSF, sample, DSFC, DSFO, n, m, val);
            }
            break;
        default:
            break;
    };

    // Finalizar cronómetro
    high_resolution_clock::time_point fin = high_resolution_clock::now();
    duration<double> tiempo_transcurrido = duration_cast<duration<double>>(fin - inicio);

    cout << tiempo_transcurrido.count() << " segundos\n";
}

void guardar_valores(int* X, int n, char opcion){
    ofstream archivo;

	// Abrir archivo
    if (opcion == 'L')
        archivo.open("valores/valores_lineales.txt");
    else if (opcion == 'N')
        archivo.open("valores/valores_normales.txt");

	// Ecribir valores
    for (int i = 0; i < n; i++)
        archivo << X[i] << " ";

    archivo.close();
}

