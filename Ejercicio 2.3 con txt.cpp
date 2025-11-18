#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace std::chrono;

bool esTestor(const vector<vector<int>>& MB, const vector<int>& columnas, int filas_usadas) {
    for (int i = 0; i < filas_usadas; i++) {
        bool tiene_uno = false;
        for (int c : columnas) {
            if (MB[i][c] == 1) {
                tiene_uno = true;
                break;
            }
        }
        if (!tiene_uno) return false;
    }
    return true;
}

bool esTipico(const vector<vector<int>>& MB, const vector<int>& columnas, int filas_usadas) {
    if (!esTestor(MB, columnas, filas_usadas)) return false;

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

    ifstream archivo("matriz.txt");
    if (!archivo.is_open()) {
        cout << "Error: No se encontro el archivo 'matriz.txt'.\n";
        return 1;
    }

    vector<vector<int>> MB;
    string linea;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        vector<int> filaTemp;
        int valor;
        while (ss >> valor) filaTemp.push_back(valor);
        MB.push_back(filaTemp);
    }
    archivo.close();

    int filas = MB.size();
    int columnas = MB[0].size();

    cout << "Matriz cargada: " << filas << " filas x "
         << columnas << " columnas.\n";

    auto inicio_total = high_resolution_clock::now();

    vector<vector<int>> testores_tipicos;
    vector<vector<int>> tipicos_finales;  // <--- AQUI GUARDAREMOS LOS ULTIMOS

    for (int f = 1; f <= filas; f++) {
        testores_tipicos.clear();

        for (int mask = 1; mask < (1 << columnas); mask++) {
            vector<int> subset;

            for (int j = 0; j < columnas; j++) {
                if (mask & (1 << j)) subset.push_back(j);
            }

            if (esTipico(MB, subset, f)) testores_tipicos.push_back(subset);
        }

        // Si estamos en la última fila, guardamos los típicos
        if (f == filas) {
            tipicos_finales = testores_tipicos;
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

    // ------------------------------------------------------
    // GUARDAR LOS TIPOICOS FINALES EN ARCHIVO
    // ------------------------------------------------------
    ofstream salida("tipicos.txt");
    if (!salida.is_open()) {
        cout << "Error al crear tipicos.txt\n";
        return 1;
    }

    cout << "\nGuardando testores tipicos finales en 'tipicos.txt'...\n";

    for (auto& t : tipicos_finales) {
        for (int j : t) salida << char('a' + j) << " ";
        salida << "\n";
    }

    salida.close();
    cout << "Archivo 'tipicos.txt' creado exitosamente.\n";

    cout << "\n=== Fin del proceso ===\n";
    return 0;
}
