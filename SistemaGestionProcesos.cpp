#include <iostream>
using namespace std;

// 1. Estructura para procesos
struct Proceso {
    int id;
    string nombre;
    int prioridad;
    string estado;
    Proceso* siguiente;
};

// 2. Lista enlazada para gestión de procesos
class ListaProcesos {
private:
    Proceso* cabeza;
    int contadorId;
public:
    ListaProcesos() : cabeza(NULL), contadorId(1) {}

    void insertarProceso(string nombre, int prioridad) {
        Proceso* nuevo = new Proceso();
        nuevo->id = contadorId++;
        nuevo->nombre = nombre;
        nuevo->prioridad = prioridad;
        nuevo->estado = "nuevo";
        nuevo->siguiente = NULL;

        if (cabeza == NULL) {
            cabeza = nuevo;
        } else {
            Proceso* actual = cabeza;
            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        cout << "Proceso creado - ID: " << nuevo->id << endl;
    }

    void mostrarProcesos() {
        cout << "\n=== LISTA DE PROCESOS ===" << endl;
        Proceso* actual = cabeza;
        while (actual != NULL) {
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
        while (actual != NULL) {
            if (actual->id == id) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return NULL;
    }

    void eliminarProceso(int id) {
        if (cabeza == NULL) return;

        if (cabeza->id == id) {
            Proceso* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            cout << "Proceso " << id << " eliminado." << endl;
            return;
        }

        Proceso* actual = cabeza;
        while (actual->siguiente != NULL && actual->siguiente->id != id) {
            actual = actual->siguiente;
        }

        if (actual->siguiente != NULL) {
            Proceso* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            delete temp;
            cout << "Proceso " << id << " eliminado." << endl;
        } else {
            cout << "Proceso no encontrado." << endl;
        }
    }
};

// === NUEVA CLASE: Pila para procesos ejecutados ===
class PilaProcesos {
private:
    Proceso* cima;
public:
    PilaProcesos() : cima(NULL) {}

    void apilar(Proceso* p) {
        p->estado = "terminado";
        p->siguiente = cima;
        cima = p;
    }

    void mostrarHistorial() {
        Proceso* actual = cima;
        if (actual == NULL) {
            cout << "\nHistorial vacío. No hay procesos ejecutados aún." << endl;
            return;
        }

        cout << "\n=== HISTORIAL DE PROCESOS TERMINADOS ===" << endl;
        while (actual != NULL) {
            cout << "ID: " << actual->id
                 << " | Nombre: " << actual->nombre
                 << " | Prioridad: " << actual->prioridad
                 << " | Estado: " << actual->estado << endl;
            actual = actual->siguiente;
        }
        cout << "=========================================" << endl;
    }
};

// === Instancia global de pila ===
PilaProcesos pila;

// 3. Cola de prioridad para planificación
class ColaPrioridad {
private:
    Proceso* frente;
public:
    ColaPrioridad() : frente(NULL) {}

    void encolar(Proceso* proceso) {
        if (proceso->estado == "bloqueado") {
            cout << "No se puede encolar el proceso " << proceso->id << " porque está bloqueado." << endl;
            return;
        }

        proceso->estado = "listo";
        proceso->siguiente = NULL;

        if (frente == NULL || proceso->prioridad < frente->prioridad) {
            proceso->siguiente = frente;
            frente = proceso;
        } else {
            Proceso* actual = frente;
            while (actual->siguiente != NULL &&
                   actual->siguiente->prioridad <= proceso->prioridad) {
                actual = actual->siguiente;
            }
            proceso->siguiente = actual->siguiente;
            actual->siguiente = proceso;
        }
        cout << "Proceso " << proceso->id << " encolado para ejecucion." << endl;
    }

    void ejecutarProceso() {
        if (frente == NULL) {
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

        // Apilar en historial ANTES de eliminar
        pila.apilar(temp);

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
    cout << "7. Ver historial de procesos ejecutados" << endl;
    cout << "8. Bloquear un proceso" << endl;
    cout << "9. Desbloquear un proceso" << endl;
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
                    if (p != NULL) {
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

            case 7:
                pila.mostrarHistorial();
                break;

            case 8:
                cout << "ID del proceso a bloquear: ";
                cin >> id;
                {
                    Proceso* p = lista.buscarProceso(id);
                    if (p != NULL) {
                        p->estado = "bloqueado";
                        cout << "Proceso " << id << " bloqueado." << endl;
                    } else {
                        cout << "Proceso no encontrado." << endl;
                    }
                }
                break;

            case 9:
                cout << "ID del proceso a desbloquear: ";
                cin >> id;
                {
                    Proceso* p = lista.buscarProceso(id);
                    if (p != NULL) {
                        if (p->estado == "bloqueado") {
                            p->estado = "nuevo";
                            cout << "Proceso " << id << " desbloqueado." << endl;
                        } else {
                            cout << "El proceso no está bloqueado." << endl;
                        }
                    } else {
                        cout << "Proceso no encontrado." << endl;
                    }
                }
                break;

            default:
                cout << "Opcion no valida. Intente nuevamente." << endl;
        }
    } while (opcion != 6);

    return 0;
}

