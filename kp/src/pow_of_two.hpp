
#include <iostream>
#include <list>
#include <vector>

class pow_two_allocator {
private:
    std::vector<std::list<char *>> free_blocks_lists;
    std::vector<int> pows_of_two = {16,32,64,128,256,512,1024};
    char *data;
    
public:
    pow_two_allocator(std::vector<int> &blocksAmount);

    void *allocate(int bytesAmount);
    void deallocate(void *ptr);
    ~pow_two_allocator();
};