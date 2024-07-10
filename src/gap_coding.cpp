#include "../headers/gap_coding.h"

// Generar un arreglo gap_coding a partir de un Arreglo A de 
// tamaño n. Cada valor del arreglo generado tendrá el valor de 
// A[i] - A[i-1] con i > 0 el primer valor del arreglo será 
// igual a A[0].
short* generar_arreglo_gap_coding(int* A, int n){
	short* GC = new short[n];	
	
	for(int i = n-1; i > 0; i--)
		GC[i] = short(A[i] - A[i-1]);
		
	GC[0] = short(A[0]);
	return GC;
}

// Generar un arreglo sample de tamaño m a partir de un Arreglo 
// A de tamaño n. Cada valor en la posición i del sample será 
// A[i * (n/m)].
int* generar_sample(int* A, int n, int m) {
	int* sample = new int[m];
	int b = n / m;
	
	for (int i = 0; i < m; i++) {
		sample[i] = A[i*b]; 
	}
	
	return sample;
}

// Realiza la busqueda binaria sobre un arreglo sample y 
// complementa con un arreglo gap_coding para buscar val
int busqueda_binaria_gap_coding(short* GC, int* sample, int n, int m, int val){
	int izq = 0;
	int der = m-1;
	int med, num;	
	int b = n / m;
	// Se ejecuta la búsqueda binaria en el sample
	while(izq <= der){
		med = izq + (der - izq) / 2;
		
		if(sample[med] == val)
			return med * b;
			
		if(sample[med] < val)
			izq = med + 1;
		else 
			der = med - 1;
	}
	// Si no se encuentra el valor en el sample, continúa
		
	// Si med es 0, significa que el valor a buscar es menor
	// al primer valor del arreglo. Al estar este ordenado,
	// significa que el valor no se encuentra.
	if(med == 0) return -1;
	
	// Si el valor a buscar es menor que el que se encuentra en 
	// la posición med, entonces se corrige retrocediendo una 
	// posición. 
	//      0  1  2
	// Ej: [1, 3, 5], buscando el 4 retorna la posición 2
	// 4 < 5 entonces se continúa con la posición 1
	if(val < sample[med])
		med -= 1;
	
	// Se obtiene el valor en la posición med para comenzar
	// a reconstruir el valor
	num = sample[med];
	
	// Obtenemos la posición en el arreglo gap_coding en la que 
	// se encontraría el valor a reconstruir 
	med = med * b;
		
	// Comenzamos a reconstruir el valor.
	// Retorna -1 si se pasa del tamaño del arreglo med >= n o
	// si el valor reconstruido pasa del valor a buscar. Valor 
	// no se encuentra.
	
	while(med < n && num <= val){
		// Si se encuentra se retorna su posición
		if (num == val) 
			return med;
		
		// Se agrega la siguiente diferencia al valor que se 
		// reconstruye
		num = num + GC[med];
		med = med + 1;
	}
	
	return -1;
}
