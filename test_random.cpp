#include <random>
#include <iostream>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < 100; ++i) {
        std::cout << dis(gen);
    }

    std::cout << std::endl;
    return 0;
}