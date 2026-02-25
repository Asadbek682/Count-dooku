#include <iostream>
#include <coroutine>
#include <memory>
#include <string>

template<typename T>
class Generator {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    
    struct promise_type {
        T current_value;
        
        Generator get_return_object() {
            return Generator(handle_type::from_promise(*this));
        }
        
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { std::terminate(); }
        
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        
        void return_void() {}
    };
    
    handle_type coro;
    
    Generator(handle_type h) : coro(h) {}
    ~Generator() { if(coro) coro.destroy(); }
    
    bool next() {
        coro.resume();
        return !coro.done();
    }
    
    T value() { return coro.promise().current_value; }
};

Generator<int> fibonacci() {
    int a = 0, b = 1;
    while(true) {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

Generator<std::string> string_generator() {
    co_yield "Hello";
    co_yield "World";
    co_yield "From";
    co_yield "Coroutines";
}

int main() {
    std::cout << "Fibonacci numbers:\n";
    auto fib = fibonacci();
    for(int i = 0; i < 10 && fib.next(); ++i) {
        std::cout << fib.value() << " ";
    }
    std::cout << "\n\n";
    
    std::cout << "String generator:\n";
    auto gen = string_generator();
    while(gen.next()) {
        std::cout << gen.value() << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
