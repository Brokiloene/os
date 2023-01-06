#include "pow_of_two.hpp"

pow_two_allocator::pow_two_allocator(std::vector<int> &blocks_amount) {
    free_blocks_lists = std::vector<std::list<char *>>(pows_of_two.size());
    int bytes_sum = 0;
    for (int i = 0; i < blocks_amount.size(); ++i) {
        bytes_sum += blocks_amount[i] * pows_of_two[i];
    }
    data = (char *) malloc(bytes_sum);
    char *data_copy = data;
    for (int i = 0; i < blocks_amount.size(); ++i) {
        for (int j = 0; j < blocks_amount[i]; ++j) {
            free_blocks_lists[i].push_back(data_copy);
            *((int *)data_copy) = pows_of_two[i];
            data_copy += pows_of_two[i];
        }
    }
}

void *pow_two_allocator::allocate(int bytes_amount) {
    if (bytes_amount == 0) {
        return nullptr;
    }
    bytes_amount += sizeof(int);
    int ind = -1;
    for (int i = 0; i < free_blocks_lists.size(); ++i) {
        if (bytes_amount <= pows_of_two[i] && !free_blocks_lists[i].empty()) {
            ind = i;
            break;
        }
    }
    if (ind == -1) {
        std::cout << "There isn't memory\n";
    }

    char *memory = free_blocks_lists[ind].front();
    free_blocks_lists[ind].pop_front();
    return (void *)(memory + sizeof(int));
}

void pow_two_allocator::deallocate(void *ptr) {
    char *char_ptr = (char *)ptr;
    char_ptr = char_ptr - sizeof(int);
    int block_size = *((int *)char_ptr);
    int ind = -1;
    for(int i = 0; i < pows_of_two.size(); ++i) {
        if(pows_of_two[i] == block_size) {
            ind = i;
        }
    }

    free_blocks_lists[ind].push_back(char_ptr);
}

pow_two_allocator::~pow_two_allocator() {
    free(data);
}