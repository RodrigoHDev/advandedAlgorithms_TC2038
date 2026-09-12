
/*
 * Title: BranchBound.h
 * Description: Branch & Bound solver for a 0/1 knapsack-style selection
 * of indexes (space, value, id), maximizing total value without
 * exceeding a given storage capacity.
 *
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 */


#include <iostream>
#include <limits>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

/*
In order to analyze complexity:
n = amount (number of candidate indexes).
The recursion explores, in the worst case, a full binary decision tree
of depth n (include / exclude each index), so unpruned it would be
O(2^n). Branch & Bound pruning (via maxValue) cuts this in practice,
but the guaranteed worst-case bound remains exponential.
*/


// Restricciones del problema (ver enunciado):
// 1 <= N <= 30 ; 1 <= C <= 10000 ; 1 <= Ei <= C ; 1 <= Bi <= 100000.
static const int MIN_N = 1;
static const int MAX_N = 30;
static const int MIN_C = 1;
static const int MAX_C = 10000;
static const int MIN_B = 1;
static const int MAX_B = 100000;


class BranchBound {
public:



     //Default variables
     int capacity = 0;
     int amount = 0;
     vector<tuple<int,int,int>> indexes = {};

     //Branch & Bound variables
     int bestValue = 0;
     int bestSpace = 0;
     int value = 0;
     int occupiedSpace = 0;
     vector<tuple<int,int,int>> path = {};
     vector<tuple<int,int,int>> bestPath = {};

    // true si capacity, amount y cada tupla de indexes cumplen las
    // restricciones del problema; false en caso contrario (ver
    // validateInput()). explore() no hace nada si valid es false.
     bool valid = true;


     /*
    * validateInput()
    * Verifica que capacity, amount, y cada tupla (espacio, valor, id)
    * de indexes cumplan las restricciones del problema (1<=N<=30,
    * 1<=C<=10000, 1<=Ei<=C, 1<=Bi<=100000). Esta validacion es
    * defensiva: main.cpp ya valida la entrada antes de construir el
    * objeto, pero se repite aqui para que la clase sea segura de usar
    * de forma independiente.
    *
    * Complexity:
    *  Time:  O(n), where n = indexes.size() (recorre cada tupla una vez).
    *  Space: O(1) auxiliary.
    *
    * Params: none (usa los miembros capacity, amount e indexes ya asignados).
    * Returns: true si todo cumple las restricciones; false en caso
    *  contrario (e imprime "Invalid entry format").
    */
     bool validateInput(){
          if(amount < MIN_N || amount > MAX_N) return false;
          if(capacity < MIN_C || capacity > MAX_C) return false;
          if((int)indexes.size() != amount) return false;

          for(int k = 0; k < amount; k++){
               int espacio = get<0>(indexes[k]);
               int valor = get<1>(indexes[k]);
               if(espacio < 1 || espacio > capacity) return false;
               if(valor < MIN_B || valor > MAX_B) return false;
          }
          return true;
     }


     /*
    * maxValue()
    * Cota optimista (fractional knapsack bound) de lo maximo que se
    * podria acumular a futuro desde el nodo (i, obtainedValue,
    * occupiedSpace). Asume que 'indexes' ya viene ordenado
    * descendentemente por relacion valor/espacio (ver sortByRatio en
    * main.cpp) -- eso es lo que permite tomar avaramente los objetos
    * completos y, para el primero que no quepa entero, sumar solo la
    * fraccion que si cabe. Esa fraccion es una sobreestimacion valida
    * porque en la realidad no se pueden tomar fracciones de un indice,
    * asi que ninguna combinacion 0/1 real puede superarla.
    *
    * Sin esto (sumar solo combinaciones "que quepan" en el orden dado,
    * como en la version anterior), el valor devuelto podia ser una cota
    * inferior en vez de superior, y el algoritmo podia podar por error
    * ramas que contenian la solucion optima.
    *
    * Complexity:
    *   Time:  O(n - i), where n = amount (single pass over the
    *          remaining, not-yet-decided indexes; at most one of ellos
    *          aporta una fraccion antes de cortar).
    *   Space: O(1) auxiliary (only local accumulators; no extra structures).
    *
    * Params:
    *  i is the current index in the decision sequence (first index not
    *   yet decided).
    *  obtainedValue is the value accumulated so far along the current path.
    *  occupiedSpace is the space occupied so far along the current path.
    * Returns: an upper bound on the best value reachable from this node,
    *  used to prune branches that cannot possibly beat bestValue.
    */
     int maxValue(int i, int obtainedValue, long long occupiedSpace){
          for(; i<amount; i++){
               int espacio = get<0>(indexes[i]);
               int valor = get<1>(indexes[i]);

               if(occupiedSpace + espacio <= capacity){
                    obtainedValue += valor;
                    occupiedSpace += espacio;
               } else {
                    long long remaining = capacity - occupiedSpace;
                    if(remaining > 0){
                         // Fraccion del indice que si cabria: cota superior valida
                         // (piso hacia el entero, sigue siendo >= cualquier OPT entero).
                         obtainedValue += (int)((long long)valor * remaining / espacio);
                    }
                    break; // el resto, ordenado por ratio, aporta aun menos por unidad de espacio
               }
          }
          return obtainedValue;
     }

     /**
     * Builds a BranchBound object over the given candidate indexes and
     * validates it against the problem's restrictions.
     *
     * Complexity:
     *   Time:  O(n), where n = indexes_.size() (copies the vector into
     *          the member variable, then validateInput() scans it once more).
     *   Space: O(n) auxiliary, for the stored copy of indexes_.
     */
     BranchBound(int capacity_, int amount_, vector<tuple<int,int,int>> indexes_){
          capacity = capacity_;
          amount = amount_;
          indexes = indexes_;

          valid = validateInput();
          if(!valid){
               cout << "Invalid entry format" << endl;
          }
     }


    /**
     * explore()
     * Recursively builds the decision tree (include/exclude each index),
     * pruning branches that exceed capacity or whose optimistic bound
     * (maxValue) cannot beat the best value found so far.
     *
     * Complexity:
     *  Time: O(n * 2^n) worst case, where n = amount — at each of the
     *   up to 2^n nodes of the decision tree, maxValue() does an O(n)
     *   scan. In practice, pruning (helped by sorting indexes by
     *   value/space ratio beforehand) visits far fewer nodes.
     *  Space: O(n) auxiliary, for the recursion call stack (depth n)
     *   plus the path/bestPath vectors (each up to size n).
     *
     * Params:
     *  i is the index currently being decided (include or exclude).
     *  obtainedValue is the value accumulated so far along the current path.
     *  occupiedSpace is the space occupied so far along the current path.
     * Returns: none (updates bestValue, bestSpace and bestPath in place).
     */
     void explore(int i, int obtainedValue, int occupiedSpace){
          if(!valid) return; 

          if(i == amount){
               if(obtainedValue > bestValue){
                    bestValue = obtainedValue;
                    bestSpace = occupiedSpace;
                    bestPath = path;
                    return;
               }
          }

          int maxVal = maxValue(i, obtainedValue, (long long)occupiedSpace);
          if(maxVal <= bestValue)
               return ;

          if(occupiedSpace + get<0>(indexes[i]) <= capacity){
               path.push_back(indexes[i]);
               explore(i+1, obtainedValue + get<1>(indexes[i]), occupiedSpace + get<0>(indexes[i]));
               path.pop_back();
          }

          explore(i+1, obtainedValue, occupiedSpace);
     }

};



/*
* Due to submition motifs both designed files have been added here.

 * Title: main.cpp
 * Description: Reads a set of candidate indexes (space, benefit) and a
 * storage capacity, then uses Branch & Bound (BranchBound.h) to select
 * the subset of indexes that maximizes total benefit without exceeding
 * the capacity.
 *
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <tuple>

using namespace std;

/*
In order to analyze complexity:
n = indices (number of candidate indexes read from input).
*/

// Restricciones del problema (definidas en BranchBound.h, incluido
// arriba): MIN_N, MAX_N, MIN_C, MAX_C, MIN_B, MAX_B.

/*
 * isValidHeader()
 * Verifica que N (cantidad de indices) y C (capacidad) cumplan
 * 1<=N<=30 y 1<=C<=10000.
 *
 * Complexity:
 *  Time:  O(1).
 *  Space: O(1) auxiliary.
 *
 * Params:
 *  n es la cantidad de indices candidatos leida de la entrada.
 *  c es la capacidad maxima de almacenamiento leida de la entrada.
 * Returns: true si n y c cumplen las restricciones; false en caso contrario.
 */
bool isValidHeader(int n, int c){
     return n >= MIN_N && n <= MAX_N && c >= MIN_C && c <= MAX_C;
}


/*
 * isValidItem()
 * Verifica que un indice individual cumpla 1<=E<=C y 1<=B<=100000.
 *
 * Complexity:
 *  Time:  O(1).
 *  Space: O(1) auxiliary.
 *
 * Params:
 *  espacio es el espacio (E) que ocupa el indice, en MB.
 *  valor es el beneficio (B) estimado del indice.
 *  capacidad es la capacidad maxima C, contra la que se acota espacio.
 * Returns: true si espacio y valor cumplen las restricciones; false en
 *  caso contrario.
 */
bool isValidItem(int espacio, int valor, int capacidad){
     return espacio >= 1 && espacio <= capacidad && valor >= MIN_B && valor <= MAX_B;
}

/*
 * sortByRatio()
 * Ordena un vector de tuplas (espacio, valor, indice) de forma
 * descendente según la relación beneficio/espacio (valor/espacio).
 *
 * Se evita la división directa (valor/espacio) para no perder precisión
 * con enteros ni arriesgar división por cero; en su lugar se comparan
 * las razones mediante multiplicación cruzada:
 *      valorA/espacioA > valorB/espacioB  <=>  valorA*espacioB > valorB*espacioA
 *
 * Complexity:
 *  Time:  O(n log n), where n = indexes.size() (std::sort with a
 *   constant-time comparator).
 *  Space: O(log n) auxiliary, for std::sort's internal recursion
 *   (sorting is done in place on the given vector; no extra containers
 *   are allocated).
 *
 * Params:
 *  indexes is a vector of tuples (espacio, valor, indice original) to
 *   be reordered in place.
 * Returns: none (reorders indexes by reference).
 */
void sortByRatio(vector<tuple<int,int,int>>& indexes){
     sort(indexes.begin(), indexes.end(),
          [](const tuple<int,int,int>& a, const tuple<int,int,int>& b){
               long long espacioA = get<0>(a), valorA = get<1>(a);
               long long espacioB = get<0>(b), valorB = get<1>(b);
               return valorA * espacioB > valorB * espacioA; // descendente
          });
}


/*
 * sortByIndex()
 * Ordena un vector de tuplas (espacio, valor, indice) de forma
 * descendente según su indice original (tercer valor de la tupla), para
 * poder recuperar el orden de entrada al momento de imprimir el
 * resultado.
 *
 * Complexity:
 *  Time:  O(k log k), where k = indexes.size() (in this program, the
 *   size of the selected path, k <= n).
 *  Space: O(log k) auxiliary, for std::sort's internal recursion.
 *
 * Params:
 *  indexes is a vector of tuples (espacio, valor, indice original) to
 *   be reordered in place.
 * Returns: none (reorders indexes by reference).
 */
void sortByIndex(vector<tuple<int,int,int>>& indexes){
     sort(indexes.begin(), indexes.end(),
          [](const tuple<int,int,int>& a, const tuple<int,int,int>& b){
               long long espacioA = get<2>(a);
               long long espacioB = get<2>(b);
               return espacioB > espacioA; // descendente
          });
}


/*
main()

Reads N candidate indexes and capacity C, validating both the header
and each item against the problem's restrictions (see isValidHeader /
isValidItem). On any violation -- including non-numeric input -- it
prints "Invalid entry format" and stops. Otherwise it sorts the items
by value/space ratio to strengthen Branch & Bound pruning, runs the
search, and prints the maximum benefit, space used, and selected index
identifiers (restored to input order, 1-indexed).

Complexity:
 Time: O(n * 2^n) worst case, where n = indices — dominated by
  BranchBound::explore (see BranchBound.h); reading and validating
  input is O(n), sortByRatio is O(n log n), and sortByIndex on the
  result path is O(k log k) with k <= n, all dominated by the search
  itself.
 Space: O(n) auxiliary, for the indic vector and the BranchBound
  instance's internal vectors (indexes, path, bestPath).
*/
int main(){
     int indices = 0;
     int capacidad = 0;

     if(!(cin >> indices >> capacidad)){
          cout << "Invalid entry format" << endl;
          return 1;
     }
     if(!isValidHeader(indices, capacidad)){
          cout << "Invalid entry format" << endl;
          return 1;
     }
     cin.ignore();

	vector<tuple<int,int,int>> indic(indices);
     int espacio = 0;
     int valor = 0;
	for (int i = 0; i < indices; i++) {
		if(!(cin >> espacio >> valor)){
               cout << "Invalid entry format" << endl;
               return 1;
          }
          cin.ignore();

          if(!isValidItem(espacio, valor, capacidad)){
               cout << "Invalid entry format" << endl;
               return 1;
          }

          tuple<int,int,int> tup(espacio, valor, i);
          indic[i] = tup;
	}

     sortByRatio(indic);

     BranchBound bb =  BranchBound(capacidad, indices, indic);
     if(!bb.valid){
          // BranchBound ya imprimio "Invalid entry format" en su constructor
          return 1;
     }
     bb.explore(0,0,0);

     cout << "Beneficio maximo: " << bb.bestValue << endl;
     cout << "Espacio utilizado: " << bb.bestSpace << " MB" << endl;
     cout << "Indices seleccionados: ";
     sortByIndex(bb.bestPath);
     for(int i = 0; i<bb.bestPath.size(); i++){
          cout << get<2>(bb.bestPath[i])+1 << " ";
     }

	return 0;
}


//-------------------------------------------------------------------------------------

