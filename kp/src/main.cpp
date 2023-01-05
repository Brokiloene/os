#include <iostream>
#include <chrono>

#include "pow_of_two.hpp"
#include "mckk.hpp"

using namespace std::chrono;

int main(){
    std::vector<int> blocks_amount = {64, 32, 16, 4, 20, 10, 0};

    steady_clock::time_point pow2_init_start = steady_clock::now();
    pow_two_allocator pow_two_allocator(blocks_amount);
    steady_clock::time_point pow2_init_end = steady_clock::now();

    std::cout << "powers of two init time: " << 
    std::chrono::duration_cast<std::chrono::nanoseconds>(pow2_init_end - pow2_init_start).count() << " nanosec\n";
    
    int pages_cnt = 10;
    std::vector<int> pages_fragments = {32, 128, 256, 1024, 512, 256, 256, 1024, 16, 256};

    steady_clock::time_point mckk_init_start = steady_clock::now();
    mckk_allocator mckk_allocator(pages_cnt, pages_fragments);
    steady_clock::time_point mckk_init_end = steady_clock::now();

    std::cout << "mckk init time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(mckk_init_end - mckk_init_start).count() << " nanosec\n\n";

    std::cout << "test: allocate 10 char[256] + deallocate + allocate 10 char[128] + deallocate:\n";
    std::vector<char *> pointers1(10, 0);
    steady_clock::time_point pow2_test_start = steady_clock::now();
    for (int i = 0; i < 10; ++i) {
        pointers1[i] = (char *)pow_two_allocator.allocate(256);
    }
    for (int i = 0; i < 10; ++i) {
        pow_two_allocator.deallocate(pointers1[i]);
    }
    for (int i = 5; i < 10; ++i) {
        pointers1[i] = (char *)pow_two_allocator.allocate(128);
    }
    for (int i = 0; i < 10; ++i) {
        pow_two_allocator.deallocate(pointers1[i]);
    }
    steady_clock::time_point pow_two_test_end = steady_clock::now();
    std::cerr << "powers of two: " << std::chrono::duration_cast<std::chrono::microseconds>(pow_two_test_end - pow2_test_start).count() << " microsec\n";
    
    
    std::vector<char *> pointers2(10, 0);
    steady_clock::time_point mckk_test_start = steady_clock::now();
    for (int i = 0; i < 10; ++i) {
        pointers2[i] = (char *)mckk_allocator.allocate(256);
    }
    for (int i = 5; i < 10; ++i) {
        mckk_allocator.deallocate(pointers2[i]);
    }
    for (int i = 5; i < 10; ++i) {
        pointers2[i] = (char *)mckk_allocator.allocate(128);
    }
    for (int i = 0; i < 10; ++i) {
        mckk_allocator.deallocate(pointers2[i]);
    }
    steady_clock::time_point mckk_test_end = steady_clock::now();
    std::cerr << "mckk: " << std::chrono::duration_cast<std::chrono::microseconds>(mckk_test_end - mckk_test_start).count() << " microsec\n";
}