#include <iostream>
#include <iomanip>
#include <sstream>

// Custom stream manipulator for binary output
struct binary_manip {
    int value;
    binary_manip(int v) : value(v) {}
};

std::ostream& operator<<(std::ostream& os, const binary_manip& bm) {
    std::string result;
    int val = bm.value;
    
    if(val == 0) result = "0";
    else {
        while(val > 0) {
            result = (val % 2 ? "1" : "0") + result;
            val /= 2;
        }
    }
    
    os << "0b" << result;
    return os;
}

binary_manip binary(int v) { return binary_manip(v); }

// Custom stream manipulator with parameters
class width_wrapper {
private:
    int width;
    char fill;
    
public:
    width_wrapper(int w, char f = ' ') : width(w), fill(f) {}
    
    friend std::ostream& operator<<(std::ostream& os, const width_wrapper& ww) {
        os << std::setw(ww.width) << std::setfill(ww.fill);
        return os;
    }
};

width_wrapper width(int w, char fill = ' ') { return width_wrapper(w, fill); }

// Manipulator that works on input streams
class skip_whitespace {
public:
    skip_whitespace() {}
    
    friend std::istream& operator>>(std::istream& is, const skip_whitespace&) {
        while(std::isspace(is.peek()))
            is.get();
        return is;
    }
};

int main() {
    // Binary output
    std::cout << "Binary representations:\n";
    for(int i = 0; i <= 16; ++i) {
        std::cout << std::setw(2) << i << " = " << binary(i) << "\n";
    }
    
    // Custom width manipulator
    std::cout << "\nWith custom width:\n";
    std::cout << width(10) << 42 << width(10, '*') << 42 << "\n";
    std::cout << width(10) << "hello" << width(10, '-') << "world\n";
    
    // Input skipping
    std::string input = "  42   hello   world  ";
    std::istringstream iss(input);
    
    int num;
    std::string s1, s2;
    
    iss >> skip_whitespace() >> num;
    iss >> skip_whitespace() >> s1;
    iss >> skip_whitespace() >> s2;
    
    std::cout << "\nParsed input:\n";
    std::cout << "Number: " << num << "\n";
    std::cout << "String1: " << s1 << "\n";
    std::cout << "String2: " << s2 << "\n";
    
    return 0;
}
