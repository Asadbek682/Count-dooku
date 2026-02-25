#include <iostream>
#include <vector>
#include <utility>

class Buffer {
private:
    int* data;
    size_t size;
    
public:
    Buffer(size_t s) : size(s) {
        data = new int[size];
        std::cout << "Constructor\n";
    }
    
    ~Buffer() {
        delete[] data;
        std::cout << "Destructor\n";
    }
    
    // Copy constructor
    Buffer(const Buffer& other) : size(other.size) {
        data = new int[size];
        std::copy(other.data, other.data + size, data);
        std::cout << "Copy constructor\n";
    }
    
    // Move constructor
    Buffer(Buffer&& other) noexcept : data(nullptr), size(0) {
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
        std::cout << "Move constructor\n";
    }
};

template<typename T>
void processBuffer(T&& buffer) {
    Buffer b = std::forward<T>(buffer);
    // Process buffer...
}

int main() {
    Buffer buf1(10);
    processBuffer(buf1);  // Lvalue: copy
    processBuffer(Buffer(5));  // Rvalue: move
    
    return 0;
}
