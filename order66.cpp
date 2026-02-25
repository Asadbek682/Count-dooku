#include <iostream>
#include <vector>
#include <algorithm>

// Sorting policies
struct AscendingSort {
    template<typename T>
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

struct DescendingSort {
    template<typename T>
    bool operator()(const T& a, const T& b) const {
        return a > b;
    }
};

// Printing policies
struct VerbosePrint {
    template<typename T>
    void operator()(const T& value, int index) const {
        std::cout << "Element[" << index << "] = " << value << "\n";
    }
};

struct SimplePrint {
    template<typename T>
    void operator()(const T& value, int) const {
        std::cout << value << " ";
    }
};

// Threading policies
struct SingleThreaded {
    void lock() const {}
    void unlock() const {}
};

struct MultiThreaded {
    void lock() const {
        std::cout << "Locking mutex\n";
    }
    void unlock() const {
        std::cout << "Unlocking mutex\n";
    }
};

// Main container with policies
template<typename T, 
         typename SortingPolicy = AscendingSort,
         typename PrintingPolicy = SimplePrint,
         typename ThreadingPolicy = SingleThreaded>
class DataContainer {
private:
    std::vector<T> data;
    SortingPolicy sorter;
    PrintingPolicy printer;
    ThreadingPolicy threader;
    
public:
    void add(const T& value) {
        threader.lock();
        data.push_back(value);
        threader.unlock();
    }
    
    void sort() {
        threader.lock();
        std::sort(data.begin(), data.end(), sorter);
        threader.unlock();
    }
    
    void print() const {
        threader.lock();
        std::cout << "Container contents:\n";
        for(size_t i = 0; i < data.size(); ++i) {
            printer(data[i], i);
        }
        std::cout << "\n";
        threader.unlock();
    }
};

int main() {
    // Different policy combinations
    DataContainer<int, AscendingSort, VerbosePrint> container1;
    DataContainer<int, DescendingSort, SimplePrint> container2;
    
    for(int i = 0; i < 5; ++i) {
        container1.add(i * 10);
        container2.add(i * 10);
    }
    
    std::cout << "Container 1 (Ascending + Verbose):\n";
    container1.sort();
    container1.print();
    
    std::cout << "\nContainer 2 (Descending + Simple):\n";
    container2.sort();
    container2.print();
    
    return 0;
}
