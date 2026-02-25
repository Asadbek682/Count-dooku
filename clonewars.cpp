#include <iostream>
#include <string_view>

// Compile-time string hashing (FNV-1a)
constexpr uint32_t fnv1a_hash(std::string_view str) {
    uint32_t hash = 2166136261u;
    for(char c : str) {
        hash ^= static_cast<uint32_t>(c);
        hash *= 16777619u;
    }
    return hash;
}

// User-defined literal for compile-time hashing
constexpr uint32_t operator"" _hash(const char* str, size_t len) {
    return fnv1a_hash(std::string_view(str, len));
}

// Switch-like construct using hashes
void handle_command(std::string_view cmd) {
    switch(fnv1a_hash(cmd)) {
        case "start"_hash:
            std::cout << "Starting...\n";
            break;
        case "stop"_hash:
            std::cout << "Stopping...\n";
            break;
        case "pause"_hash:
            std::cout << "Pausing...\n";
            break;
        case "resume"_hash:
            std::cout << "Resuming...\n";
            break;
        default:
            std::cout << "Unknown command: " << cmd << "\n";
    }
}

// Compile-time string manipulation
constexpr std::string_view trim(std::string_view str) {
    auto start = str.begin();
    while(start != str.end() && (*start == ' ' || *start == '\t'))
        ++start;
    
    auto end = str.end() - 1;
    while(end >= start && (*end == ' ' || *end == '\t'))
        --end;
    
    return std::string_view(start, end - start + 1);
}

int main() {
    // Compile-time hash verification
    constexpr auto hash1 = fnv1a_hash("hello");
    constexpr auto hash2 = "hello"_hash;
    static_assert(hash1 == hash2);
    
    std::cout << "Hash of 'hello': " << hash1 << "\n\n";
    
    // Command handling
    handle_command("start");
    handle_command("stop");
    handle_command("restart");
    
    // Compile-time string trimming
    constexpr std::string_view str = "   hello world   ";
    constexpr auto trimmed = trim(str);
    std::cout << "\nTrimmed: '" << trimmed << "'\n";
    
    return 0;
}
