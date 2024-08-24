#include <iostream>

template <typename T>
class MPointer {
    private:
        T* ptr;
        int ID;
        int* contadorRef;

        MPointer(T* p) : ptr(p), contadorRef(new int(1)), ID(generadorID()) {} //Para que solo sea llamado con New()

    //Generador de ID's unicos
    int generadorID() {
        static int id = 0;
        return ++id;
    }

    public:

        static MPointer<T> New() {
        T* newPtr = new T();  // Reserva de memoria para un objeto de tipo T
        return MPointer<T>(newPtr);
        }

        //Constuctor para aumentar el contador al copiar
        MPointer(const MPointer& other) : ptr(other.ptr), contadorRef(other.contadorRef), ID(other.ID){
            ++(*contadorRef);
        }

        //Libera memoria
        ~MPointer() {
            if (--(*contadorRef) == 0) {
                delete ptr;
                delete contadorRef;
        }
    }

        //Sobrecarga de operadores
        T& operator*() const {
            return *ptr;
        }

        T* operator&() const {
            return ptr;
        }

    // Operador = para otro MPointer
    MPointer& operator=(MPointer& other) {
        if (this != static_cast<const void*>(&other)) {
            if (--(*contadorRef) == 0) {
                delete ptr;
                delete contadorRef;
            }
                //copia los datos
                ptr = other.ptr;
                ID = other.ID;
                contadorRef = other.contadorRef;
                ++(*contadorRef);
        }
        return *this;
    }

    //Operador = para un valor del mismo tipo
    MPointer& operator=(const T& value) {
        //Asigna el valor al objeto apuntado
        *ptr = value;
        return *this;
    }

};

int main() {
    std::cout << "Prueba 1: Crear un MPointer<int> y asignar un valor directamente" << std::endl;
    MPointer<int> myPtr = MPointer<int>::New();  // Crea un MPointer<int> y reserva memoria para un int
    *myPtr = 5;  // Almacena el valor 5 en el entero encapsulado
    std::cout << "Valor de myPtr: " << *myPtr << " (esperado: 5)" << std::endl;

    std::cout << "\nPrueba 2: Crear otro MPointer<int> y asignarlo al primero" << std::endl;
    MPointer<int> myPtr2 = MPointer<int>::New();  // Crea otro MPointer<int>
    myPtr2 = myPtr;  // myPtr2 ahora apunta a la misma dirección que myPtr
    std::cout << "Valor de myPtr2 después de asignar myPtr: " << *myPtr2 << " (esperado: 5)" << std::endl;

    std::cout << "\nPrueba 3: Asignar un valor directamente a myPtr2" << std::endl;
    myPtr2 = 10;  // Asigna directamente el valor 10 al objeto apuntado por myPtr2
    std::cout << "Valor de myPtr2 después de asignar 10: " << *myPtr2 << " (esperado: 10)" << std::endl;
    std::cout << "Valor de myPtr (debe ser igual a myPtr2): " << *myPtr << " (esperado: 10)" << std::endl;

    std::cout << "\nPrueba 4: Crear un tercer MPointer<int> y asignarle un valor" << std::endl;
    MPointer<int> myPtr3 = MPointer<int>::New();
    myPtr3 = 20;
    std::cout << "Valor de myPtr3: " << *myPtr3 << " (esperado: 20)" << std::endl;

    std::cout << "\nPrueba 5: Asignar myPtr3 a myPtr2" << std::endl;
    myPtr2 = myPtr3;
    std::cout << "Valor de myPtr2 después de asignar myPtr3: " << *myPtr2 << " (esperado: 20)" << std::endl;
    std::cout << "Valor de myPtr3 (debe ser igual a myPtr2): " << *myPtr3 << " (esperado: 20)" << std::endl;

    std::cout << "\nPrueba 6: Verificar que myPtr todavía tiene el valor 10" << std::endl;
    std::cout << "Valor de myPtr: " << *myPtr << " (esperado: 10)" << std::endl;

    std::cout << "\nPrueba 7: Asignación encadenada" << std::endl;
    myPtr = myPtr2 = myPtr3;
    std::cout << "Valor de myPtr después de la asignación encadenada: " << *myPtr << " (esperado: 20)" << std::endl;

    std::cout << "\nPrueba 8: Destrucción y liberación de memoria (se espera que no haya errores)" << std::endl;
    // Al final del bloque, los destructores se llaman automáticamente, liberando la memoria
    // Si el conteo de referencias está correcto, no habrá errores de doble liberación.

    return 0;
}