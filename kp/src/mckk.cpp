#include "mckk.hpp"

mckk_allocator::mckk_allocator(int pages_cnt, std::vector<int> &pages_fragments) {

    free_blocks_lists = std::vector<std::list<char *>>(pows_of_two.size());

    //std::cout << free_blocks_lists.size() << '\n';

    data = (char *) malloc(pages_cnt * PAGE_SIZE);
    char *cur_page_start = data;
    char *cur_page_end = cur_page_start + (PAGE_SIZE - 1);
    block_mem_info = std::vector<Page>(pages_cnt);

    for(int i = 0; i < pages_cnt; ++i){
        block_mem_info[i].block_size = pages_fragments[i];
        block_mem_info[i].start = cur_page_start;
        block_mem_info[i].end = cur_page_end;

        cur_page_start += PAGE_SIZE;
        cur_page_end += PAGE_SIZE;
    }

    for (int i = 0; i < block_mem_info.size(); ++i) {
        int ind = -1;
        for(int j = 0; j < pows_of_two.size(); ++j) {
            if(block_mem_info[i].block_size == pows_of_two[j]) {
                ind = j;
                break;
            }
        }
        char *cur_block_start = block_mem_info[i].start;
        for(int j = 0; j < PAGE_SIZE / block_mem_info[i].block_size; ++j) {
            free_blocks_lists[ind].push_back(cur_block_start);
            cur_block_start += block_mem_info[i].block_size;
        }
    }
}

void *mckk_allocator::allocate(int bytes_amount) {
    if (bytes_amount == 0) {
        return nullptr;
    }

    //std::cout << "1freeBlocksLists.size = " << free_blocks_lists.size() << std::endl;
    /*for (auto el : free_blocks_lists){
        std::cout << el.size() << std::endl;
    }*/
    int ind = -1;
    for (int i = 0; i < free_blocks_lists.size(); ++i) {
        if (bytes_amount <= pows_of_two[i] && !free_blocks_lists[i].empty()) { // if requested amount of bytes is fit and such block exists
            ind = i;
            break;
        }
    }
    if (ind == -1) {
        std::cout << "no memory\n";
        return nullptr;
    }

    char *memory = free_blocks_lists[ind].front();
    free_blocks_lists[ind].pop_front();
    return (void *)memory;
}

void mckk_allocator::deallocate(void *ptr) {
    if (ptr == nullptr) {
        return;
    }

    char *char_ptr = (char *)ptr;

    int ind_page = -1;
    for(int i = 0; i < block_mem_info.size(); ++i) {
        if(block_mem_info[i].start <= char_ptr && char_ptr <= block_mem_info[i].end) {
            ind_page = i;
            break;
        }
    }

    int ind_block = -1;
    for(int j = 0; j < pows_of_two.size(); ++j) {
        if(block_mem_info[ind_page].block_size == pows_of_two[j]) {
            ind_block = j;
            break;
        }
    }

    free_blocks_lists[ind_block].push_back(char_ptr);
}

mckk_allocator::~mckk_allocator() {
    free(data);
}