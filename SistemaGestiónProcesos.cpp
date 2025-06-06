#include <iostream>    // Incluye la librería para entrada y salida estándar
#include <string>      // Incluye la librería para manejo de cadenas de texto (strings)
using namespace std;   // Usa el espacio de nombres std para evitar escribir std:: antes de cout, string, etc.

// Estructura para representar un proceso
struct Proceso {
    int id;             // Identificador único del proceso
    string nombre;      // Nombre del proceso
    int prioridad;      // Prioridad del proceso
    int memoria;        // Memoria asignada al proceso (en MB)
    Proceso* siguiente; // Puntero al siguiente proceso en la lista enlazada
};

// Clase para la Lista Enlazada (Gestor de Procesos)
class ListaProcesos {
private:
    Proceso* cabeza;    // Puntero al primer proceso de la lista
    int contadorId;     // Contador para asignar IDs únicos a los procesos
public:
    ListaProcesos() : cabeza(NULL), contadorId(1) {}  // Constructor que inicializa cabeza en NULL y contadorId en 1

    // Insertar nuevo proceso
    void insertarProceso(string nombre, int prioridad, int memoria) {
        Proceso* nuevo = new Proceso;            // Crea un nuevo proceso dinámicamente
        nuevo->id = contadorId++;                // Asigna un ID único y luego incrementa el contador
        nuevo->nombre = nombre;                  // Asigna el nombre al proceso
        nuevo->prioridad = prioridad;            // Asigna la prioridad al proceso
        nuevo->memoria = memoria;                // Asigna la memoria al proceso
        nuevo->siguiente = cabeza;               // El nuevo proceso apunta al proceso que era cabeza (inserción al inicio)
        cabeza = nuevo;                          // Actualiza cabeza para que apunte al nuevo proceso
        cout << "Proceso " << nombre << " agregado con ID: " << nuevo->id << endl; // Mensaje confirmando la inserción
    }
    
    // Eliminar proceso por ID
    bool eliminarProceso(int id) {
        Proceso* actual = cabeza;    // Apunta al primer proceso de la lista
        Proceso* anterior = NULL;    // Apunta al proceso anterior al actual (inicialmente NULL)
        
        while (actual != NULL) {     // Recorre la lista hasta que actual sea NULL
            if (actual->id == id) {  // Si encuentra el proceso con el ID buscado
                if (anterior == NULL) {           // Si es el primer proceso (cabeza)
                    cabeza = actual->siguiente;  // Actualiza cabeza al siguiente proceso
                } else {
                    anterior->siguiente = actual->siguiente; // Salta el proceso actual en la lista
                }
                delete actual;  // Libera la memoria del proceso eliminado
                cout << "Proceso con ID " << id << " eliminado." << endl; // Mensaje confirmando eliminación
                return true;   // Retorna true indicando éxito
            }
            anterior = actual;         // Actualiza anterior para la próxima iteración
            actual = actual->siguiente; // Avanza al siguiente proceso
        }
        cout << "Proceso con ID " << id << " no encontrado." << endl; // Mensaje si no encuentra el proceso
        return false;  // Retorna false indicando que no se encontró el proceso
    }
    
    // Buscar proceso por nombre
    void buscarPorNombre(string nombre) {
        Proceso* actual = cabeza;  // Apunta al primer proceso de la lista
        bool encontrado = false;   // Bandera para saber si se encontró algún proceso
        
        cout << "\nResultados de busqueda para '" << nombre << "':" << endl; // Encabezado de resultados
        while (actual != NULL) {   // Recorre la lista hasta el final
            if (actual->nombre.find(nombre) != string::npos) {  // Busca si el nombre contiene la cadena dada
                cout << "ID: " << actual->id << ", Nombre: " << actual->nombre ;
                cout << ", Prioridad: " << actual->prioridad ;
                cout << ", Memoria: " << actual->memoria << "MB" << endl;  // Muestra info del proceso encontrado
                encontrado = true;   // Marca que encontró al menos un proceso
            }
            actual = actual->siguiente;  // Avanza al siguiente proceso
        }
        
        if (!encontrado) {  // Si no encontró ningún proceso
            cout << "No se encontraron procesos con ese nombre." << endl;  // Mensaje de no resultados
        }
    }
    
    // Modificar prioridad de un proceso
    bool modificarPrioridad(int id, int nuevaPrioridad) {
        Proceso* actual = cabeza;  // Apunta al primer proceso de la lista
        
        while (actual != NULL) {   // Recorre la lista
            if (actual->id == id) {  // Busca el proceso con el ID dado
                actual->prioridad = nuevaPrioridad;  // Cambia la prioridad
                cout << "Prioridad del proceso ID " << id << " actualizada a " << nuevaPrioridad << endl; // Mensaje de confirmación
                return true;  // Retorna true indicando éxito
            }
            actual = actual->siguiente;  // Avanza al siguiente proceso
        }
        cout << "Proceso con ID " << id << " no encontrado." << endl; // Mensaje si no encuentra el proceso
        return false;  // Retorna false indicando que no encontró el proceso
    }
    
    // Mostrar todos los procesos
    void mostrarProcesos() {
        Proceso* actual = cabeza;  // Apunta al primer proceso de la lista
        
        cout << "\n=== LISTA DE PROCESOS ===" << endl;  // Encabezado para mostrar la lista
        if (actual == NULL) {    // Si la lista está vacía
            cout << "No hay procesos registrados." << endl;  // Mensaje de lista vacía
            return;  // Sale del método
        }
        
        while (actual != NULL) {  // Recorre toda la lista
            cout << "ID: " << actual->id << ", Nombre: " << actual->nombre;
            cout << ", Prioridad: " << actual->prioridad;
            cout << ", Memoria: " << actual->memoria << "MB" << endl;  // Muestra los datos de cada proceso
            actual = actual->siguiente;  // Avanza al siguiente proceso
        }
    }
    
    // Método para obtener el puntero a la cabeza de la lista (solo lectura)
    Proceso* getCabeza() const {
        return cabeza;  // Retorna el puntero al primer proceso
    }
};

// Nodo para la cola de prioridad
struct NodoCola {
    Proceso* proceso;    // Puntero al proceso almacenado en el nodo
    NodoCola* siguiente; // Puntero al siguiente nodo en la cola
};

// Clase para la Cola de Prioridad (Planificador de CPU)
class ColaCPU {
private:
    NodoCola* frente;   // Puntero al frente de la cola
    NodoCola* final;    // Puntero al final de la cola
public:
    ColaCPU() : frente(NULL), final(NULL) {}  // Constructor que inicializa punteros en NULL
    
    // Encolar proceso según prioridad (mayor prioridad primero)
    void encolar(Proceso* proceso) {
        NodoCola* nuevo = new NodoCola;  // Crea un nuevo nodo para la cola
        nuevo->proceso = proceso;         // Asigna el proceso al nodo
        nuevo->siguiente = NULL;          // El siguiente nodo es NULL (al final)
        
        if (frente == NULL) {             // Si la cola está vacía
            frente = final = nuevo;       // El nuevo nodo es frente y final
        } else if (proceso->prioridad > frente->proceso->prioridad) {  // Si el nuevo proceso tiene mayor prioridad que el frente
            nuevo->siguiente = frente;    // Inserta el nuevo nodo antes del frente
            frente = nuevo;               // Actualiza frente al nuevo nodo
        } else {
            NodoCola* actual = frente;    // Nodo auxiliar para recorrer la cola
            // Busca la posición donde insertar el nuevo nodo manteniendo orden por prioridad descendente
            while (actual->siguiente != NULL && 
                   actual->siguiente->proceso->prioridad >= proceso->prioridad) {
                actual = actual->siguiente;  // Avanza al siguiente nodo
            }
            nuevo->siguiente = actual->siguiente;  // Inserta el nuevo nodo en la posición encontrada
            actual->siguiente = nuevo;
            if (nuevo->siguiente == NULL) {  // Si el nuevo nodo quedó al final
                final = nuevo;                // Actualiza el final
            }
        }
    }
    
    // Desencolar proceso (ejecutar)
    Proceso* desencolar() {
        if (frente == NULL) {   // Si la cola está vacía
            return NULL;        // Retorna NULL
        }
        
        NodoCola* temp = frente;      // Guarda temporalmente el nodo del frente
        Proceso* proceso = frente->proceso;  // Obtiene el proceso del frente
        frente = frente->siguiente;   // Avanza el frente al siguiente nodo
        
        if (frente == NULL) {         // Si la cola queda vacía
            final = NULL;             // Actualiza final a NULL
        }
        
        delete temp;                  // Libera la memoria del nodo eliminado
        return proceso;               // Retorna el proceso desencolado
    }
    
    // Mostrar cola de CPU
    void mostrarCola() {
        NodoCola* actual = frente;  // Apunta al primer nodo de la cola
        
        cout << "\n=== COLA DE CPU (Ordenados por prioridad) ===" << endl;  // Encabezado de la cola
        if (actual == NULL) {      // Si la cola está vacía
            cout << "No hay procesos en la cola de CPU." << endl;  // Mensaje de cola vacía
            return;                // Sale del método
        }
        
        while (actual != NULL) {   // Recorre la cola mostrando los procesos
            cout << "ID: " << actual->proceso->id << ", Nombre: " << actual->proceso->nombre;
            cout << ", Prioridad: " << actual->proceso->prioridad; 
            cout << ", Memoria: " << actual->proceso->memoria << "MB" << endl;  // Muestra info del proceso
            actual = actual->siguiente;  // Avanza al siguiente nodo
        }
    }
};
// Clase para la Pila (Gestor de Memoria)
class PilaMemoria {
private:
    Proceso* tope; // Puntero al tope de la pila (último proceso asignado en memoria)
public:
    PilaMemoria() : tope(NULL) {} // Constructor que inicializa el tope como NULL (memoria vacía)
    
    // Push (asignar memoria)
    void asignarMemoria(Proceso* proceso) {
        proceso->siguiente = tope; // El nuevo proceso apunta al tope actual
        tope = proceso; // Se actualiza el tope al nuevo proceso
        cout << "Memoria asignada al proceso " << proceso->nombre << endl; // Mensaje de confirmación
    }
    
    // Pop (liberar memoria)
    Proceso* liberarMemoria() {
        if (tope == NULL) { // Si la pila está vacía
            return NULL; // Retornar NULL (no hay procesos para liberar)
        }
        
        Proceso* proceso = tope; // Guardar el proceso actual del tope
        tope = tope->siguiente; // Actualizar el tope al siguiente proceso
        proceso->siguiente = NULL; // Eliminar el vínculo al siguiente
        cout << "Memoria liberada del proceso " << proceso->nombre << endl; // Mensaje de liberación
        return proceso; // Retornar el proceso que fue liberado
    }
    
    // Mostrar estado de la memoria
    void mostrarMemoria() {
	    Proceso* actual = tope; // Puntero auxiliar para recorrer la pila
	    int memoriaUsada = 0; // Variable para sumar la memoria total usada
	
	    cout << "\n=== ESTADO DE LA MEMORIA ===" << endl; // Encabezado del estado de memoria
	    if (actual == NULL) { // Si no hay procesos
	        cout << "No hay procesos usando memoria." << endl; // Mensaje indicativo
	    }
	
	    while (actual != NULL) { // Mientras haya procesos en la pila
	        cout << "ID: " << actual->id << ", Nombre: " << actual->nombre 
	             << ", Memoria usada: " << actual->memoria << "MB" << endl; // Mostrar info del proceso
	        memoriaUsada += actual->memoria; // Sumar su memoria al total
	        actual = actual->siguiente; // Avanzar al siguiente proceso
	    }
	
	    const int MEMORIA_TOTAL = 4096; // 4GB de RAM simulada
	    cout << "\nResumen de memoria:" << endl;
	    cout << "Memoria total: " << MEMORIA_TOTAL << "MB" << endl; // Memoria disponible
	    cout << "Memoria usada: " << memoriaUsada << "MB" << endl; // Memoria actualmente usada
	    cout << "Memoria disponible: " << MEMORIA_TOTAL - memoriaUsada << "MB" << endl; // Memoria libre
	    
	    float porcentaje = (static_cast<float>(memoriaUsada) / MEMORIA_TOTAL) * 100; // Calcular porcentaje usado
	    cout << "Porcentaje usado: " << porcentaje << "%" << endl; // Mostrar porcentaje
    }
};

// Función para mostrar el menú principal
void mostrarMenu() {
    cout << "\n=== SISTEMA DE GESTION DE PROCESOS ===" << endl; // Título
    cout << "1. Agregar nuevo proceso" << endl; // Opción 1
    cout << "2. Eliminar proceso" << endl; // Opción 2
    cout << "3. Buscar proceso por nombre" << endl; // Opción 3
    cout << "4. Modificar prioridad de proceso" << endl; // Opción 4
    cout << "5. Mostrar todos los procesos" << endl; // Opción 5
    cout << "6. Encolar proceso para ejecucion" << endl; // Opción 6
    cout << "7. Ejecutar siguiente proceso" << endl; // Opción 7
    cout << "8. Mostrar cola de CPU" << endl; // Opción 8
    cout << "9. Asignar memoria a proceso" << endl; // Opción 9
    cout << "10. Liberar memoria" << endl; // Opción 10
    cout << "11. Mostrar estado de memoria" << endl; // Opción 11
    cout << "0. Salir" << endl; // Opción 0
    cout << "Seleccione una opcion: "; // Solicitar entrada del usuario
}

int main() {
    ListaProcesos lista; // Crear objeto para manejar lista de procesos
    ColaCPU colaCPU; // Crear objeto para manejar cola de CPU
    PilaMemoria pilaMemoria; // Crear objeto para manejar memoria como pila
    int opcion; // Variable para almacenar la opción del menú
    
    do {
        mostrarMenu(); // Mostrar el menú en cada iteración
        cin >> opcion; // Leer opción del usuario
        cin.ignore(); // Limpiar el buffer del teclado
        
        switch (opcion) { // Evaluar la opción seleccionada
            case 1: {
                string nombre;
                int prioridad, memoria;
                
                cout << "Nombre del proceso: "; // Solicitar nombre
                getline(cin, nombre); // Leer nombre con espacios
                cout << "Prioridad (1-10): "; // Solicitar prioridad
                cin >> prioridad;
                cout << "Memoria requerida (MB): "; // Solicitar memoria
                cin >> memoria;
                
                lista.insertarProceso(nombre, prioridad, memoria); // Insertar nuevo proceso en la lista
                break;
            }
            case 2: {
                int id;
                cout << "ID del proceso a eliminar: "; // Solicitar ID
                cin >> id;
                lista.eliminarProceso(id); // Eliminar proceso por ID
                break;
            }
            case 3: {
                string nombre;
                cout << "Nombre a buscar: "; // Solicitar nombre
                getline(cin, nombre); // Leer nombre
                lista.buscarPorNombre(nombre); // Buscar proceso por nombre
                break;
            }
            case 4: {
                int id, nuevaPrioridad;
                cout << "ID del proceso: "; // Solicitar ID
                cin >> id;
                cout << "Nueva prioridad (1-10): "; // Solicitar nueva prioridad
                cin >> nuevaPrioridad;
                lista.modificarPrioridad(id, nuevaPrioridad); // Modificar prioridad del proceso
                break;
            }
            case 5:
                lista.mostrarProcesos(); // Mostrar todos los procesos
                break;
            case 6: {
                int id;
                cout << "ID del proceso a encolar: "; // Solicitar ID
                cin >> id;
                
                // Buscar el proceso en la lista
                Proceso* actual = lista.getCabeza(); // Obtener el inicio de la lista
                Proceso* procesoEncolar = NULL; // Inicializar puntero
                
                while (actual != NULL) { // Recorrer la lista
                    if (actual->id == id) { // Si se encuentra el proceso
                        procesoEncolar = actual;
                        break; // Salir del ciclo
                    }
                    actual = actual->siguiente; // Avanzar en la lista
                }
                
                if (procesoEncolar != NULL) { // Si se encontró el proceso
                    colaCPU.encolar(procesoEncolar); // Encolarlo para ejecución
                    cout << "Proceso " << procesoEncolar->nombre << " encolado para ejecucion." << endl;
                } else {
                    cout << "Proceso con ID " << id << " no encontrado." << endl; // No encontrado
                }
                break;
            }
            case 7: {
                Proceso* procesoEjecutado = colaCPU.desencolar(); // Desencolar el siguiente proceso
                if (procesoEjecutado != NULL) { // Si hay proceso
                    cout << "Ejecutando proceso: " << procesoEjecutado->nombre << endl;
                } else {
                    cout << "No hay procesos en la cola de CPU." << endl; // Cola vacía
                }
                break;
            }
            case 8:
                colaCPU.mostrarCola(); // Mostrar cola de procesos
                break;
            case 9: {
                int id;
                cout << "ID del proceso para asignar memoria: "; // Solicitar ID
                cin >> id;
                
                // Buscar el proceso en la lista
                Proceso* actual = lista.getCabeza(); // Obtener el inicio de la lista
                Proceso* procesoMemoria = NULL;
                
                while (actual != NULL) { // Recorrer la lista
                    if (actual->id == id) {
                        procesoMemoria = actual;
                        break;
                    }
                    actual = actual->siguiente;
                }
                
                if (procesoMemoria != NULL) {
                    pilaMemoria.asignarMemoria(procesoMemoria); // Asignar memoria
                } else {
                    cout << "Proceso con ID " << id << " no encontrado." << endl; // No encontrado
                }
                break;
            }
            case 10: {
                Proceso* procesoLiberado = pilaMemoria.liberarMemoria(); // Liberar el último proceso de memoria
                if (procesoLiberado == NULL) { // Si no había procesos
                    cout << "No hay procesos usando memoria." << endl;
                }
                break;
            }
            case 11:
                pilaMemoria.mostrarMemoria(); // Mostrar estado de la memoria
                break;
            case 0:
                cout << "Saliendo del sistema..." << endl; // Finalizar programa
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl; // Opción incorrecta
        }
    } while (opcion != 0); // Repetir hasta que el usuario elija salir
    
    return 0; // Fin del programa
}
