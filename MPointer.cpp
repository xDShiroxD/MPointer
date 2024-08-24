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

    // Sacado de IA, evita que se haga doble delete
    MPointer& operator=(MPointer& other) {
        if (this != static_cast<const void*>(&other)) {
            if (--(*contadorRef) == 0) {
                delete ptr;
                delete contadorRef;
            }
                //En caso de no ser la ultima instancia copia los datos
                ptr = other.ptr;
                ID = other.ID;
                contadorRef = other.contadorRef;
                ++(*contadorRef);
        }
        return *this;
    }

};

int main() {
    MPointer<int> myPtr = MPointer<int>::New();  
    MPointer<int> myPtr2 = MPointer<int>::New(); 

    *myPtr = 5; 

    int valor = *myPtr;  
    std::cout << "Valor obtenido: " << valor << std::endl;

    myPtr2 = myPtr;  // Copia el puntero y el ID de myPtr a myPtr2
    std::cout << "Valor obtenido después de la asignación: " << *myPtr2 << std::endl;
    std::cout << "Direccion uno:" << &myPtr << std::endl;
    std::cout << "Direccion uno:" << &myPtr2 << std::endl;
    return 0;
}