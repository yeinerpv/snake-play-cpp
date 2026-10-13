# Snake Play 🐍

Juego de la serpiente por turnos para **dos jugadores**, desarrollado en **C++** como aplicación de consola para Windows.

Proyecto del curso *Introducción a la Programación* (00831) del Diplomado en Sistemas de Computación de la Universidad Estatal a Distancia (UNED), Costa Rica, II cuatrimestre 2026.

## Funcionalidades

- **Menú principal** con cinco opciones: instrucciones, registro de jugadores, iniciar juego, tabla de puntuaciones y salir.
- **Registro de jugadores** con validación de identificación (9 dígitos) y nombre completo (nombre y dos apellidos, con soporte para tildes y ñ). Permite registrar, modificar y eliminar jugadores.
- **Partida de 10 rondas** con turnos alternos sobre un tablero de 10 × 10. La serpiente y la comida aparecen en posiciones aleatorias y se controlan con las flechas del teclado.
- **Manejo de excepciones**: si la serpiente choca contra el borde, se lanza una excepción y el jugador pierde el turno.
- **Persistencia en archivos de texto**: los jugadores y los puntajes se guardan en `jugadores.txt` y `puntajes.txt`.
- **Tabla de puntuaciones acumuladas** por jugador, con la fecha de la última partida.

## Conceptos aplicados

| Concepto | Dónde se usa |
| --- | --- |
| Funciones con prototipos | Todas las funciones se declaran antes de `main` y se definen después |
| Arreglos unidimensionales | Puntos de cada jugador (`int puntos[2]`) |
| Arreglos bidimensionales | Tablero de juego (`char tablero[10][10]`) |
| Estructuras y vectores | `struct Posicion` y `vector<Posicion>` para la serpiente |
| Excepciones | `throw` / `try` / `catch` al chocar con el borde y al abrir archivos |
| Archivos | `ifstream` y `ofstream` para leer, agregar y reescribir registros |
| Recursividad | Búsqueda recursiva de un jugador en la tabla de puntajes acumulados (`buscarEnArreglo`) |

## Cómo ejecutarlo

**Requisitos:** Windows y un compilador de C++ (por ejemplo, MinGW incluido con Code::Blocks).

**Con Code::Blocks:**

1. Abra el archivo `YeinerPerezVargasProyectoSnake.cbp`.
2. Presione **F9** (*Build and run*).

**Desde la terminal:**

```bash
g++ main.cpp -o snake.exe
snake.exe
```

Los archivos `jugadores.txt` y `puntajes.txt` se crean automáticamente al registrar el primer jugador y jugar la primera partida.

## Cómo se juega

1. Registre al menos dos jugadores.
2. Elija **Iniciar juego** y seleccione a los dos participantes.
3. En cada turno, mueva la serpiente (`0`) con las flechas hasta la comida (`*`). Tiene 10 movimientos.
4. Comer la comida suma un punto. Chocar contra el borde hace perder el turno.
5. Al terminar las 10 rondas, el ganador recibe 2 puntos. En caso de empate, cada jugador recibe 1 punto.

## Autor

**Yeiner Pérez Vargas**, estudiante de Sistemas de Computación en la UNED.
[github.com/yeinerpv](https://github.com/yeinerpv) · [yeinerpv@gmail.com](mailto:yeinerpv@gmail.com)
