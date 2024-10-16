#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cstring>

constexpr size_t SIZE = 10 * 1024 * 1024;  // Array size (10 million elements)

// Utility to measure time in microseconds
template<typename Func>
long long measureTime(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

void test_aligned_access() {
    // Allocate memory with proper alignment (assumed to be a multiple of 64 bytes)
    float* aligned_memory = new float[SIZE];

    // Access all elements
    auto aligned_time = measureTime([&]() {
        for (size_t i = 0; i < SIZE; ++i) {
            aligned_memory[i] = aligned_memory[i] + 1.0f;
        }
    });

    std::cout << "Aligned access time: " << aligned_time << " microseconds" << std::endl;

    // Clean up
    // delete[] aligned_memory;
    std::free(aligned_memory);
}

void test_misaligned_access() {
    // Allocate extra space to intentionally misalign the memory
    void* raw_memory = std::malloc(SIZE * sizeof(float) + 64);
    // float* misaligned_memory = reinterpret_cast<float*>(reinterpret_cast<char*>(raw_memory) + 1); // Misalign by 1 byte
    float* misaligned_memory = reinterpret_cast<float*>(reinterpret_cast<char*>(raw_memory) + 63); // Misalign by 63 bytes

    // Access all elements
    auto misaligned_time = measureTime([&]() {
        for (size_t i = 0; i < SIZE; ++i) {
            misaligned_memory[i] = misaligned_memory[i] + 1.0f;
        }
    });

    std::cout << "Misaligned access time: " << misaligned_time << " microseconds" << std::endl;

    // Clean up
    std::free(raw_memory);
}

int main() {
    std::cout << "Testing memory access performance with alignment vs. misalignment...\n";

    // Run aligned access test
    test_aligned_access();

    // Run misaligned access test
    test_misaligned_access();

    return 0;
}