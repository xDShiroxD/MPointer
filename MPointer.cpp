#include <iostream>

template <typename T>
class MPointer {
    private:
        T* ptr;
        int ID; 

        MPointer(T* p) : ptr(p), ID(generadorID()) {} //Para que solo sea llamado con New()

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

        //Libera memoria
        ~MPointer() {
            delete ptr;
        }

        //Sobrecarga de operadores
        T& operator*() const {
            return *ptr;
        }

        T* operator&() const {
            return ptr;
        }

        MPointer& operator=(const MPointer& other) {
            if (this != static_cast<const void*>(&other)) {
                this->ptr = other.ptr;
                this->ID = other.ID;

                //Aqui se actualiza el GC

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