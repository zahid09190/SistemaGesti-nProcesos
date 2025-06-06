Algoritmo GestionProcesos
    Definir procesos Como Cadena
    Definir prioridades Como Entero
    Definir memorias Como Entero
    Definir contador, i, opcion Como Entero
    Definir nombre, nombreBuscar Como Cadena
    Definir encontrado Como Logico
	
    Dimension procesos[100]
    Dimension prioridades[100]
    Dimension memorias[100]
	
    contador <- 0
	
    Repetir
        Escribir "----- MENÚ -----"
        Escribir "1. Agregar proceso"
        Escribir "2. Buscar proceso"
        Escribir "3. Mostrar procesos"
        Escribir "0. Salir"
        Escribir "Ingrese una opción:"
        Leer opcion
		
        Segun opcion Hacer
            Caso 1:
                Si contador < 100 Entonces
                    Escribir "Nombre del proceso:"
                    Leer nombre
                    Escribir "Prioridad:"
                    Leer prioridades[contador]
                    Escribir "Memoria (MB):"
                    Leer memorias[contador]
                    procesos[contador] <- nombre
                    contador <- contador + 1
                    Escribir "Proceso agregado exitosamente."
                SiNo
                    Escribir "No se pueden agregar más procesos (límite alcanzado)."
                FinSi
				
            Caso 2:
                Escribir "Ingrese el nombre del proceso a buscar:"
                Leer nombreBuscar
                encontrado <- Falso
                Para i <- 0 Hasta contador - 1
                    Si procesos[i] = nombreBuscar Entonces
                        Escribir "Proceso encontrado en la posición ", i + 1
                        Escribir "Nombre: ", procesos[i]
                        Escribir "Prioridad: ", prioridades[i]
                        Escribir "Memoria: ", memorias[i], " MB"
                        encontrado <- Verdadero
                    FinSi
                FinPara
                Si No encontrado Entonces
                    Escribir "Proceso no encontrado."
                FinSi
				
            Caso 3:
                Si contador = 0 Entonces
                    Escribir "No hay procesos registrados."
                SiNo
                    Para i <- 0 Hasta contador - 1
                        Escribir "----- Proceso ", i + 1, " -----"
                        Escribir "Nombre: ", procesos[i]
                        Escribir "Prioridad: ", prioridades[i]
                        Escribir "Memoria: ", memorias[i], " MB"
                    FinPara
                FinSi
				
            Caso 0:
                Escribir "Saliendo del programa..."
				
            De Otro Modo:
                Escribir "Opción no válida. Intente de nuevo."
        FinSegun
		
    Hasta Que opcion = 0
FinAlgoritmo
