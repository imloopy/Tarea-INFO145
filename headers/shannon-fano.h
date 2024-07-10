#include <iostream>
#include <unordered_map>
#include <cmath>
#include <bitset>

using namespace std;

// Definición de la estructura para los elementos almacenados en C
struct Elemento {
    short simbolo;
    double probabilidad;
    int codigo;
};

void swap(Elemento& a, Elemento& b);
int particion_probabilidad(Elemento* A, int low, int high);
void quicksort(Elemento* A, int low, int high);
pair<Elemento*, int> conseguir_valores(short* A, int n);
pair<int, double> divisorProbabilidad(Elemento* C, int a, int b, double probTotal);
void codificarSF(Elemento* C, int a, int b, double probTotal, int pot);
unordered_map<short, short> generar_DSF(Elemento* C, int a, int n);
short* generar_GCSF(short* GC, unordered_map<short, short>& DSFC, unordered_map<short, short>& DSFO, int n);
int decodificar_SF(unordered_map<short, short>& DSFC, unordered_map<short, short>& DSFO, int val);
int busqueda_binaria_shannon_fano(short* GCSF, int* sample, unordered_map<short, short> DSFC, unordered_map<short, short> DSFO, int n, int m, int val);
