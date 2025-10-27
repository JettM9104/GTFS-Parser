#include <iostream>

template<typename... Args>
double average(Args... args) {
    // Use fold expression to sum all arguments
    double sum = (args + ...); 
    // Divide by the number of arguments (sizeof...(args))
    return sum / sizeof...(args);
}

int main() {
    std::cout << average(1, 2, 3, 4, 5) << std::endl;        // Output: 3
    std::cout << average(2.5, 3.5, 4.5) << std::endl;        // Output: 3.5
    std::cout << average(10, 20) << std::endl;               // Output: 15
}