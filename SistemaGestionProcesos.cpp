#include <iostream>
using namespace std;

// 1. Estructura para procesos
struct Proceso {
    int id;
    string nombre;
    int prioridad; // 0 = más alta, 4 = más baja
    string estado; // "nuevo", "listo", "ejecutando", "terminado"
    Proceso* siguiente;
};

// 2. Lista enlazada para gestión de procesos
class ListaProcesos {
private:
    Proceso* cabeza;
    int contadorId;
public:
    ListaProcesos() : cabeza(NULL), contadorId(1) {} // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
    
    void insertarProceso(string nombre, int prioridad) {
        Proceso* nuevo = new Proceso();
        nuevo->id = contadorId++;
        nuevo->nombre = nombre;
        nuevo->prioridad = prioridad;
        nuevo->estado = "nuevo";
        nuevo->siguiente = NULL; // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
        
        if (cabeza == NULL) { // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
            cabeza = nuevo;
        } else {
            Proceso* actual = cabeza;
            while (actual->siguiente != NULL) { // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        cout << "Proceso creado - ID: " << nuevo->id << endl;
    }
    
    void mostrarProcesos() {
        cout << "\n=== LISTA DE PROCESOS ===" << endl;
        Proceso* actual = cabeza;
        while (actual != NULL) { // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
            cout << "ID: " << actual->id  
                 << " | Nombre: " << actual->nombre  
                 << " | Prioridad: " << actual->prioridad
                 << " | Estado: " << actual->estado << endl;
            actual = actual->siguiente;
        }
        cout << "========================" << endl;
    }
    
    Proceso* buscarProceso(int id) {
        Proceso* actual = cabeza;
        while (actual != NULL) { // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
            if (actual->id == id) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return NULL; // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
    }
    
    void eliminarProceso(int id) {
        if (cabeza == NULL) return; // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
        
        if (cabeza->id == id) {
            Proceso* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            cout << "Proceso " << id << " eliminado." << endl;
            return;
        }
        
        Proceso* actual = cabeza;
        while (actual->siguiente != NULL && actual->siguiente->id != id) { // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
            actual = actual->siguiente;
        }
        
        if (actual->siguiente != NULL) { // cambie nullptr a NULL por que esta version de Dev-C++ no recocono nullptr
            Proceso* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            delete temp;
            cout << "Proceso " << id << " eliminado." << endl;
        } else {
            cout << "Proceso no encontrado." << endl;
        }
    }
};

// 3. Cola de prioridad para planificación
class ColaPrioridad {
private:
    Proceso* frente;
public:
    ColaPrioridad() : frente(NULL) {} // Changed nullptr to NULL
    
    void encolar(Proceso* proceso) {
        proceso->estado = "listo";
        proceso->siguiente = NULL; // Changed nullptr to NULL
        
        if (frente == NULL || proceso->prioridad < frente->prioridad) { // Changed nullptr to NULL
            proceso->siguiente = frente;
            frente = proceso;
        } else {
            Proceso* actual = frente;
            while (actual->siguiente != NULL &&  // Changed nullptr to NULL
                   actual->siguiente->prioridad <= proceso->prioridad) {
                actual = actual->siguiente;
            }
            proceso->siguiente = actual->siguiente;
            actual->siguiente = proceso;
        }
        cout << "Proceso " << proceso->id << " encolado para ejecucion." << endl;
    }
    
    void ejecutarProceso() {
        if (frente == NULL) { // Changed nullptr to NULL
            cout << "No hay procesos en cola." << endl;
            return;
        }
        
        frente->estado = "ejecutando";
        cout << "\n=== EJECUTANDO PROCESO ===" << endl;
        cout << "ID: " << frente->id << endl;
        cout << "Nombre: " << frente->nombre << endl;
        cout << "Prioridad: " << frente->prioridad << endl;
        cout << "=========================" << endl;
        
        Proceso* temp = frente;
        frente = frente->siguiente;
        delete temp;
    }
};

// 4. Menú principal
void mostrarMenu() {
    cout << "\n=== SISTEMA DE GESTION DE PROCESOS ===" << endl;
    cout << "1. Crear nuevo proceso" << endl;
    cout << "2. Mostrar todos los procesos" << endl;
    cout << "3. Eliminar proceso" << endl;
    cout << "4. Encolar proceso para ejecucion" << endl;
    cout << "5. Ejecutar siguiente proceso" << endl;
    cout << "6. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int main() {
    ListaProcesos lista;
    ColaPrioridad cola;
    int opcion, id, prioridad;
    string nombre;
    
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                cout << "Nombre del proceso: ";
                cin >> nombre;
                cout << "Prioridad (0-4): ";
                cin >> prioridad;
                lista.insertarProceso(nombre, prioridad);
                break;
            
            case 2:
                lista.mostrarProcesos();
                break;
            
            case 3:
                cout << "ID del proceso a eliminar: ";
                cin >> id;
                lista.eliminarProceso(id);
                break;
            
            case 4:
                cout << "ID del proceso a encolar: ";
                cin >> id;
                {
                    Proceso* p = lista.buscarProceso(id);
                    if (p != NULL) { // Changed nullptr to NULL               
                        cola.encolar(p);
                    } else {
                        cout << "Proceso no encontrado." << endl;
                    }
                }
                break;
            
            case 5:
                cola.ejecutarProceso();
                break;
            
            case 6:
                cout << "Saliendo del sistema..." << endl;
                break;
            
            default:
                cout << "Opcion no valida. Intente nuevamente." << endl;
        }
    } while (opcion != 6);
    
    return 0;
}
