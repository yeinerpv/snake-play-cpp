/*
 * Autor: Yeiner Pérez Vargas
 * Fecha: Inicio 20 de julio 2026
 * Curso: Introducción a la Programación - 00831
 * Proyecto 2 - II Cuatrimestre 2026
 *
 * Descripción: Juego de serpiente (Snake Play) para dos jugadores con
 * turnos alternos. Menu principal, registro de jugadores,
 * partida con tablero de 10x10, manejo de excepciones para chocar
 * contra el borde, y almacenamiento de jugadores y puntajes en
 * archivos de texto (jugadores.txt y puntajes.txt).
 *
 * Declaración de uso de IA: Se utilizó Claude (Anthropic) como apoyo
 * para revisar lógica, detectar errores y mejorar la estructura del
 * código. La programación, comprensión, adaptación y defensa del
 * proyecto son de autoría propia del estudiante.
 */

#include <iostream>
#include <fstream>    // para ifstream / ofstream
#include <string>
#include <cctype>     // para isdigit(), isalpha(), toupper()
#include <cstdio>     // para remove() y rename()
#include <conio.h>    // para _getch()
#include <cstdlib>    // para rand() y srand()
#include <ctime>      // para time()
#include <windows.h>  // para Sleep()
#include <limits>     // para numeric_limits (limpiar buffer de cin)
#include <iomanip>    // para setw() y left (formato de tabla)
#include <vector>     // para vector<string> (listas de tamano dinamico)
#include <stdexcept>  // para runtime_error
using namespace std;

// ================== ESTRUCTURAS ==================

struct Posicion {
    int fila;
    int columna;
};

// ================== CONSTANTES ==================
const int FILAS = 10;
const int COLUMNAS = 10;
const int MAX_MOVIMIENTOS = 10;
const int NUM_RONDAS = 10;

// ================== PROTOTIPOS: Menu principal ==================
void mostrarMenuPrincipal();
int leerOpcionMenu();

// ================== PROTOTIPOS: Instrucciones del juego ==================
void mostrarInstrucciones();

// ================== PROTOTIPOS: Registro de jugadores ==================
char leerOpcionValida(string mensaje, string opcionesValidas);
bool validarIdentificacion(string id);
bool esLetraValida(unsigned char c);
bool validarNombre(string nombre);
void guardarJugador(string id, string nombre);
bool buscarJugador(string idBuscada, string &nombreEncontrado);
void actualizarArchivoJugadores(string idBuscada, string nuevoNombre, bool eliminar);
void registrarJugador();

// ================== PROTOTIPOS: Seleccion de jugadores ==================
bool esNumeroValido(string texto);
void mostrarListaJugadores(vector<string> &nombresDisponibles, vector<string> &idsDisponibles);
void iniciarJuego();

// ================== PROTOTIPOS: Puntajes ==================
string obtenerFechaActual();
void guardarPuntaje(string id, int puntaje);
void calcularPuntajesFinales(int puntos[2], int &puntajeFinalJ1, int &puntajeFinalJ2);

// ================== PROTOTIPOS: Tabla de puntuaciones ==================
int buscarEnArreglo(vector<string> &idsUnicos, string idBuscado, int desde);
void calcularPuntajesAcumulados(vector<string> &idsUnicos, vector<string> &fechasUltimas, vector<int> &puntajesAcumulados);
void mostrarTablaPuntuaciones();

// ================== PROTOTIPOS: Logica del juego (tablero, turno, partida) ==================
void inicializarTablero(char tablero[FILAS][COLUMNAS]);
void generarPosicion(int &fila, int &columna);
void imprimirTablero(char tablero[FILAS][COLUMNAS]);
bool posicionesIguales(int filaSerpiente, int columnaSerpiente, int filaComida, int columnaComida);
bool esTeclaValida(int tecla);
void moverSerpiente(int tecla, int &fila, int &columna);
bool jugarTurno(char tablero[FILAS][COLUMNAS], vector<Posicion> &serpiente, int filaComida, int columnaComida, string idJugador, string nombreJugador);
void jugarPartida(char tablero[FILAS][COLUMNAS], int puntos[2], string idJ1, string nombreJ1, string idJ2, string nombreJ2);

// ================== MAIN ==================
int main() {


    system("chcp 65001 > nul");

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(time(0));

    int opcion;

    do {
        mostrarMenuPrincipal();
        opcion = leerOpcionMenu();

        switch (opcion) {
            case 1:
                mostrarInstrucciones();
                break;
            case 2:
                registrarJugador();
                break;
            case 3:
                iniciarJuego();
                break;
            case 4:
                mostrarTablaPuntuaciones();
                break;
            case 5:
                system("cls");
                cout << "Gracias por jugar Snake Play. Hasta pronto!" << endl;
                Sleep(2000);
                break;
        }

    } while (opcion != 5);

    return 0;
}

// ================== DEFINICIONES: Menu principal ==================

// Imprime el menu principal con sus 5 opciones (no imprime nada mas).
void mostrarMenuPrincipal() {

    system("cls");
    cout << "===================================================" << endl;
    cout << "            SNAKE PLAY - MENÚ PRINCIPAL" << endl;
    cout << "===================================================" << endl << endl;

    cout << "1. Instrucciones del Juego" << endl;
    cout << "2. Registrar jugadores" << endl;
    cout << "3. Iniciar juego" << endl;
    cout << "4. Tabla de puntuaciones" << endl;
    cout << "5. Salir" << endl << endl;
}

// Lee la opcion del menu (1-5) validando que sea numerica y este en rango.
int leerOpcionMenu() {

    string entrada;
    int opcion = -1;
    bool valida = false;

    do {
        cout << "Seleccione una opción (1-5): ";
        getline(cin, entrada);

        valida = false;
        if (esNumeroValido(entrada)) {
            opcion = stoi(entrada);
            if (opcion >= 1 && opcion <= 5) {
                valida = true;
            }
        }

        if (!valida) {
            cout << "Opción inválida, vuelva a intentarlo." << endl;
            Sleep(2000);
            mostrarMenuPrincipal();   // limpia pantalla y redibuja el menu
        }

    } while (!valida);

    return opcion;
}


void mostrarInstrucciones() {

    system("cls");
    cout << "===================================================" << endl;
    cout << "            INSTRUCCIONES DEL JUEGO SNAKE" << endl;
    cout << "===================================================" << endl << endl;

    cout << "1. Participan dos jugadores por cada partida." << endl;
    cout << "2. Cada jugador tendrá 10 turnos en forma alterna." << endl;
    cout << "3. Cada jugador moverá la serpiente (0) con las flechas" << endl;
    cout << "   del teclado para que llegue hasta la comida (*)." << endl;
    cout << "4. Si la serpiente toca la comida: Gana un punto y se" << endl;
    cout << "   cambia el turno." << endl;
    cout << "5. Si la serpiente choca contra el borde del terreno," << endl;
    cout << "   pierde el turno (sin puntos) y continúa el otro" << endl;
    cout << "   jugador." << endl;
    cout << "6. Debes pensar rápido y moverte con cuidado." << endl;
    cout << "7. Al finalizar los 10 turnos, gana el jugador con mayor" << endl;
    cout << "   puntaje. Si ambos tienen la misma puntuación, se" << endl;
    cout << "   registra un empate." << endl << endl;

    cout << "Detalles adicionales:" << endl;
    cout << " - El tablero es una matriz de 10x10 casillas." << endl;
    cout << " - La serpiente inicia en una posición aleatoria." << endl;
    cout << " - La comida aparece en una posición aleatoria en cada turno." << endl;
    cout << " - Los puntos acumulados se guardan en la tabla de puntuaciones." << endl << endl;

    cout << "Presione cualquier tecla para volver al menú..." << endl;
    _getch();
}

// ================== DEFINICIONES: Registro de jugadores ==================

// Pide un caracter y repite hasta que este dentro de opcionesValidas (ej. "SN").
char leerOpcionValida(string mensaje, string opcionesValidas) {

    char opcion;
    bool valida = false;

    do {
        cout << mensaje;
        cin >> opcion;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        opcion = toupper(opcion);

        valida = false;
        for (int i = 0; i < opcionesValidas.length(); i++) {
            if (opcion == opcionesValidas[i]) {
                valida = true;
            }
        }

        if (!valida) {
            cout << "Opción inválida, vuelva a intentarlo." << endl;
            Sleep(2000);
        }

    } while (!valida);

    return opcion;
}

// Verifica que la identificacion tenga exactamente 9 digitos numericos.
bool validarIdentificacion(string id) {

    if (id.length() != 9) {
        return false;
    }

    for (int i = 0; i < id.length(); i++) {
        if (!isdigit(id[i])) {
            return false;
        }
    }

    return true;
}

// Determina si un caracter es letra, incluyendo tildes/enie en UTF-8.
bool esLetraValida(unsigned char c) {
    // isalpha() cubre las letras normales sin tilde (a-z, A-Z).
    // Cualquier byte >= 128 es parte de una letra acentuada o ñ/Ñ/ü/Ü
    // codificada en UTF-8 (esas letras usan 2 bytes cada una).
    if (isalpha(c)) {
        return true;
    }

    if (c >= 128) {
        return true;
    }

    return false;
}

// Valida que el nombre tenga exactamente 3 palabras (nombre + 2 apellidos).
bool validarNombre(string nombre) {

    if (nombre.length() == 0) {
        return false;
    }

    int contadorPalabras = 0;
    bool dentroDePalabra = false;

    for (int i = 0; i < nombre.length(); i++) {

        if (nombre[i] == '\n' || nombre[i] == '\r') {
            return false;
        }

        if (nombre[i] == ' ') {
            if (i == 0 || nombre[i - 1] == ' ') {
                return false;
            }
            dentroDePalabra = false;
        }
        else if (esLetraValida((unsigned char)nombre[i])) {
            if (!dentroDePalabra) {
                contadorPalabras++;
                dentroDePalabra = true;
            }
        }
        else {
            return false;
        }
    }

    if (nombre[nombre.length() - 1] == ' ') {
        return false;
    }

    return (contadorPalabras == 3);   // nombre + dos apellidos, exactamente 3
}

// Agrega un jugador nuevo al final de jugadores.txt.

void guardarJugador(string id, string nombre) {

    try {
        ofstream archivo("jugadores.txt", ios::app);

        if (!archivo.is_open()) {
            throw runtime_error("No se pudo abrir jugadores.txt");
        }

        archivo << id << "," << nombre << endl;
        archivo.close();
    }
    catch (runtime_error &e) {
        cout << "Error al guardar: " << e.what() << endl;
    }
}

// Busca un id en jugadores.txt; si existe, devuelve su nombre por referencia.
bool buscarJugador(string idBuscada, string &nombreEncontrado) {

    ifstream archivo("jugadores.txt");
    string linea;

    if (!archivo.is_open()) {
        return false;
    }

    while (getline(archivo, linea)) {

        int posicionComa = linea.find(',');
        string idLinea = linea.substr(0, posicionComa);
        string nombreLinea = linea.substr(posicionComa + 1);

        if (idLinea == idBuscada) {
            nombreEncontrado = nombreLinea;
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

// Reescribe jugadores.txt modificando o eliminando el registro indicado.
void actualizarArchivoJugadores(string idBuscada, string nuevoNombre, bool eliminar) {

    ifstream archivoOriginal("jugadores.txt");
    ofstream archivoTemporal("temporal.txt");

    string linea;

    while (getline(archivoOriginal, linea)) {

        int posicionComa = linea.find(',');
        string idLinea = linea.substr(0, posicionComa);

        if (idLinea == idBuscada) {
            if (!eliminar) {
                archivoTemporal << idLinea << "," << nuevoNombre << endl;
            }
        }
        else {
            archivoTemporal << linea << endl;
        }
    }

    archivoOriginal.close();
    archivoTemporal.close();

    remove("jugadores.txt");
    rename("temporal.txt", "jugadores.txt");
}

// Opcion 2: pide id/nombre y decide registrar, modificar o eliminar.
void registrarJugador() {

    bool volverAlMenu = false;

    do {
        string id, nombre, nombreEncontrado;
        bool idValida = false;

        do {
            system("cls");
            cout << "=== REGISTRAR JUGADORES ===" << endl << endl;
            cout << "Ingrese identificación (9 dígitos): ";
            getline(cin, id);

            idValida = validarIdentificacion(id);

            if (!idValida) {
                cout << "Identificación INVÁLIDA. Debe tener exactamente 9 dígitos." << endl;
                Sleep(2000);
            }

        } while (!idValida);

        bool existe = buscarJugador(id, nombreEncontrado);

        if (!existe) {
            bool nombreValido = false;

            do {
                system("cls");
                cout << "=== REGISTRAR JUGADORES ===" << endl << endl;
                cout << "Identificación: " << id << endl;
                cout << "Ingrese nombre completo (nombre y dos apellidos): ";
                getline(cin, nombre);

                nombreValido = validarNombre(nombre);

                if (!nombreValido) {
                    cout << "Nombre INVÁLIDO. Debe ingresar nombre y dos apellidos." << endl;
                    Sleep(2000);
                }

            } while (!nombreValido);

            guardarJugador(id, nombre);

            system("cls");
            cout << "Jugador registrado correctamente." << endl;
            Sleep(2000);
        }
        else {
            char opcion;

            system("cls");
            cout << "=== REGISTRAR JUGADORES ===" << endl << endl;
            cout << "Advertencia: jugador ya existe -> " << nombreEncontrado << endl << endl;
            opcion = leerOpcionValida("Desea Modificar (M), Eliminar (E) o Regresar (S)? ", "MES");

            if (opcion == 'M') {
                bool nombreValido = false;

                do {
                    system("cls");
                    cout << "Ingrese el nuevo nombre completo (nombre y dos apellidos): ";
                    getline(cin, nombre);

                    nombreValido = validarNombre(nombre);

                    if (!nombreValido) {
                        cout << "Nombre INVÁLIDO. Debe ingresar nombre y dos apellidos." << endl;
                        Sleep(2000);
                    }

                } while (!nombreValido);

                actualizarArchivoJugadores(id, nombre, false);

                system("cls");
                cout << "Jugador modificado correctamente." << endl;
                Sleep(2000);
            }
            else if (opcion == 'E') {
                actualizarArchivoJugadores(id, "", true);

                system("cls");
                cout << "Jugador eliminado correctamente." << endl;
                Sleep(2000);
            }
            else {
                // opcion == 'S', o cualquier otra cosa: regresar sin hacer nada
            }
        }

        char respuesta;
        system("cls");
        respuesta = leerOpcionValida("Desea volver al menu (S/N)? ", "SN");

        volverAlMenu = (respuesta == 'S');

    } while (!volverAlMenu);
}

// ================== DEFINICIONES: Seleccion de jugadores ==================

// Verifica que el texto contenga solo digitos (sin letras ni simbolos).
bool esNumeroValido(string texto) {

    if (texto.length() == 0) {
        return false;
    }

    for (int i = 0; i < texto.length(); i++) {
        if (!isdigit(texto[i])) {
            return false;
        }
    }

    return true;
}

// Imprime la lista numerada de jugadores disponibles para iniciar partida.
void mostrarListaJugadores(vector<string> &nombresDisponibles, vector<string> &idsDisponibles) {

    cout << "=== INICIAR JUEGO ===" << endl << endl;
    cout << "Jugadores disponibles:" << endl;

    for (int i = 0; i < nombresDisponibles.size(); i++) {
        cout << (i + 1) << ". " << nombresDisponibles[i]
             << " (ID: " << idsDisponibles[i] << ")" << endl;
    }
}

// Opcion 3: selecciona 2 jugadores y corre la partida.
void iniciarJuego() {

    vector<string> idsDisponibles;
    vector<string> nombresDisponibles;

    ifstream archivo("jugadores.txt");
    string linea;

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            int posicionComa = linea.find(',');
            idsDisponibles.push_back(linea.substr(0, posicionComa));
            nombresDisponibles.push_back(linea.substr(posicionComa + 1));
        }
        archivo.close();
    }

    int totalJugadores = idsDisponibles.size();

    if (totalJugadores == 0) {
        system("cls");
        cout << "Debe registrar jugadores." << endl;   // texto exacto del enunciado
        Sleep(2000);
        return;
    }

    // con exactamente 1 jugador registrado la
    // seleccion del segundo jugador quedaria en un ciclo infinito, ya que
    // se exige que sea distinto al primero pero no habria otro disponible.
    if (totalJugadores < 2) {
        system("cls");
        cout << "Se necesita al menos un jugador mas. Debe registrar otro jugador." << endl;
        Sleep(2000);
        return;
    }

    system("cls");
    mostrarListaJugadores(nombresDisponibles, idsDisponibles);

    // ---------- Seleccion del primer jugador ----------
    string entrada;
    int seleccion1 = 0;
    bool valido1 = false;

    do {
        cout << endl << "Seleccione el número del primer jugador: ";
        getline(cin, entrada);

        valido1 = false;
        if (esNumeroValido(entrada)) {
            seleccion1 = stoi(entrada);
            if (seleccion1 >= 1 && seleccion1 <= totalJugadores) {
                valido1 = true;
            }
        }

        if (!valido1) {
            cout << "Opción inválida, vuelva a intentarlo." << endl;
            Sleep(2000);
            system("cls");
            mostrarListaJugadores(nombresDisponibles, idsDisponibles);
        }

    } while (!valido1);

    // ---------- Seleccion del segundo jugador ----------
    int seleccion2 = 0;
    bool valido2 = false;

    system("cls");
    mostrarListaJugadores(nombresDisponibles, idsDisponibles);
    cout << endl << "Jugador 1 seleccionado: " << nombresDisponibles[seleccion1 - 1] << endl;

    do {
        cout << endl << "Seleccione el número del segundo jugador: ";
        getline(cin, entrada);

        valido2 = false;
        if (esNumeroValido(entrada)) {
            seleccion2 = stoi(entrada);
            if (seleccion2 >= 1 && seleccion2 <= totalJugadores && seleccion2 != seleccion1) {
                valido2 = true;
            }
        }

        if (!valido2) {
            cout << "Opción inválida, vuelva a intentarlo." << endl;
            Sleep(2000);
            system("cls");
            mostrarListaJugadores(nombresDisponibles, idsDisponibles);
            cout << endl << "Jugador 1 seleccionado: " << nombresDisponibles[seleccion1 - 1] << endl;
        }

    } while (!valido2);

    // ---------- Conectar la seleccion con la partida ----------
    string idJ1 = idsDisponibles[seleccion1 - 1];
    string nombreJ1 = nombresDisponibles[seleccion1 - 1];
    string idJ2 = idsDisponibles[seleccion2 - 1];
    string nombreJ2 = nombresDisponibles[seleccion2 - 1];

    char tablero[FILAS][COLUMNAS];
    int puntos[2];
    char jugarDeNuevo;


    do {
        puntos[0] = 0;
        puntos[1] = 0;

        jugarPartida(tablero, puntos, idJ1, nombreJ1, idJ2, nombreJ2);

        system("cls");
        cout << "=== RESULTADO FINAL DE LA PARTIDA ===" << endl << endl;
        cout << nombreJ1 << " -> " << puntos[0] << " comidas" << endl;
        cout << nombreJ2 << " -> " << puntos[1] << " comidas" << endl << endl;

        if (puntos[0] > puntos[1]) {
            cout << "Gana " << nombreJ1 << " (2 puntos). " << nombreJ2 << " obtiene 0 puntos." << endl;
        }
        else if (puntos[1] > puntos[0]) {
            cout << "Gana " << nombreJ2 << " (2 puntos). " << nombreJ1 << " obtiene 0 puntos." << endl;
        }
        else {
            cout << "¡Empate! 1 punto para cada jugador." << endl;
        }

        int puntajeFinalJ1, puntajeFinalJ2;
        calcularPuntajesFinales(puntos, puntajeFinalJ1, puntajeFinalJ2);

        guardarPuntaje(idJ1, puntajeFinalJ1);
        guardarPuntaje(idJ2, puntajeFinalJ2);

        cout << endl;
        jugarDeNuevo = leerOpcionValida("¿Desean jugar de nuevo? (S/N): ", "SN");

    } while (jugarDeNuevo == 'S');
}

// ================== DEFINICIONES: Puntajes ==================

// Devuelve la fecha del sistema en formato dd/mm/yyyy.
string obtenerFechaActual() {

    time_t ahora = time(0);
    tm *fechaLocal = localtime(&ahora);

    int dia = fechaLocal->tm_mday;
    int mes = fechaLocal->tm_mon + 1;
    int anio = fechaLocal->tm_year + 1900;

    string fechaTexto = "";

    if (dia < 10) fechaTexto += "0";
    fechaTexto += to_string(dia) + "/";

    if (mes < 10) fechaTexto += "0";
    fechaTexto += to_string(mes) + "/";

    fechaTexto += to_string(anio);

    return fechaTexto;
}

// Agrega una linea (id, puntaje, fecha) al final de puntajes.txt.
void guardarPuntaje(string id, int puntaje) {

    try {
        ofstream archivo("puntajes.txt", ios::app);

        if (!archivo.is_open()) {
            throw runtime_error("No se pudo abrir puntajes.txt");
        }

        archivo << id << "," << puntaje << "," << obtenerFechaActual() << endl;
        archivo.close();
    }
    catch (runtime_error &e) {
        cout << "Error al guardar: " << e.what() << endl;
    }
}

// Convierte las comidas de cada jugador en puntos finales (2/1/0).
void calcularPuntajesFinales(int puntos[2], int &puntajeFinalJ1, int &puntajeFinalJ2) {

    if (puntos[0] > puntos[1]) {
        puntajeFinalJ1 = 2;
        puntajeFinalJ2 = 0;
    }
    else if (puntos[1] > puntos[0]) {
        puntajeFinalJ1 = 0;
        puntajeFinalJ2 = 2;
    }
    else {
        puntajeFinalJ1 = 1;
        puntajeFinalJ2 = 1;
    }
}

// ================== DEFINICIONES: Tabla de puntuaciones ==================

// Busca idBuscado en idsUnicos desde el indice 'desde' en adelante.
// Retorna el indice si lo encuentra, -1 si no existe.
int buscarEnArreglo(vector<string> &idsUnicos, string idBuscado, int desde) {

    // Caso base: se revisaron todos los elementos sin encontrarlo
    if (desde >= (int)idsUnicos.size()) {
        return -1;
    }

    // Si el elemento actual coincide, se retorna su posicion
    if (idsUnicos[desde] == idBuscado) {
        return desde;
    }

    // Caso recursivo: se sigue buscando en el resto del vector
    return buscarEnArreglo(idsUnicos, idBuscado, desde + 1);
}

// Lee puntajes.txt y suma el historial completo por jugador unico.
void calcularPuntajesAcumulados(vector<string> &idsUnicos, vector<string> &fechasUltimas, vector<int> &puntajesAcumulados) {

    idsUnicos.clear();
    fechasUltimas.clear();
    puntajesAcumulados.clear();

    ifstream archivo("puntajes.txt");
    string linea;

    if (!archivo.is_open()) {
        return;   // si aun no existe el archivo, no hay puntajes que mostrar
    }

    while (getline(archivo, linea)) {

        int primeraComa = linea.find(',');
        int segundaComa = linea.find(',', primeraComa + 1);

        string idLinea = linea.substr(0, primeraComa);
        string puntajeTexto = linea.substr(primeraComa + 1, segundaComa - primeraComa - 1);
        string fechaLinea = linea.substr(segundaComa + 1);

        int puntajeLinea = stoi(puntajeTexto);

        int posicion = buscarEnArreglo(idsUnicos, idLinea, 0);

        if (posicion == -1) {
            // jugador todavia no esta en la tabla acumulada: se agrega
            idsUnicos.push_back(idLinea);
            puntajesAcumulados.push_back(puntajeLinea);
            fechasUltimas.push_back(fechaLinea);
        }
        else {
            // jugador ya estaba: se suma el puntaje y se actualiza la fecha
            puntajesAcumulados[posicion] += puntajeLinea;
            fechasUltimas[posicion] = fechaLinea;
        }
    }

    archivo.close();
}

// Opcion 4: muestra la tabla acumulada de puntajes.
void mostrarTablaPuntuaciones() {

    vector<string> idsUnicos;
    vector<string> fechasUltimas;
    vector<int> puntajesAcumulados;
    char respuesta;

    do {
        calcularPuntajesAcumulados(idsUnicos, fechasUltimas, puntajesAcumulados);

        system("cls");
        cout << "===================================================" << endl;
        cout << "               TABLA DE PUNTUACIONES" << endl;
        cout << "===================================================" << endl;

        if (idsUnicos.size() == 0) {
            cout << endl << "Aún no hay puntajes registrados." << endl << endl;
        }
        else {
            cout << left << setw(12) << "ID" << setw(25) << "Nombre"
                 << setw(10) << "Puntaje" << "Ultima Fecha" << endl;
            cout << "---------------------------------------------------" << endl;

            for (int i = 0; i < idsUnicos.size(); i++) {

                string nombre;
                bool encontrado = buscarJugador(idsUnicos[i], nombre);

                if (!encontrado) {
                    // el jugador pudo haber sido eliminado despues de jugar
                    nombre = "(jugador eliminado)";
                }

                cout << left << setw(12) << idsUnicos[i] << setw(25) << nombre
                     << setw(10) << puntajesAcumulados[i] << fechasUltimas[i] << endl;
            }
            cout << endl;
        }

        respuesta = leerOpcionValida("Desea regresar al menu (S/N)? ", "SN");

    } while (respuesta == 'N');
}

// ================== DEFINICIONES: Logica del juego ==================

// Llena el tablero de 10x10 con celdas vacias ('.').
void inicializarTablero(char tablero[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            tablero[i][j] = '.';
        }
    }
}

// Genera una posicion aleatoria dentro del tablero.
void generarPosicion(int &fila, int &columna) {
    fila = rand() % FILAS;
    columna = rand() % COLUMNAS;
}

// Dibuja el tablero en pantalla con un borde de '#'.
void imprimirTablero(char tablero[FILAS][COLUMNAS]) {

    cout << "#";
    for (int j = 0; j < COLUMNAS; j++) cout << "##";
    cout << "#" << endl;

    for (int i = 0; i < FILAS; i++) {
        cout << "#";
        for (int j = 0; j < COLUMNAS; j++) {
            cout << tablero[i][j] << " ";
        }
        cout << "#" << endl;
    }

    cout << "#";
    for (int j = 0; j < COLUMNAS; j++) cout << "##";
    cout << "#" << endl;
}

// Compara dos posiciones (fila, columna) y dice si son la misma celda.
bool posicionesIguales(int filaSerpiente, int columnaSerpiente, int filaComida, int columnaComida) {
    return (filaSerpiente == filaComida) && (columnaSerpiente == columnaComida);
}

// Verifica que la tecla presionada sea una de las 4 flechas.
bool esTeclaValida(int tecla) {
    return (tecla == 72 || tecla == 80 || tecla == 75 || tecla == 77);
}

// Calcula la nueva posicion segun la flecha; lanza excepcion si sale del tablero.
void moverSerpiente(int tecla, int &fila, int &columna) {

    int nuevaFila = fila;
    int nuevaColumna = columna;

    if (tecla == 72) {
        nuevaFila = nuevaFila - 1;
    }
    else if (tecla == 80) {
        nuevaFila = nuevaFila + 1;
    }
    else if (tecla == 75) {
        nuevaColumna = nuevaColumna - 1;
    }
    else if (tecla == 77) {
        nuevaColumna = nuevaColumna + 1;
    }

    if (nuevaFila < 0 || nuevaFila > 9 || nuevaColumna < 0 || nuevaColumna > 9) {
        throw string("Choque contra el borde");
    }

    fila = nuevaFila;
    columna = nuevaColumna;
}

// Controla un turno completo de un jugador: mueve la serpiente hasta que
// come, choca contra el borde, o se acaban los movimientos permitidos.
bool jugarTurno(char tablero[FILAS][COLUMNAS], vector<Posicion> &serpiente, int filaComida, int columnaComida, string idJugador, string nombreJugador) {

    int contadorMovimientos = 0;
    bool ganoPunto = false;
    bool turnoTerminado = false;
    int tecla;

    do {
        system("cls");


        cout << "Turno de: " << nombreJugador << endl;
        cout << "ID: " << idJugador << endl << endl;
        imprimirTablero(tablero);
        cout << "Movimientos restantes: " << (MAX_MOVIMIENTOS - contadorMovimientos) << endl;
        cout << "Presione una flecha para moverse: ";

    int prefijo = _getch();
    bool esExtendida = (prefijo == 0 || prefijo == 224);

        if (esExtendida) {
        tecla = _getch();
    }
        else {
        tecla = prefijo;
    }

        if (!esExtendida || !esTeclaValida(tecla)) {
        cout << endl << "Tecla incorrecta, inténtelo de nuevo." << endl;
        Sleep(2000);
        continue;
    }

        tablero[serpiente[0].fila][serpiente[0].columna] = '.';

        // moverSerpiente() lanza una excepcion (throw string) si la
        // nueva posicion queda fuera del tablero (0-9).
        try {
            moverSerpiente(tecla, serpiente[0].fila, serpiente[0].columna);

            if (posicionesIguales(serpiente[0].fila, serpiente[0].columna, filaComida, columnaComida)) {
                tablero[serpiente[0].fila][serpiente[0].columna] = '0';
                cout << "¡Comiste la comida! Ganas un punto." << endl;
                Sleep(2000);
                ganoPunto = true;
                turnoTerminado = true;
            }
            else {
                tablero[serpiente[0].fila][serpiente[0].columna] = '0';
            }
        }

        catch (const string &mensaje) {
            cout << mensaje << endl;
            cout << "Pierdes el turno." << endl;
            Sleep(2000);
            turnoTerminado = true;
        }

        contadorMovimientos++;

        if (contadorMovimientos >= MAX_MOVIMIENTOS) {
            turnoTerminado = true;
        }

    } while (!turnoTerminado);

    return ganoPunto;
}


void jugarPartida(char tablero[FILAS][COLUMNAS], int puntos[2], string idJ1, string nombreJ1, string idJ2, string nombreJ2) {

    vector<Posicion> serpiente1(1);   // vector de 1 elemento
    vector<Posicion> serpiente2(1);
    int filaComida, columnaComida;

    for (int ronda = 1; ronda <= NUM_RONDAS; ronda++) {

        // ---------- Turno del Jugador 1 ----------
        inicializarTablero(tablero);
        generarPosicion(serpiente1[0].fila, serpiente1[0].columna);
        do {
            generarPosicion(filaComida, columnaComida);
        } while (posicionesIguales(serpiente1[0].fila, serpiente1[0].columna, filaComida, columnaComida));

        tablero[serpiente1[0].fila][serpiente1[0].columna] = '0';
        tablero[filaComida][columnaComida] = '*';

        system("cls");
        cout << "Ronda " << ronda << " de " << NUM_RONDAS << " - Turno de " << nombreJ1 << endl;
        cout << "Puntaje acumulado -> " << nombreJ1 << ": " << puntos[0]
             << "  " << nombreJ2 << ": " << puntos[1] << endl << endl;
        Sleep(2000);

        if (jugarTurno(tablero, serpiente1, filaComida, columnaComida, idJ1, nombreJ1)) {
            puntos[0]++;
        }

        // ---------- Turno del Jugador 2 ----------
        inicializarTablero(tablero);
        generarPosicion(serpiente2[0].fila, serpiente2[0].columna);
        do {
            generarPosicion(filaComida, columnaComida);
        } while (posicionesIguales(serpiente2[0].fila, serpiente2[0].columna, filaComida, columnaComida));

        tablero[serpiente2[0].fila][serpiente2[0].columna] = '0';
        tablero[filaComida][columnaComida] = '*';

        system("cls");
        cout << "Ronda " << ronda << " de " << NUM_RONDAS << " - Turno de " << nombreJ2 << endl;
        cout << "Puntaje acumulado -> " << nombreJ1 << ": " << puntos[0]
             << "  " << nombreJ2 << ": " << puntos[1] << endl << endl;
        Sleep(2000);

        if (jugarTurno(tablero, serpiente2, filaComida, columnaComida, idJ2, nombreJ2)) {
            puntos[1]++;
        }
    }
}
