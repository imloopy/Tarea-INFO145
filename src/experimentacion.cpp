#include "../headers/arreglo_explicito.h"
#include "../headers/shannon-fano.h"
#include "../headers/gap_coding.h"
#include "../headers/funciones.h"
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cout << "Ejecutar como: " << argv[0] << " n f s\n";
        cout << "n: tamaño arreglo original\nf: divisor para tamaño sample (n/f)\ns: desviación estándar\n";
        return 1;
    }

    int n = atoi(argv[1]); // Tamaño del arreglo original
    int f = atoi(argv[2]); // Divisor de n para tamaño del sample  
    int s = atoi(argv[3]); // Desviación estándar para el arreglo con distribución normal  
	int l = n/f;           // Tamaño sample (arbitrario para reducir hasta un cuarto el tamaño original)
	int m = 0;             // Media para el arreglo con distribución normal
	int e = 8;             // Cantidad de valores aleatorios (máximo de diferencias entre valores + 1)
	
	cout << "Tamaño arreglo original: " << n << "\nTamaño sample: " << l << "\nDesviación estándar: " << s << "\n" << endl;
	
	// ===========================
	// = ARREGLO LINEAL ORDENADO =
	// ===========================

	// ARREGLO EXPLÍCITO - ARREGLO LINEAL

	// Generar arreglo dinámico normal L
	int* L = generar_arreglo_lineal(n, e);
	//guardar_valores(L, n, 'L');
	
	/*
	cout << sizeof(long)*8 << endl; // 64
	cout << sizeof(int)*8 << endl; // 32
	cout << sizeof(short)*8 << endl; // 16
	cout << sizeof(char)*8 << endl; // 8
	*/
	
	// Ejecutar la experimentación de Arreglo Explícito 
	// para arreglo lineal L.
	cout << "Experimento Arreglo Explícito - Arreglo Lineal" << endl;
	cout << "Búsqueda lineal: ";
	experimentacion_arreglo_explicito(L, n, e, 1);
	cout << "Búsqueda aleatoria: ";
	experimentacion_arreglo_explicito(L, n, e, 2);
	cout << "Espacio utilizado: " << 
	sizeof(L)*8 + 
	sizeof(L[0])*8*n 
	<< "bits" << endl << endl;
	
	// GAP-CODING - ARREGLO LINEAL

	short* GCL = generar_arreglo_gap_coding(L, n);
	int* sampleL = generar_sample(L, n, l);
	
	// Liberar la memoria del arreglo L,
	// no se volverá a utilizar.
	delete[] L;
	
    cout << "Posibles valores distintos: " << e << endl;
	// Ejecutar la experimentación de GAP-Coding 
	// para arreglo lineal L.
	cout << "Experimento GAP-Coding - Arreglo Lineal" << endl;
	cout << "Búsqueda lineal: ";
	experimentacion_gap_coding(GCL, sampleL, n, l, e, 1);
	cout << "Búsqueda aleatoria: ";
	experimentacion_gap_coding(GCL, sampleL, n, l, e, 2);
	cout << "Espacio utilizado: " << 
	sizeof(GCL)*8 + 
	sizeof(GCL[0])*8*n + 
	sizeof(sampleL)*8 + 
	sizeof(sampleL[0])*8*l 
	<< "bits" << endl << endl;
	
	// SHANNON-FANO - ARREGLO LINEAL
	// Obtener elementos únicos del arreglo de GAP-Coding
	// Cada elemento tendrá un campo de símbolo, probabilidad
	// y código.
	pair<Elemento*, short> valoresL = conseguir_valores(GCL, n);
    Elemento* elementosL = valoresL.first;
    int tamanoElementosL = valoresL.second;
    cout << "Valores distintos: " << tamanoElementosL << endl;
    int indice_medioL;
    
    // Se obtiene la probabilidad del simbolo menos probable
    double probMenorL = elementosL[tamanoElementosL-1].probabilidad;

    // Se identifica el índice que separa a los elementos más
    // comunes de los outliers.
    for(int i = tamanoElementosL-2; i > 0; i--)
    	// Se define que los elementos con probabilidad mayor al
    	// a la probabilidad menor son comunes. Todos los elementos
    	// con la probabilidad menor se consideran outliers.
    	if(elementosL[i].probabilidad != probMenorL){
    		indice_medioL = i+1;
    		break;
		}
    
    double probCL = 0;
    // Se calcula la probabilidad total de los elementos comunes
    // y la de los outliers para la codificación.
    for(int i = 0; i <= indice_medioL; i++){
    	probCL += elementosL[i].probabilidad;
    }
    double probOL = 1-probCL;
    
    // Los elementos son codificados usando Shannon-Fano a partir
    // del segundo bit. Se usará el primer bit para decidir si el
    // elementos es común o outlier. 0 si es común, 1 si es outlier.
    // Esto para evitar que los outliers empeoren la compresión de los
    // elementos más comunes. Luego se utilizará un diccionario para 
    // decodificar los elementos más comunes y otro para los outliers.
	codificarSF(elementosL, 0, indice_medioL, probCL, 1);
	codificarSF(elementosL, indice_medioL+1, tamanoElementosL-1, probOL, 1);
	
	// Se agrega a los códigos de los outliers un 1 al inicio.
	// Los más comunes mantendrán un 0 al inicio de su código
    for(int i = indice_medioL+1; i < tamanoElementosL; i++)
    	elementosL[i].codigo += 1;
	
	// Se genera un diccionario de decodificación para los 
	// elementos comunes y otro para los outliers
	unordered_map<short, short> DSFLC = generar_DSF(elementosL, 0, indice_medioL+1);
	unordered_map<short, short> DSFLO = generar_DSF(elementosL, indice_medioL+1, tamanoElementosL);
	
	// Se codifica el arreglo GAP-Coding usando la codificación
	// generada para los elementos comunes y outliers
	short* GCSFL = generar_GCSF(GCL, DSFLC, DSFLO, n);
	
	// Liberar la memoria del arreglo GCL y
	// elementosL, no se volverán a utilizar.
	delete[] elementosL;
	delete[] GCL;
	

	// Ejecutar la experimentación de Shannon-Fano 
	// para arreglo lineal L.
	cout << "Experimento Shannon-Fano - Arreglo Lineal" << endl;
	cout << "Búsqueda lineal: ";
	experimentacion_shannon_fano(GCSFL, sampleL, DSFLC, DSFLO, n, l, e, 1);
	cout << "Búsqueda aleatoria: ";
	experimentacion_shannon_fano(GCSFL, sampleL, DSFLC, DSFLO, n, l, e, 2);
	cout << "Espacio utilizado: " << 
	sizeof(GCSFL)*8 + 
	sizeof(GCSFL[0])*8*n + 
	sizeof(sampleL)*8 + 
	sizeof(sampleL[0])*8*l + 
	sizeof(DSFLC)*8 +
	(DSFLC.size() * (sizeof(short) + sizeof(short))) *8 +
	sizeof(DSFLO)*8 +
	(DSFLO.size() * (sizeof(short) + sizeof(short))) *8
	<< "bits" << endl << endl;
	
	// Liberar la memoria del arreglo sampleL	
	delete[] GCSFL;
	delete[] sampleL;
	
	// ===========================
	// = ARREGLO NORMAL ORDENADO =
	// ===========================
	
	// ARREGLO EXPLÍCITO - ARREGLO NORMAL
	
	// Generar arreglo dinámico normal N
	int* N = generar_arreglo_normal(n, e, m, s);
	//guardar_valores(N, n, 'N');
	
	// Ejecutar la experimentación de Arreglo Explícito
	// para arreglo normal N
	cout << "Experimento Arreglo Explícito - Arreglo Normal" << endl;
	cout << "Búsqueda lineal: ";
	experimentacion_arreglo_explicito(N, n, e, 1);
	cout << "Búsqueda aleatoria: ";
	experimentacion_arreglo_explicito(N, n, e, 2);
	cout << "Espacio utilizado: " << 
	sizeof(N)*8 + 
	sizeof(N[0])*8*n 
	<< "bits" << endl << endl;

	// GAP-CODING - ARREGLO NORMAL

	short* GCN = generar_arreglo_gap_coding(N, n);
	int* sampleN = generar_sample(N, n, l);
		
	// Liberar la memoria del arreglo N
	delete[] N;

    cout << "Posibles valores distintos: " << e << endl;
	// Ejecutar la experimentación de GAP-Coding 
	// para arreglo normal N.
	cout << "Experimento GAP-Coding - Arreglo Normal" << endl;
	cout << "Búsqueda lineal: ";
	experimentacion_gap_coding(GCN, sampleN, n, l, e, 1);
	cout << "Búsqueda aleatoria: ";
	experimentacion_gap_coding(GCN, sampleN, n, l, e, 2);
	cout << "Espacio utilizado: " << 
	sizeof(GCN)*8 + 
	sizeof(GCN[0])*8*n + 
	sizeof(sampleN)*8 + 
	sizeof(sampleN[0])*8*l 
	<< "bits" << endl << endl;

	
	// SHANNON-FANO - ARREGLO NORMAL
	
	// Obtener elementos únicos del arreglo de GAP-Coding
	// Cada elemento tendrá un campo de símbolo, probabilidad
	// y código.
	pair<Elemento*, short> valoresN = conseguir_valores(GCN, n);
    Elemento* elementosN = valoresN.first;
    int tamanoElementosN = valoresN.second;
    cout << "Valores distintos: " << tamanoElementosN << endl;
    
    int indice_medioN;
    
    // Se obtiene la probabilidad del simbolo menos probable
    double probMenorN = elementosN[tamanoElementosN-1].probabilidad;

    // Se identifica el índice que separa a los elementos más
    // comunes de los outliers.
    for(int i = tamanoElementosN-2; i > 0; i--)
    	// Se define que los elementos con probabilidad mayor al
    	// a la probabilidad menor son comunes. Todos los elementos
    	// con la probabilidad menor se consideran outliers.
    	if(elementosN[i].probabilidad != probMenorN){
    		indice_medioN = i+1;
    		break;
		}
    
    double probCN = 0;
    // Se calcula la probabilidad total de los elementos comunes
    // y la de los outliers para la codificación.
    for(int i = 0; i <= indice_medioN; i++){
    	probCN += elementosN[i].probabilidad;
    }
    double probON = 1-probCN;
    
    // Los elementos son codificados usando Shannon-Fano a partir
    // del segundo bit. Se usará el primer bit para decidir si el
    // elementos es común o outlier. 0 si es común, 1 si es outlier.
    // Esto para evitar que los outliers empeoren la compresión de los
    // elementos más comunes. Luego se utilizará un diccionario para 
    // decodificar los elementos más comunes y otro para los outliers.
	codificarSF(elementosN, 0, indice_medioN, probCN, 1);
	codificarSF(elementosN, indice_medioN+1, tamanoElementosN-1, probON, 1);
	
	// Se agrega a los códigos de los outliers un 1 al inicio.
	// Los más comunes mantendrán un 0 al inicio de su código
    for(int i = indice_medioN+1; i < tamanoElementosN; i++)
    	elementosN[i].codigo += 1;
	
	// Se genera un diccionario de decodificación para los 
	// elementos comunes y otro para los outliers
	unordered_map<short, short> DSFNC = generar_DSF(elementosN, 0, indice_medioN+1);
	unordered_map<short, short> DSFNO = generar_DSF(elementosN, indice_medioN+1, tamanoElementosN);
	
	// Se codifica el arreglo GAP-Coding usando la codificación
	// generada para los elementos comunes y outliers
	short* GCSFN = generar_GCSF(GCN, DSFNC, DSFNO, n);
	
	// Liberar la memoria del arreglo GCL y
	// elementosL, no se volverán a utilizar.
	delete[] elementosN;
	delete[] GCN;
	
	// Ejecutar la experimentación de Shannon-Fano 
	// para arreglo normal N.
	cout << "Experimento Shannon-Fano - Arreglo Normal" << endl;
	cout << "Búsqueda lineal: ";
	experimentacion_shannon_fano(GCSFN, sampleN, DSFNC, DSFNO, n, l, e, 1);
	cout << "Búsqueda aleatoria: ";
	experimentacion_shannon_fano(GCSFN, sampleN, DSFNC, DSFNO, n, l, e, 2);
	cout << "Espacio utilizado: " << 
	sizeof(GCSFN)*8 + 
	sizeof(GCSFN[0])*8*n + 
	sizeof(sampleN)*8 + 
	sizeof(sampleN[0])*8*l + 
	sizeof(DSFNC)*8 +
	(DSFNC.size() * (sizeof(short) + sizeof(short))) *8 +
	sizeof(DSFNO)*8 +
	(DSFNO.size() * (sizeof(short) + sizeof(short))) *8
	<< "bits" << endl << endl;
	
	// Liberar la memoria del arreglo sampleN
	// y GCSFN	
	delete[] GCSFN;
	delete[] sampleN;
	
	return 0;
}

