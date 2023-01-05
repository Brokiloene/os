#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <map>

#define PAGE_SIZE 1024

struct Page {
    int block_size;
    char *start;
    char *end;
};

class mckk_allocator {
private:
    std::vector<int> pows_of_two = {16,32,64,128,256,512,1024};
    std::vector<std::list<char *>> free_blocks_lists;
    std::vector<Page> block_mem_info;
    char *data;
    
public:
    mckk_allocator(int pages_cnt, std::vector<int>& pages_fragments); // pages_fragments is a vector with sizes of blocks 
                                                                        // on which page is splitted
    void *allocate(int bytes_amount);
    void deallocate(void *ptr);

    ~mckk_allocator();

};