#include <iostream>
#include <variant>
#include <string>
#include <vector>

// Visitor pattern with std::variant
struct Visitor {
    void operator()(int i) const {
        std::cout << "Integer: " << i << std::endl;
    }
    
    void operator()(double d) const {
        std::cout << "Double: " << d << std::endl;
    }
    
    void operator()(const std::string& s) const {
        std::cout << "String: " << s << std::endl;
    }
};

// Alternative using overload pattern
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

int main() {
    std::vector<std::variant<int, double, std::string>> values;
    
    values.push_back(42);
    values.push_back(3.14);
    values.push_back("Hello, Variant!");
    
    std::cout << "Using visitor pattern:\n";
    for (const auto& v : values) {
        std::visit(Visitor{}, v);
    }
    
    std::cout << "\nUsing overload pattern:\n";
    for (const auto& v : values) {
        std::visit(overload{
            [](int i) { std::cout << "int: " << i << std::endl; },
            [](double d) { std::cout << "double: " << d << std::endl; },
            [](const std::string& s) { std::cout << "string: " << s << std::endl; }
        }, v);
    }
    
    return 0;
}
