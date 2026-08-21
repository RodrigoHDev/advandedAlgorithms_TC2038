#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Tu función ya declarada
vector<double> mergeSort(vector<double> arr);

int main() {
    vector<string> testFiles = {
        "test1.txt", "test2.txt", "test3.txt",
        "test4.txt", "test5.txt", "test6.txt"
    };

    for (const string &filename : testFiles) {

        ifstream file(filename);
        if (!file.is_open()) {
            cout << "No se pudo abrir: " << filename << endl;
            continue;
        }

        vector<double> numbers;
        int elements = 0;
        string line;

        while (getline(file, line)) {
            // Ignorar líneas de comentario/guía
            if (line.find("//") != string::npos) continue;
            if (line.empty()) continue;

            stringstream ss(line);

            if (elements == 0) {
                ss >> elements; // primera línea útil = cantidad de elementos
            } else {
                double value;
                ss >> value;
                numbers.push_back(value);
            }
        }

        vector<double> ordered = mergeSort(numbers);

        // Aquí ya tienes "ordered" listo para usar/verificar como necesites
    }

    return 0;
}