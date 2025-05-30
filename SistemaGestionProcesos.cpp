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

    // NUEVO: contar procesos ejecutados
    int contarProcesos() {
        int contador = 0;
        Proceso* actual = cima;
        while (actual != NULL) {
            contador++;
            actual = actual->siguiente;
        }
        return contador;
    }
};

// Instancia global
PilaProcesos pila;

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
    
    void modificarPrioridadProceso(int id, int nuevaPrioridad) {
        if (nuevaPrioridad < 0 || nuevaPrioridad > 4) {
            cout << "Prioridad invalida. Debe estar entre 0 y 4." << endl;
            return;
        }

        Proceso* actual = cabeza;
        while (actual != NULL) {
            if (actual->id == id) {
                actual->prioridad = nuevaPrioridad;
                cout << "Prioridad del proceso " << id << " modificada a "
                     << nuevaPrioridad << "." << endl;
                return;
            }
            actual = actual->siguiente;
        }
        cout << "Proceso " << id << " no encontrado." << endl;
    }

    // NUEVO: Mostrar estadísticas
    void mostrarEstadisticas() {
        int total = 0, bloqueados = 0, activos = 0;

        Proceso* actual = cabeza;
        while (actual != NULL) {
            total++;
            if (actual->estado == "bloqueado") bloqueados++;
            if (actual->estado != "terminado") activos++;
            actual = actual->siguiente;
        }

        cout << "\n=== ESTADÍSTICAS GENERALES ===" << endl;
        cout << "Total de procesos creados: " << total << endl;
        cout << "Total de procesos ejecutados: " << contarEjecutados() << endl;
        cout << "Total de procesos activos: " << activos << endl;
        cout << "Total de procesos bloqueados: " << bloqueados << endl;
        cout << "===============================" << endl;
    }

    int contarEjecutados() {
        return pila.contarProcesos();  // llama a la pila global
    }
};

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
    cout << "6. Ver historial de procesos ejecutados" << endl;
    cout << "7. Bloquear un proceso" << endl;
    cout << "8. Desbloquear un proceso" << endl;
    cout << "9. Modificar prioridad de un proceso" << endl;
    cout << "10. Mostrar estadísticas generales" << endl;
    cout << "11. Salir" << endl;
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

                do {
                    cout << "Prioridad (0-4): ";
                    cin >> prioridad;
                    if (prioridad < 0 || prioridad > 4) {
                        cout << "Prioridad inválida. Debe estar entre 0 y 4." << endl;
                    }
                } while (prioridad < 0 || prioridad > 4);

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
                pila.mostrarHistorial();
                break;

            case 7:
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

            case 8:
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

            case 9:
                cout << "Ingrese el ID del proceso a modificar: ";
                cin >> id;
                cout << "Ingrese la nueva prioridad (0-4): ";
                cin >> prioridad;
                lista.modificarPrioridadProceso(id, prioridad);
                break;

            case 10:
                lista.mostrarEstadisticas();
                break;

            case 11:
                cout << "Saliendo del sistema..." << endl;
                break;

            default:
                cout << "Opcion no valida. Intente nuevamente." << endl;
        }
    } while(opcion != 11);

    return 0;
}
