#include <iostream>
#include <type_traits>
#include <vector>
#include <list>

// SFINAE: Enable for integral types only
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
process(T value) {
    std::cout << "Processing integral: " << value << "\n";
}

// SFINAE: Enable for floating point types only
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type
process(T value) {
    std::cout << "Processing floating point: " << value << "\n";
}

// Check if type has begin() and end() methods
template<typename T>
class has_iterator {
private:
    template<typename U>
    static auto test(int) -> decltype(
        std::declval<U>().begin(),
        std::declval<U>().end(),
        std::true_type{}
    );
    
    template<typename>
    static std::false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

// Overload for containers with iterators
template<typename T>
typename std::enable_if<has_iterator<T>::value, void>::type
print_container(const T& container) {
    std::cout << "Container: ";
    for(const auto& elem : container) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

// Overload for non-containers
template<typename T>
typename std::enable_if<!has_iterator<T>::value, void>::type
print_container(const T&) {
    std::cout << "Not a container\n";
}

// Variadic templates with SFINAE
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
sum_all(T t) {
    return t;
}

template<typename T, typename... Args>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
sum_all(T t, Args... args) {
    return t + sum_all(args...);
}

int main() {
    // Type-based dispatching
    process(42);        // integral
    process(3.14);      // floating point
    // process("hello"); // error: no matching function
    
    // Container detection
    std::vector<int> vec = {1, 2, 3};
    std::list<double> lst = {1.1, 2.2, 3.3};
    int single = 42;
    
    print_container(vec);
    print_container(lst);
    print_container(single);
    
    // Variadic sum
    std::cout << "Sum: " << sum_all(1, 2, 3, 4, 5) << "\n";
    std::cout << "Sum: " << sum_all(1.5, 2.5, 3.5) << "\n";
    
    return 0;
}
