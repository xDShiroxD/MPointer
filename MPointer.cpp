#include <iostream>

template <typename T>
class MPointer {
    private:
        T* ptr;

        MPointer(T* p) : ptr(p) {} //Para que solo sea llamado con New()

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

        T operator&() const {
            return *ptr;
        }

};

int main() {
    MPointer<int> myPtr = MPointer<int>::New();  // Crea un MPointer<int> y reserva memoria para un int

    *myPtr = 5;  // Almacena el valor 5 en el entero encapsulado

    int valor = &myPtr;  // Obtiene el valor almacenado en myPtr
    std::cout << "Valor obtenido: " << valor << std::endl;

    return 0;
}
