#include <iostream>
#include <vector>
#include <memory>

template<typename T>
class LoggingAllocator {
public:
    using value_type = T;
    
    LoggingAllocator() = default;
    
    template<typename U>
    LoggingAllocator(const LoggingAllocator<U>&) {}
    
    T* allocate(std::size_t n) {
        std::cout << "Allocating " << n << " objects of size " << sizeof(T) << std::endl;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    
    void deallocate(T* p, std::size_t n) {
        std::cout << "Deallocating " << n << " objects" << std::endl;
        ::operator delete(p);
    }
    
    template<typename U>
    bool operator==(const LoggingAllocator<U>&) const { return true; }
    
    template<typename U>
    bool operator!=(const LoggingAllocator<U>&) const { return false; }
};

int main() {
    std::vector<int, LoggingAllocator<int>> vec;
    
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }
    
    for (const auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
