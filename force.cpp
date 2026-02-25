#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

template<typename... Args>
class Signal {
private:
    struct Connection {
        std::function<void(Args...)> slot;
        int id;
        bool blocked;
        
        Connection(std::function<void(Args...)> s, int i) 
            : slot(s), id(i), blocked(false) {}
    };
    
    std::vector<std::shared_ptr<Connection>> connections;
    int next_id = 0;
    
public:
    class ScopedConnection {
    private:
        Signal* signal;
        int id;
        
    public:
        ScopedConnection(Signal* s, int i) : signal(s), id(i) {}
        
        ScopedConnection(const ScopedConnection&) = delete;
        
        ScopedConnection(ScopedConnection&& other) noexcept 
            : signal(other.signal), id(other.id) {
            other.signal = nullptr;
        }
        
        ~ScopedConnection() {
            if(signal) signal->disconnect(id);
        }
        
        void block() { signal->block(id); }
        void unblock() { signal->unblock(id); }
    };
    
    ScopedConnection connect(std::function<void(Args...)> slot) {
        auto conn = std::make_shared<Connection>(slot, next_id);
        connections.push_back(conn);
        return ScopedConnection(this, next_id++);
    }
    
    void emit(Args... args) {
        for(const auto& conn : connections) {
            if(!conn->blocked) {
                conn->slot(args...);
            }
        }
    }
    
    void disconnect(int id) {
        connections.erase(
            std::remove_if(connections.begin(), connections.end(),
                [id](const auto& conn) { return conn->id == id; }),
            connections.end()
        );
    }
    
    void block(int id) {
        for(auto& conn : connections) {
            if(conn->id == id) {
                conn->blocked = true;
                break;
            }
        }
    }
    
    void unblock(int id) {
        for(auto& conn : connections) {
            if(conn->id == id) {
                conn->blocked = false;
                break;
            }
        }
    }
};

// Example usage
class Button {
public:
    Signal<> onClick;
    Signal<int> onDoubleClick;
    Signal<std::string, int> onTextChanged;
    
    void click() {
        std::cout << "Button clicked\n";
        onClick.emit();
    }
    
    void doubleClick(int clicks) {
        std::cout << "Button double-clicked: " << clicks << "\n";
        onDoubleClick.emit(clicks);
    }
    
    void textChanged(const std::string& text, int length) {
        std::cout << "Text changed to: " << text << "\n";
        onTextChanged.emit(text, length);
    }
};

class Logger {
public:
    void logClick() {
        std::cout << "Logger: Click event logged\n";
    }
    
    void logDoubleClick(int clicks) {
        std::cout << "Logger: Double click with " << clicks << " clicks\n";
    }
    
    void logTextChange(const std::string& text, int length) {
        std::cout << "Logger: Text changed to '" << text 
                  << "' (length: " << length << ")\n";
    }
};

int main() {
    Button button;
    Logger logger;
    
    // Connect signals
    auto conn1 = button.onClick.connect([&]() { logger.logClick(); });
    auto conn2 = button.onDoubleClick.connect(
        [&](int clicks) { logger.logDoubleClick(clicks); }
    );
    
    // Scoped connection
    {
        auto conn3 = button.onTextChanged.connect(
            [&](const std::string& text, int len) {
                logger.logTextChange(text, len);
            }
        );
        
        std::cout << "Inside scope:\n";
        button.textChanged("Hello", 5);
    } // conn3 goes out of scope, automatically disconnects
    
    std::cout << "\nOutside scope (text handler disconnected):\n";
    button.textChanged("World", 5);
    
    // Test other signals
    std::cout << "\nTesting other signals:\n";
    button.click();
    button.doubleClick(2);
    
    // Block a connection
    std::cout << "\nBlocking click handler:\n";
    conn1.block();
    button.click();
    
    std::cout << "\nUnblocking click handler:\n";
    conn1.unblock();
    button.click();
    
    return 0;
}
