# Resolucion de laberintos: Backtracking y Ramificacion y Poda

**Autores:**

- Alexis Yaocalli Berthou Haas - A01713458
- Rodrigo Alejandro Hurtado Cortes - A01713854

## Proposito

El programa recibe un laberinto de `M` filas por `N` columnas. Una celda con
`1` es transitable y una con `0` es una pared. El origen es `(0, 0)` y la
meta es `(M-1, N-1)`. Se imprime primero el laberinto original y despues una
solucion obtenida con cada tecnica.

Los movimientos son ortogonales y una celda no puede repetirse dentro del
mismo camino. Si no existe solucion, se muestra `NO VALID SOLUTION`.

## Tecnicas utilizadas

### Backtracking

`Backtracking::findWay` construye un camino de forma recursiva. Marca la
celda actual, intenta los movimientos en el orden **abajo, derecha, izquierda
y arriba**, y desmarca la celda si ninguna alternativa llega a la meta. La
primera ruta valida que encuentra se imprime como solucion.

- Tiempo en el peor caso: `O(3^(M*N))`.
- Espacio auxiliar: `O(M*N)` por las matrices `visited` y `solution`, mas la
  pila recursiva.

### Ramificacion y poda

`branchBound::findWay` explora caminos simples y conserva la ruta mas corta
encontrada. Cuando el costo de una ruta parcial ya es igual o mayor que el de
la mejor ruta conocida, se descarta: esa es la poda. El orden de avance es
**abajo, arriba, izquierda y derecha**.

- Tiempo en el peor caso: `O(3^(M*N))`.
- Espacio auxiliar: `O(M*N)` por `visited`, las pilas de rutas y la recursion.

En ambos casos, `M` es el numero de filas y `N` el de columnas. Las funciones
tambien incluyen su complejidad en los comentarios del codigo.

> **Nota sobre la misma complejidad:** aunque Ramificacion y Poda suele ser
> mas rapida en la practica porque descarta rutas cuyo costo ya no puede
> mejorar la mejor solucion encontrada, en el peor caso puede tardar en hallar
> esa primera solucion o enfrentar muchas rutas de costo similar. Entonces la
> poda casi no reduce las exploraciones y ambas tecnicas conservan la cota
> exponencial `O(3^(M*N))`.

## Formato de entradaPropósito

Poner en práctica la técnica de programación de "backtracking" y la de "ramificación y poda"
Instrucciones

Investiga sobre el uso de backtracking y poda para aplicaciones de laberintos.

Utilizando la técnica de programación de "backtracking" y/o la de "ramificación y poda", escribe en C++ un programa que resuelva un laberinto.

El programa recibe dos números enteros M y N, seguido de M líneas de N valores booleanos(0|1) separados por un espacio, por la entrada estándar que representan el laberinto. Un 1 representa una casilla en la que es posible moverse, un 0 es una casilla por la que NO se puede pasar. 
El origen o casilla de inicio es siempre la casilla (0,0) y la salida o meta es siempre la casilla (M-1, N-1).

La salida del programa es una matriz de valores booleanos (0|1) que representan el camino para salir del laberinto. Mostrar la solución utilizando la técnica de backtracking, o utilizando la técnica de ramificación y poda. Especifica cuál de las dos estás utilizando.

Muestra en pantalla el laberinto inicial y después las soluciones encontradas.

Ejemplo de entrada:
4
4
1 0 0 0
1 1 0 1
0 1 0 0
1 1 1 1

Ejemplo de salida:

Backtracking
1 0 0 0
1 1 0 0
0 1 0 0
0 1 1 1

Ramificación y poda
1 0 0 0
1 1 0 0
0 1 0 0
0 1 1 1

Algunos ejemplos y consideraciones:

    Casos_prueba_Back_Poda.txtDescargar Casos_prueba_Back_Poda.txt
    Backtracking_0.txtDescargar Backtracking_0.txt
    Backtracking_1.txtDescargar Backtracking_1.txt
    Backtracking_2.txtDescargar Backtracking_2.txt
    En los ejemplos: señalar error en caso de que se ingresen datos diferentes a los enteros positivos M y N, y a los valores booleanos, según corresponda.
    Indica en los comentarios, el criterio de avance, ejemplo: hacia el frente y hacia abajo, etc. 

 

Sube un archivo ZIP que se llame A18_Backtracking_Equipo_XX (donde las XX se sustituyen por el número de equipo correspondiente a los integrantes del equipo).
     El archivo ZIP contiene una carpeta (folder) llamada <A18_Backtracking_Equipo_XX>, y dentro se encuentra el archivo "main.cpp"
     Es posible tener más archivos dentro de la carpeta.
Evaluación

Tu programa debe compilar sin errores ni warnings.

Piensa en posibles casos de prueba extremos que pueden ser utilizados para probar tu programa.

Tu solución propuesta debe ser correcta y eficiente.

Para obtener el 100% de los puntos de esta actividad, tu programa:

    80% - Lista de 4 casos de prueba para cada una de las funcionalidades donde para cada una se evaluará:
        Excelente (80%) - evalúa correctamente los 4 casos de prueba.
        Muy Bien (60%) - evalúa correctamente 3 casos de prueba.
        Bien (40%) - evalúa correctamente 2 casos de prueba
        Insuficiente (20%) - evalúa correctamente 1 o 0 casos de prueba.

    10% - El código deberá seguir los lineamientos estipulados en el estándar de codificaciónDescargar estándar de codificación.
    10% - Especifican en cada uno de las funcionalidades la complejidad computacional como parte de su documentación.


```text
M N
v11 v12 ... v1N
...
vM1 vM2 ... vMN
```

`M` y `N` deben ser enteros positivos; cada valor del laberinto debe ser `0`
o `1`. El programa reporta un error ante dimensiones, valores o entrada
incompleta invalidos.

## Compilar y ejecutar

En Windows (CMD):

```cmd
g++ main.cpp -o maze.exe
maze.exe < input\input1.txt
```

En Bash:

```bash
g++ main.cpp -o maze
./maze < input/input1.txt
```

## Casos de prueba

Se incluyen cuatro casos funcionales en la carpeta `input/`. Para cada uno se
debe comprobar que el camino impreso inicia en `(0,0)`, termina en `(M-1,N-1)`,
solo usa celdas con valor `1` y avanza ortogonalmente.

| Archivo | Caso que cubre | Resultado esperado |
| --- | --- | --- |
| `input1.txt` | Ruta con desvio por paredes | Ambas tecnicas encuentran una ruta valida. |
| `input2.txt` | Laberinto 6x6 con varias bifurcaciones | Ambas tecnicas encuentran una ruta valida; ramificacion y poda devuelve una ruta mas corta. |
| `input3.txt` | No existe conexion entre origen y meta | Ambas tecnicas muestran `NO VALID SOLUTION`. |
| `input4.txt` | Laberinto rectangular 7x9 con obstaculos | Ambas tecnicas encuentran una ruta valida. |

Pruebas adicionales recomendadas para la validacion de entrada:

```text
0 2
```

Debe indicar que las dimensiones deben ser positivas.

```text
2 2
1 2
1 1
```

Debe indicar que los valores del laberinto deben ser `0` o `1`.

## Estructura

- `main.cpp`: lee, valida e imprime el laberinto y los resultados.
- `backtracking.h`: implementacion de Backtracking.
- `branchBound.h`: implementacion de Ramificacion y Poda.
- `input/`: los cuatro casos de prueba funcionales.
