#include <iostream>
#include <vector>
#include <fstream>  // Necesario para leer archivos
#include <sstream>  // Necesario para procesar texto
#include <string>   // Necesario para manejar strings

using namespace std;

int filas = 0, columnas = 0;
vector<vector<int>> BM;
vector<vector<int>> testores;
vector<vector<int>> tipicos;

// ----------------------------------------
// Verifica si un conjunto S (indices de columnas) es testor
// ----------------------------------------
bool esTestorConjunto(const vector<int>& S) {
    if (S.empty()) return false;

    for (int f = 0; f < filas; f++) {
        bool cubierta = false;
        for (int col : S) {
            if (BM[f][col] == 1) {
                cubierta = true;
                break;
            }
        }
        if (!cubierta) return false;
    }
    return true;
}

// ----------------------------------------
// Factibilidad: ¿vale la pena seguir extendiendo S?
// Si hay una fila que no esta cubierta por S y tampoco la podra
// cubrir ninguna columna futura (>= k), entonces se poda.
// ----------------------------------------
bool factible(const vector<int>& S, int k) {
    for (int f = 0; f < filas; f++) {
        bool posible = false;

        // ya cubierta por S?
        for (int col : S) {
            if (BM[f][col] == 1) {
                posible = true;
                break;
            }
        }

        // si no, ver si alguna columna futura la podria cubrir
        if (!posible) {
            for (int c = k; c < columnas; c++) {
                if (BM[f][c] == 1) {
                    posible = true;
                    break;
                }
            }
        }

        if (!posible) return false;
    }
    return true;
}

// ----------------------------------------
// Backtracking BT con poda (saltos)
// S: conjunto actual
// k: siguiente columna que se puede agregar (lexicografico)
// ----------------------------------------
void BT(vector<int>& S, int k) {
    // poda por factibilidad (salto de rama)
    if (!factible(S, k)) return;

    // si ya es testor, lo guardamos y NO expandimos supersets
    if (esTestorConjunto(S)) {
        testores.push_back(S);
        return;
    }

    // expandir agregando columnas posteriores
    for (int c = k; c < columnas; c++) {
        S.push_back(c);
        BT(S, c + 1);
        S.pop_back();
    }
}

// ----------------------------------------
// Es tipico si ningun subconjunto propio es testor
// ----------------------------------------
bool esTipicoConjunto(const vector<int>& S) {
    int n = S.size();
    if (n == 0) return false;

    int total = 1 << n; // 2^n subconjuntos

    for (int mask = 1; mask < total; mask++) {
        if (mask == total - 1) continue; // no el conjunto completo

        vector<int> sub;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                sub.push_back(S[i]);
            }
        }

        if (esTestorConjunto(sub)) return false;
    }

    return true;
}

// ----------------------------------------
// MAIN
// ----------------------------------------
int main() {
    cout << "=== Algoritmo BT (Backtracking con Poda) ===\n";
    cout << "Intentando leer 'matriz.txt'...\n";

    // --- LECTURA DE ARCHIVO ---
    ifstream archivo("matriz.txt");
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo 'matriz.txt'. Asegurese de haberlo generado.\n";
        return 1;
    }

    string linea;
    BM.clear();
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        vector<int> filaTemp;
        int valor;
        while (ss >> valor) {
            filaTemp.push_back(valor);
        }
        if (!filaTemp.empty()) {
            BM.push_back(filaTemp);
        }
    }
    archivo.close();

    // Configurar dimensiones globales
    filas = BM.size();
    if (filas > 0) {
        columnas = BM[0].size();
        cout << "Matriz cargada exitosamente: " << filas << " filas x " << columnas << " columnas.\n";
    } else {
        cout << "Error: La matriz del archivo esta vacia.\n";
        return 1;
    }
    // --- FIN LECTURA ---

    vector<int> S;
    BT(S, 0);  // empezar desde columna 0

    // sacar tipicos
    for (const auto& T : testores) {
        if (esTipicoConjunto(T)) {
            tipicos.push_back(T);
        }
    }

    cout << "\n=== TESTORES ENCONTRADOS ===\n";
    if (testores.empty()) {
        cout << "(No existe ningun testor)\n";
    } else {
        for (const auto& T : testores) {
            cout << "{ ";
            for (int col : T) {
                cout << char('A' + col) << " ";
            }
            cout << "}\n";
        }
    }

    cout << "\n=== TESTORES TIPICOS ===\n";
    if (tipicos.empty()) {
        cout << "(No existe ningun testor tipico)\n";
    } else {
        for (const auto& T : tipicos) {
            cout << "{ ";
            for (int col : T) {
                cout << char('A' + col) << " ";
            }
            cout << "}\n";
        }
    }

    return 0;
}
