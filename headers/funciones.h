#include <iostream>
#include <chrono>
#include <fstream> 
#include <random>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

void experimentacion_arreglo_explicito(int* A, int n, int e, int modo);
void experimentacion_gap_coding(short* GC, int* sample, int n, int m, int e, int modo);
void experimentacion_shannon_fano(short* GCSF, int* sample, unordered_map<short, short> DSFC, unordered_map<short, short> DSFO, int n, int m, int e, int modo);
void guardar_valores(int* X, int n, char opcion);
