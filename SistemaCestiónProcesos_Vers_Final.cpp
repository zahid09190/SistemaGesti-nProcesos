#include <iostream>     
#include <string>     
using namespace std;   

// Estructura para representar un proceso
struct Proceso {
    int id;            
    string nombre;      
    int prioridad;      
    int memoria;        
    Proceso* siguiente; 
};

// Clase para la Lista Enlazada (Gestor de Procesos)
class ListaProcesos {
private:
    Proceso* cabeza;    
    int contadorId;     
public:
    ListaProcesos() : cabeza(NULL), contadorId(1) {}  

    // Insertar nuevo proceso
    void insertarProceso(string nombre, int prioridad, int memoria) {
        Proceso* nuevo = new Proceso;            
        nuevo->id = contadorId++;               
        nuevo->nombre = nombre;                  
        nuevo->prioridad = prioridad;            
        nuevo->memoria = memoria;               
        nuevo->siguiente = cabeza;                
        cabeza = nuevo;                          
        cout << "Proceso " << nombre << " agregado con ID: " << nuevo->id << endl; // Mensaje confirmando la inserción
    }
    
    // Eliminar proceso por ID
    bool eliminarProceso(int id) {
        Proceso* actual = cabeza;   
        Proceso* anterior = NULL;    
        
        while (actual != NULL) {     
            if (actual->id == id) { 
                if (anterior == NULL) {           
                    cabeza = actual->siguiente;  
                } else {
                    anterior->siguiente = actual->siguiente; 
                }
                delete actual;  
                cout << "Proceso con ID " << id << " eliminado." << endl; 
                return true;   
            }
            anterior = actual;         
            actual = actual->siguiente; 
        }
        cout << "Proceso con ID " << id << " no encontrado." << endl; 
        return false; 
    }
    
    // Buscar proceso por nombre
    void buscarPorNombre(string nombre) {
        Proceso* actual = cabeza; 
        bool encontrado = false;   
        
        cout << "\nResultados de busqueda para '" << nombre << "':" << endl;
        while (actual != NULL) {   
            if (actual->nombre.find(nombre) != string::npos) {  
                cout << "ID: " << actual->id << ", Nombre: " << actual->nombre ;
                cout << ", Prioridad: " << actual->prioridad ;
                cout << ", Memoria: " << actual->memoria << "MB" << endl;  
                encontrado = true;   
            }
            actual = actual->siguiente;
		}
        
        if (!encontrado) {  
            cout << "No se encontraron procesos con ese nombre." << endl; 
        }
    }
    
    // Modificar prioridad de un proceso
    bool modificarPrioridad(int id, int nuevaPrioridad) {
        Proceso* actual = cabeza; 
        
        while (actual != NULL) {  
            if (actual->id == id) {  
                actual->prioridad = nuevaPrioridad; 
                cout << "Prioridad del proceso ID " << id << " actualizada a " << nuevaPrioridad << endl; 
                return true;  
            }
            actual = actual->siguiente;  
        }
        cout << "Proceso con ID " << id << " no encontrado." << endl; 
        return false;  
    }
    
    // Mostrar todos los procesos
    void mostrarProcesos() {
        Proceso* actual = cabeza;  
        
        cout << "\n=== LISTA DE PROCESOS ===" << endl;  
        if (actual == NULL) {    
            cout << "No hay procesos registrados." << endl; 
            return;  
        }
        
        while (actual != NULL) {  
            cout << "ID: " << actual->id << ", Nombre: " << actual->nombre;
            cout << ", Prioridad: " << actual->prioridad;
            cout << ", Memoria: " << actual->memoria << "MB" << endl; 
            actual = actual->siguiente; 
        }
    }
    
    Proceso* getCabeza() const {
        return cabeza;  
    }
};

// Nodo para la cola de prioridad
struct NodoCola {
    Proceso* proceso;    
    NodoCola* siguiente; 
};

// Clase para la Cola de Prioridad (Planificador de CPU)
class ColaCPU {
private:
    NodoCola* frente;   
    NodoCola* final;    
public:
    ColaCPU() : frente(NULL), final(NULL) {}  
    
    // Encolar proceso según prioridad (mayor prioridad primero)
    void encolar(Proceso* proceso) {
        NodoCola* nuevo = new NodoCola; 
        nuevo->proceso = proceso;         
        nuevo->siguiente = NULL;         
        
        if (frente == NULL) {             
            frente = final = nuevo;       
        } else if (proceso->prioridad > frente->proceso->prioridad) { 
            nuevo->siguiente = frente;    
            frente = nuevo;              
        } else {
            NodoCola* actual = frente;   
            while (actual->siguiente != NULL && 
                   actual->siguiente->proceso->prioridad >= proceso->prioridad) {
                actual = actual->siguiente; 
            }
            nuevo->siguiente = actual->siguiente;  
            actual->siguiente = nuevo;
            if (nuevo->siguiente == NULL) {
                final = nuevo;               
            }
        }
    }
    
    // Desencolar proceso (ejecutar)
    Proceso* desencolar() {
        if (frente == NULL) {   
            return NULL;       
        }
        
        NodoCola* temp = frente;      
        Proceso* proceso = frente->proceso;  
        frente = frente->siguiente;   
        
        if (frente == NULL) {         
            final = NULL;             
        }
        
        delete temp;                  
        return proceso;               
    }
    
    // Mostrar cola de CPU
    void mostrarCola() {
        NodoCola* actual = frente; 
        
        cout << "\n=== COLA DE CPU (Ordenados por prioridad) ===" << endl;  
        if (actual == NULL) {    
            cout << "No hay procesos en la cola de CPU." << endl;  
            return;                
        }
        
        while (actual != NULL) {  
            cout << "ID: " << actual->proceso->id << ", Nombre: " << actual->proceso->nombre;
            cout << ", Prioridad: " << actual->proceso->prioridad; 
            cout << ", Memoria: " << actual->proceso->memoria << "MB" << endl;  
            actual = actual->siguiente;  
        }
    }
};
// Clase para la Pila (Gestor de Memoria)
class PilaMemoria {
private:
    Proceso* tope; 
public:
    PilaMemoria() : tope(NULL) {} 
    
    // Push (asignar memoria)
    void asignarMemoria(Proceso* proceso) {
        proceso->siguiente = tope; 
        tope = proceso; 
        cout << "Memoria asignada al proceso " << proceso->nombre << endl; 
    }
    
    // Pop (liberar memoria)
    Proceso* liberarMemoria() {
        if (tope == NULL) { 
            return NULL; 
        }
        
        Proceso* proceso = tope; 
        tope = tope->siguiente; 
        proceso->siguiente = NULL;
        cout << "Memoria liberada del proceso " << proceso->nombre << endl; 
        return proceso; 
    }
    
    // Mostrar estado de la memoria
    void mostrarMemoria() {
	    Proceso* actual = tope; 
	    int memoriaUsada = 0; 
	
	    cout << "\n=== ESTADO DE LA MEMORIA ===" << endl; 
	    if (actual == NULL) { 
	        cout << "No hay procesos usando memoria." << endl; 
	    }
	
	    while (actual != NULL) {
	        cout << "ID: " << actual->id << ", Nombre: " << actual->nombre 
	             << ", Memoria usada: " << actual->memoria << "MB" << endl; 
	        memoriaUsada += actual->memoria; 
	        actual = actual->siguiente; 
	    }
	
	    const int MEMORIA_TOTAL = 4096; 
	    cout << "\nResumen de memoria:" << endl;
	    cout << "Memoria total: " << MEMORIA_TOTAL << "MB" << endl; 
	    cout << "Memoria usada: " << memoriaUsada << "MB" << endl; 
	    cout << "Memoria disponible: " << MEMORIA_TOTAL - memoriaUsada << "MB" << endl; 
	    
	    float porcentaje = (static_cast<float>(memoriaUsada) / MEMORIA_TOTAL) * 100; 
	    cout << "Porcentaje usado: " << porcentaje << "%" << endl; 
    }
};

// Función para mostrar el menú principal
void mostrarMenu() {
    cout << "\n=== SISTEMA DE GESTION DE PROCESOS ===" << endl; 
    cout << "1. Agregar nuevo proceso" << endl; 
    cout << "2. Eliminar proceso" << endl; 
    cout << "3. Buscar proceso por nombre" << endl; 
    cout << "4. Modificar prioridad de proceso" << endl; 
    cout << "5. Mostrar todos los procesos" << endl; 
    cout << "6. Encolar proceso para ejecucion" << endl; 
    cout << "7. Ejecutar siguiente proceso" << endl; 
    cout << "8. Mostrar cola de CPU" << endl; 
    cout << "9. Asignar memoria a proceso" << endl; 
    cout << "10. Liberar memoria" << endl; 
    cout << "11. Mostrar estado de memoria" << endl; 
    cout << "0. Salir" << endl; 
    cout << "Seleccione una opcion: "; 
}

int main() {
    ListaProcesos lista;
    ColaCPU colaCPU; 
    PilaMemoria pilaMemoria; 
    int opcion; 
    
    do {
        mostrarMenu(); 
        cin >> opcion; 
        cin.ignore(); 
        
        switch (opcion) { 
            case 1: {
                string nombre;
                int prioridad, memoria;
                
                cout << "Nombre del proceso: "; 
                getline(cin, nombre); 
                cout << "Prioridad (1-10): "; 
                cin >> prioridad;
                cout << "Memoria requerida (MB): "; 
                cin >> memoria;
                
                lista.insertarProceso(nombre, prioridad, memoria); 
                break;
            }
            case 2: {
                int id;
                cout << "ID del proceso a eliminar: "; 
                cin >> id;
                lista.eliminarProceso(id); 
                break;
            }
            case 3: {
                string nombre;
                cout << "Nombre a buscar: "; 
                getline(cin, nombre); 
                lista.buscarPorNombre(nombre); 
                break;
            }
            case 4: {
                int id, nuevaPrioridad;
                cout << "ID del proceso: ";
                cin >> id;
                cout << "Nueva prioridad (1-10): "; 
                cin >> nuevaPrioridad;
                lista.modificarPrioridad(id, nuevaPrioridad); 
                break;
            }
            case 5:
                lista.mostrarProcesos(); 
                break;
            case 6: {
                int id;
                cout << "ID del proceso a encolar: "; 
                cin >> id;
                
                Proceso* actual = lista.getCabeza(); 
                Proceso* procesoEncolar = NULL; 
                
                while (actual != NULL) { 
                    if (actual->id == id) { 
                        procesoEncolar = actual;
                        break; //
                    }
                    actual = actual->siguiente; 
                }
                
                if (procesoEncolar != NULL) { 
                    colaCPU.encolar(procesoEncolar); 
                    cout << "Proceso " << procesoEncolar->nombre << " encolado para ejecucion." << endl;
                } else {
                    cout << "Proceso con ID " << id << " no encontrado." << endl; 
                }
                break;
            }
            case 7: {
                Proceso* procesoEjecutado = colaCPU.desencolar(); 
                if (procesoEjecutado != NULL) { 
                    cout << "Ejecutando proceso: " << procesoEjecutado->nombre << endl;
                } else {
                    cout << "No hay procesos en la cola de CPU." << endl; 
                }
                break;
            }
            case 8:
                colaCPU.mostrarCola(); 
                break;
            case 9: {
                int id;
                cout << "ID del proceso para asignar memoria: "; 
                cin >> id;
                
                Proceso* actual = lista.getCabeza(); 
                Proceso* procesoMemoria = NULL;
                
                while (actual != NULL) { 
                    if (actual->id == id) {
                        procesoMemoria = actual;
                        break;
                    }
                    actual = actual->siguiente;
                }
                
                if (procesoMemoria != NULL) {
                    pilaMemoria.asignarMemoria(procesoMemoria); 
                } else {
                    cout << "Proceso con ID " << id << " no encontrado." << endl;
                }
                break;
            }
            case 10: {
                Proceso* procesoLiberado = pilaMemoria.liberarMemoria(); 
                if (procesoLiberado == NULL) { 
                    cout << "No hay procesos usando memoria." << endl;
                }
                break;
            }
            case 11:
                pilaMemoria.mostrarMemoria(); 
                break;
            case 0:
                cout << "Saliendo del sistema..." << endl; 
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl; 
        }
    } while (opcion != 0); 
    
    return 0;
}
