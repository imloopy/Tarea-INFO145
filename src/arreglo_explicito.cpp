#include "../headers/arreglo_explicito.h"

// Función para generar un arreglo ordenado de tamaño n con 
// aumento máximo e
int* generar_arreglo_lineal(int n, int e) {
	int* arreglo = new int[n];

	srand(time(0));
	arreglo[0] = rand() % e;

	for (int i = 1; i < n; ++i)
		arreglo[i] = arreglo[i-1] + rand() % e;

	return arreglo;
}

// Función para generar un arreglo dinámico con distribución 
// normal de tamaño n, aumento máximo e, media m y desviación 
// estándar s.
int* generar_arreglo_normal(int n, int e, int m, int s) {
	int* arreglo = new int[n];

	mt19937 gen(time(0));
	normal_distribution<> d(m, s);
	
	// Para que tanto el arreglo Lineal como el Normal
	// tengan valores dentro del mismo intervalo se propone:
	// En el caso que la función rand()% e para generar el
	// arreglo lineal siempre genere el valor máximo posible,
	// es decir (e-1) este será sumado al valor anterior n
	// veces, por lo que el mayor valor teórico será n*(e-1).
	// Entonces para generar valores desde 0 (posible inicio)
	// del arreglo lineal y n*(e-1), se usa una distribución
	// normal de media 0, puediendo generar un máximo n*(e-1)/2 
	// positivo y n*(e-1)/2 negativo. Luego, se suma el primer
	// valor del arreglo en valor absoluto a todo el arreglo,
	// desplazando la distribución normal iniciando en 0 y con
	// un máximo posible de n*(e-1).
	
	for (int i = 0; i < n; ++i)
		arreglo[i] = static_cast<int>(d(gen)) % ((n * (e-1) + 1)/2);
	
	sort(arreglo, arreglo + n);
	
	
	if (arreglo[0] < 0){
		int min = fabs(arreglo[0]);
		for (int i = 0; i < n; ++i)
			arreglo[i] += min;
	}
	
	return arreglo;
}


// Implementación de búsqueda binaria no recursiva sobre un 
// arreglo ordenado A de tamaño n y valor a buscar val.
int busqueda_binaria(int* A, int n, int val) {
	int med;
	int izq = 0;
	int der = n-1;
	
	while (izq <= der) {
		med =  izq + (der - izq) / 2;
	
		if (A[med] == val)
			return med;
		else if (A[med] < val)
			izq = med + 1;
		else
			der = med - 1;
	}

	return -1;
}

