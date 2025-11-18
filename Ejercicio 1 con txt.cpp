#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream> // Librería necesaria para archivos
using namespace std;
// --- FUNCIÓN PARA GUARDAR EN ARCHIVO ---
void guardarMatrizEnArchivo(const vector<vector<int>>& M, string nombreArchivo) {
    ofstream archivo(nombreArchivo); // Crea el archivo con el nombre dado

    if (archivo.is_open()) {
        for (int i = 0; i < M.size(); i++) {
            for (int j = 0; j < M[i].size(); j++) {
                archivo << M[i][j] << " ";
            }
            archivo << "\n";
        }
        archivo.close();
        cout << "Matriz guardada exitosamente en '" << nombreArchivo << "'" << endl;
    } else {
        cout << "Error: No se pudo crear el archivo." << endl;
    }
}
// Genera una matriz booleana aleatoria (0s y 1s)
vector<vector<int>> generarMatriz(int filas, int columnas) {
    vector<vector<int>> M(filas, vector<int>(columnas));

    for (int i = 0; i < filas; i++) {
        bool filaValida = false;
        while (!filaValida) {
            int suma = 0;
            for (int j = 0; j < columnas; j++) {
                M[i][j] = rand() % 2;
                suma += M[i][j];
            }
            if (suma > 0) filaValida = true;
        }
    }
    return M;
}

// Muestra una matriz en consola
void mostrarMatriz(const vector<vector<int>>& M) {
    for (int i = 0; i < M.size(); i++) {
        for (int j = 0; j < M[i].size(); j++) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

// Verifica si una fila A es subfila de otra fila B
bool esSubfila(const vector<int>& A, const vector<int>& B) {
    bool menorEstricto = false;
    for (int j = 0; j < A.size(); j++) {
        if (A[j] > B[j]) return false;
        if (A[j] < B[j]) menorEstricto = true;
    }
    return menorEstricto;
}

// Construye la matriz básica
vector<vector<int>> matrizBasica(const vector<vector<int>>& M) {
    vector<vector<int>> BM;
    for (int i = 0; i < M.size(); i++) {
        bool basica = true;
        for (int k = 0; k < M.size(); k++) {
            if (i != k && esSubfila(M[k], M[i])) {
                basica = false;
                break;
            }
        }
        if (basica) BM.push_back(M[i]);
    }
    return BM;
}

// Calcula la densidad
double densidad(const vector<vector<int>>& M) {
    if (M.empty()) return 0.0;
    int total = 0, unos = 0;
    for (int i = 0; i < M.size(); i++) {
        for (int j = 0; j < M[i].size(); j++) {
            total++;
            if (M[i][j] == 1) unos++;
        }
    }
    return (double)unos / total;
}

int main() {
    int filas = 0, columnas = 0;
    
    while(filas > 100 || filas < 1){
        cout << "Ingrese numero de filas (max 100): ";
        cin >> filas;   
    }
    
    while(columnas > 10 || columnas < 1){
        cout << "Ingrese numero de columnas (max 10): ";
        cin >> columnas;
    }

    srand(time(0)); 

    // 1. Matriz booleana
    auto M = generarMatriz(filas, columnas);
    cout << "\nMatriz booleana generada:\n";
    mostrarMatriz(M);

    // 2. Matriz básica
    auto BM = matrizBasica(M);
    cout << "\nMatriz basica:\n";
    mostrarMatriz(BM);

    // 3. Guardar en archivo (NOMBRE ACTUALIZADO)
    guardarMatrizEnArchivo(BM, "matriz.txt");

    // 4. Densidad
    cout << "\nDensidad de la matriz basica: " << densidad(BM) << endl;

    return 0;
}