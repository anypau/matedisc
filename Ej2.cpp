#include <iostream>
#include <vector>
#include <chrono>

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
    int filas, columnas;
    cout << "=== Algoritmo YYC ===\n";
    cout << "Ingrese numero de filas: ";
    cin >> filas;
    cout << "Ingrese numero de columnas: ";
    cin >> columnas;

    vector<vector<int>> MB(filas, vector<int>(columnas));
    cout << "\nIngrese la matriz basica (" << filas << "x" << columnas << "):\n";
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cin >> MB[i][j];
        }
    }

    auto inicio_total = high_resolution_clock::now();

    vector<vector<int>> testores;
    vector<vector<int>> testores_tipicos;

    // Recorre fila por fila (proceso incremental)
    for (int f = 1; f <= filas; f++) {
        testores_tipicos.clear();
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
