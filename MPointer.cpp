#include <iostream>
#include "MPointerGC.cpp"

template <typename T>
class MPointer {
    private:
        T* puntero;
        int ID;
        int* contadorRef;
        MPointer(std::nullptr_t) : puntero(nullptr), contadorRef(nullptr), ID(-1) {}
        MPointer(T* p, int id) : puntero(p), contadorRef(new int(1)), ID(id) {} //Para que solo sea llamado con New()

    public:

        static MPointer<T> New() {
            T* nuevoPuntero = new T();
            int id = MPointerGC::getInstance()->registerPointer(nuevoPuntero, new int(1));
            return MPointer<T>(nuevoPuntero, id);
        }

        //Constuctor para aumentar el contador al copiar
        MPointer(const MPointer<T>& other) {
            puntero = other.puntero;
            contadorRef = other.contadorRef;
            ID = other.ID;
            ++(*contadorRef);
    }

        //Libera memoria
        ~MPointer() {
            if (--(*contadorRef) == 0) {
                delete puntero;
                delete contadorRef;
        }
    }

        //Sobrecarga de operadores
        T& operator*() const {
            return *puntero;
        }

        T* operator&() const {
            return puntero;
        }

    // Operador = para otro MPointer
    MPointer& operator=(const MPointer& other) {
        if (this != static_cast<const void*>(&other)) { // Evitar auto-asignación
            if (--(*contadorRef) == 0) {
                delete puntero;
                delete contadorRef;
            }
            puntero = other.puntero;
            contadorRef = other.contadorRef;
            ID = other.ID;
            ++(*contadorRef);
        }
        return *this;
}


    //Operador = para un valor del mismo tipo
    MPointer& operator=(const T& value) {
        //Asigna el valor al objeto apuntado
        *puntero = value;
        return *this;
    }

    T* operator->() const {
        return puntero;
    }

};


int main() {
    std::cout << "Iniciando pruebas de manejo de memoria con MPointer" << std::endl;

    // Crear el primer MPointer y asignar un valor
    MPointer<int> myPtr = MPointer<int>::New();
    *myPtr = 42;
    std::cout << "Valor de myPtr: " << *myPtr << " (esperado: 42)" << std::endl;

    // Crear un segundo MPointer
    MPointer<int> myPtr2 = MPointer<int>::New();
    *myPtr2 = 15;
    std::cout << "Valor de myPtr2: " << *myPtr2 << " (esperado: 15)" << std::endl;

    // Asignar myPtr a myPtr2
    myPtr2 = myPtr;
    std::cout << "Valor de myPtr2 después de asignar myPtr: " << *myPtr2 << " (esperado: 42)" << std::endl;

    // Asignar un valor a myPtr2 y verificar que ambos punteros apunten al mismo valor
    *myPtr2 = 100;
    std::cout << "Valor de myPtr2 después de asignar 100: " << *myPtr2 << " (esperado: 100)" << std::endl;
    std::cout << "Valor de myPtr después de asignar 100 a myPtr2: " << *myPtr << " (esperado: 100)" << std::endl;

    // Crear un tercer MPointer
    MPointer<int> myPtr3 = MPointer<int>::New();
    *myPtr3 = 75;
    std::cout << "Valor de myPtr3: " << *myPtr3 << " (esperado: 75)" << std::endl;

    // Liberar memoria al asignar un nuevo puntero a myPtr
    myPtr = MPointer<int>::New();
    *myPtr = 50;
    std::cout << "Valor de myPtr después de reasignar: " << *myPtr << " (esperado: 50)" << std::endl;

    // Esperar para que el garbage collector ejecute la limpieza
    std::cout << "Esperando para que el garbage collector limpie la memoria no utilizada..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(6)); // Tiempo suficiente para que el GC ejecute

    std::cout << "Fin de las pruebas de manejo de memoria con MPointer" << std::endl;

    return 0;
}