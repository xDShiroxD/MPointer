#include <iostream>

template <typename T>
class MPointer {
    private:
        T* ptr;

    public:
        explicit MPointer(T* p = nullptr) : ptr(p) {}

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

};
