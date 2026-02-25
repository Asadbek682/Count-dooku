#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Filter even numbers using lambda
    std::vector<int> evens;
    std::copy_if(numbers.begin(), numbers.end(), 
                 std::back_inserter(evens), 
                 [](int n) { return n % 2 == 0; });
    
    // Transform using lambda
    std::vector<int> squares;
    std::transform(numbers.begin(), numbers.end(),
                   std::back_inserter(squares),
                   [](int n) { return n * n; });
    
    // Accumulate with lambda
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0,
                              [](int acc, int n) { return acc + n; });
    
    std::cout << "Evens: ";
    for (int n : evens) std::cout << n << " ";
    std::cout << "\nSquares: ";
    for (int n : squares) std::cout << n << " ";
    std::cout << "\nSum: " << sum << std::endl;
    
    return 0;
}
