#include "../headers/shannon-fano.h"

void swap(Elemento& a, Elemento& b) {
    Elemento temp = a;
    a = b;
    b = temp;
}

// Función de partición para quicksort en orden descendente
int particion_probabilidad(Elemento* A, int low, int high) {
    double p = A[high].probabilidad;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (A[j].probabilidad > p) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[high]);
    return i + 1;
}

// Función de ordenamiento quicksort
void quicksort(Elemento* A, int low, int high) {
    if (low < high) {
        int pi = particion_probabilidad(A, low, high);

        quicksort(A, low, pi - 1);
        quicksort(A, pi + 1, high);
    }
}

pair<Elemento*, int> conseguir_valores(short* A, int n) {
    unordered_map<short, Elemento> C;
    
    for (int i = 0; i < n; ++i) {
        short symbol = static_cast<short>(A[i]);
        if (C.find(symbol) != C.end()) {
            C[symbol].probabilidad += 1.0 / n;
        } else {
            C[symbol] = { symbol, 1.0 / n, 0 };
        }
    }
    
    // Ordenamiento usando quicksort
    Elemento* elementos = new Elemento[C.size()];
    int index = 0;
    for (auto& entry : C) {
        elementos[index++] = entry.second;
    }
    quicksort(elementos, 0, C.size() - 1);
    
    return { elementos, static_cast<int>(C.size()) };
}

// Función para encontrar el divisor de probabilidad
pair<int, double> divisorProbabilidad(Elemento* C, int a, int b, double probTotal) {
    if (b - a == 1) return { a, C[a].probabilidad };
    
    double probActual = C[a].probabilidad;
    double probMedia = probTotal / 2;
    double diferencia = abs(probMedia - probActual);
    int indice = a;
    
    for (int i = a + 1; i <= b; i++) {
        if (abs(probActual + C[i].probabilidad - probMedia) <= diferencia) {
            probActual += C[i].probabilidad;
            diferencia = abs(probActual - probMedia);
        } else {
            indice = i - 1;
            break;
        }
    }
    
    return { indice, probActual };
}

// Función para codificar usando Shannon-Fano
void codificarSF(Elemento* C, int a, int b, double probTotal, int pot) {
    if (a >= b) return;
    int p;
    double probActual;
    tie(p, probActual) = divisorProbabilidad(C, a, b, probTotal);
    for (int i = a; i <= p; i++) {
        C[i].codigo += pow(2, pot);
    }
    
    if (b - a > 1) {
        if (p - a > 0) codificarSF(C, a, p, probActual, pot + 1);
        if (b - (p + 1) > 0) codificarSF(C, p + 1, b, probTotal - probActual, pot + 1);
    }
}

// Función para generar el diccionario DSF
unordered_map<short, short> generar_DSF(Elemento* C, int a, int n) {
    unordered_map<short, short> DSF;
    for (int i = a; i < n; ++i) {
        DSF[C[i].codigo] = C[i].simbolo;    
    }
    
    return DSF;
}

// Función para generar el arreglo GCSF
short* generar_GCSF(short* GC, unordered_map<short, short>& DSFC, unordered_map<short, short>& DSFO, int n) {
    unordered_map<short, short> inverted_DSFC;
    unordered_map<short, short> inverted_DSFO;
    short* GCSF = new short[n];
    
    // Invert the DSFC map
    for (const auto& pair : DSFC) {
        inverted_DSFC[pair.second] = pair.first;
    }
    
    // Invert the DSFO map
    for (const auto& pair : DSFO) {
        inverted_DSFO[pair.second] = pair.first;
    }
    
    for (int i = 0; i < n; i++) {
        if (inverted_DSFC.find(GC[i]) != inverted_DSFC.end()) {
            GCSF[i] = inverted_DSFC[GC[i]];
        } else {
            GCSF[i] = inverted_DSFO[GC[i]];
        }
    }
    
    return GCSF;
}

// Función para decodificar usando DSF
int decodificar_SF(unordered_map<short, short>& DSFC, unordered_map<short, short>& DSFO, int val) {
    if (val % 2 == 0) return DSFC[val];
    return DSFO[val];
}

int busqueda_binaria_shannon_fano(short* GCSF, int* sample, unordered_map<short, short> DSFC, unordered_map<short, short> DSFO, int n, int m, int val) {
    int izq = 0;
    int der = m - 1;
    int med, num;    
    int b = n / m;
    // Se ejecuta la búsqueda binaria en el sample
    while (izq <= der) {
        med = izq + (der - izq) / 2;
        if (sample[med] == val)
            return med * b;
            
        if (sample[med] < val)
            izq = med + 1;
        else 
            der = med - 1;
    }
    // Si no se encuentra el valor en el sample, continúa
    
    // Si med es 0, significa que el valor a buscar es menor
    // al primer valor del arreglo. Al estar este ordenado,
    // significa que el valor no se encuentra.
    if (med == 0) return -1;
    
    // Si el valor a buscar es menor que el que se encuentra en 
    // la posición med, entonces se corrige retrocediendo una 
    // posición. 
    //      0  1  2
    // Ej: [1, 3, 5], buscando el 4 retorna la posición 2
    // 4 < 5 entonces se continúa con la posición 1
    if (val < sample[med])
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
		num = num + decodificar_SF(DSFC, DSFO, GCSF[med]);
		med = med + 1;
	}
	
	return -1;
}

/*
void mostrar_codigo_binario(int codigo) {
    // Obtener los 8 bits más bajos del código
    int bits = codigo & 0xFF;
    
    // Mostrar los bits en formato binario
    cout << bitset<8>(bits) << " ";
}
*/
