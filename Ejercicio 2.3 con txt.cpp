#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>  // Para leer archivos
#include <sstream>  // Para procesar las líneas del archivo
#include <string>   // Para manejar strings

using namespace std;
using namespace std::chrono;

bool esTestor(const vector<vector<int>>& MB, const vector<int>& columnas, int filas_usadas) {
    // Verifica si un conjunto de columnas es testor
    for (int i = 0; i < filas_usadas; i++) {
        bool tiene_uno = false;
        for (int c : columnas) {
            if (MB[i][c] == 1) {
                tiene_uno = true;
                break;
            }
        }
        if (!tiene_uno) return false; // si hay una fila con todos ceros
    }
    return true;
}

bool esTipico(const vector<vector<int>>& MB, const vector<int>& columnas, int filas_usadas) {
    if (!esTestor(MB, columnas, filas_usadas)) return false;

    // Revisa que no haya subconjunto que también sea testor
    for (size_t i = 0; i < columnas.size(); i++) {
        vector<int> sub = columnas;
        sub.erase(sub.begin() + i);
        if (esTestor(MB, sub, filas_usadas)) return false;
    }
    return true;
}

int main() {
    cout << "=== Algoritmo YYC ===\n";
    cout << "Leyendo 'matriz.txt'...\n";

    // --- INICIO DE LECTURA DE ARCHIVO ---
    ifstream archivo("matriz.txt");
    if (!archivo.is_open()) {
        cout << "Error: No se encontro el archivo 'matriz.txt'. asegurese de ejecutar el programa anterior primero.\n";
        return 1;
    }

    vector<vector<int>> MB;
    string linea;
    int filas = 0;
    int columnas = 0;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue; // Ignorar lineas vacias si las hay
        stringstream ss(linea);
        vector<int> filaTemp;
        int valor;
        while (ss >> valor) {
            filaTemp.push_back(valor);
        }
        if (!filaTemp.empty()) {
            MB.push_back(filaTemp);
        }
    }
    archivo.close();

    // Calculamos dimensiones basadas en lo leído
    filas = MB.size();
    if (filas > 0) {
        columnas = MB[0].size();
    } else {
        cout << "La matriz esta vacia.\n";
        return 0;
    }
    
    cout << "Matriz cargada: " << filas << " filas x " << columnas << " columnas.\n";
    // --- FIN DE LECTURA DE ARCHIVO ---


    auto inicio_total = high_resolution_clock::now();

    vector<vector<int>> testores;
    vector<vector<int>> testores_tipicos;

    // Recorre fila por fila (proceso incremental)
    for (int f = 1; f <= filas; f++) {
        testores_tipicos.clear();
        // Nota: Si columnas > 30, (1 << columnas) desbordará un int estándar. 
        // Para matrices pequeñas/medianas (como el ejercicio anterior max 10 col), esto funciona bien.
        for (int mask = 1; mask < (1 << columnas); mask++) {
            vector<int> subset;
            for (int j = 0; j < columnas; j++) {
                if (mask & (1 << j)) subset.push_back(j);
            }
            if (esTipico(MB, subset, f)) testores_tipicos.push_back(subset);
        }

        auto tiempo_actual = high_resolution_clock::now();
        double tiempo_ms = duration_cast<milliseconds>(tiempo_actual - inicio_total).count();

        cout << "\nFila " << f << ": Testores obtenidos\n";
        for (auto& t : testores_tipicos) {
            cout << "{ ";
            for (int j : t) cout << char('a' + j) << " ";
            cout << "} ";
        }
        cout << "\nTiempo acumulado: " << tiempo_ms << " ms\n";
    }

    cout << "\n=== Fin del proceso ===\n";
    return 0;
}
