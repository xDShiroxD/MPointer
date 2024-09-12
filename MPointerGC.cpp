#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <algorithm>

struct Node {
    void* puntero;      
    int* contadorRef;  
    Node* next;     

    Node(void* p, int* r) : puntero(p), contadorRef(r), next(nullptr) {}
};

class MPointerGC {
private:
    static MPointerGC* instance;
    Node* head;
    std::mutex mtx;
    bool running;
    int proxID;  // Almacena el próximo ID disponible

    MPointerGC() : head(nullptr), running(true), proxID(1) {
        std::thread([this]() {
            while (running) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                this->collectGarbage();
            }
        }).detach();
    }

    ~MPointerGC() {
        running = false;
        clearList();
    }

    void clearList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void collectGarbage() {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        Node* prev = nullptr;

        while (current != nullptr) {
            if (*(current->contadorRef) == 0) {
                std::cout << "Liberando memoria para puntero: " << current->puntero << std::endl;
                delete static_cast<int*>(current->puntero);
                delete current->contadorRef;

                if (prev == nullptr) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }

                Node* temp = current;
                current = current->next;
                delete temp;
            } else {
                prev = current;
                current = current->next;
            }
        }
    }

public:
    static MPointerGC* getInstance() {
        if (!instance) {
            instance = new MPointerGC();
        }
        return instance;
    }

    int registerPointer(void* puntero, int* contadorRef) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(puntero, contadorRef);
        newNode->next = head;
        head = newNode;
        std::cout << "Puntero registrado: " << puntero << ", ID asignado: " << proxID << std::endl;
        return proxID++;  // Asigna el ID y luego lo incrementa para el próximo puntero
    }

    void unregisterPointer(void* puntero) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->puntero == puntero) {
                if (prev == nullptr) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }

                delete current->contadorRef;  // Se elimina el contador de referencias
                delete current;
                std::cout << "Puntero eliminado: " << puntero << std::endl;
                break;
            }
            prev = current;
            current = current->next;
        }
    }

    MPointerGC(MPointerGC& other) = delete;
    void operator=(const MPointerGC&) = delete;
};

MPointerGC* MPointerGC::instance = nullptr;
