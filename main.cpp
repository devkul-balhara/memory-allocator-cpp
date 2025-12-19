#include <bits/stdc++.h>
using namespace std;

// Fixed-size block allocator
class MemoryPool{
    private:
    // Each free block stores a pointer to the next free block.
    // This forms a singly linked free list.
    class Block{
        public:
        Block* next;
    };

    size_t blockSize;
    size_t blockCount;
    char* memory;
    Block* freeList;

    public:
    MemoryPool(size_t blockSize, size_t blockCount)
        : blockSize(blockSize), blockCount(blockCount), freeList(nullptr) {

        memory = new char[blockSize * blockCount];

        // Split the memory buffer into fixed-size blocks
        // and link them together to form the initial free list.
        for (size_t i = 0; i < blockCount; i++) {
            Block* block = reinterpret_cast<Block*>(memory + i * blockSize);
            block->next = freeList;
            freeList = block;
        }
    }

    ~MemoryPool() {
        delete[] memory;
    }

    // Allocate a block by removing it from the free list.
    // Returns nullptr if the pool is exhausted.
    void* allocate() {
        if (!freeList) {
            return nullptr; // Out of memory
        }
        Block* block = freeList;
        freeList = freeList->next;
        return block;
    }

    // Return a block back to the free list.
    // The block is reused for future allocations.
    void deallocate(void* ptr){
        if (!ptr) return;
        Block* block = reinterpret_cast<Block*>(ptr);
        block->next = freeList;
        freeList = block;
    }
};

// Simple benchmark comparing malloc/free
// against the custom memory pool for small allocations.
void benchmark() {
    const int N = 1'000'000;

    // malloc/free benchmark
    auto start1 = chrono::high_resolution_clock::now();
    vector<void*> ptrs1;
    for (int i = 0; i < N; i++) {
        ptrs1.push_back(malloc(32));
    }
    for (void* p : ptrs1) {
        free(p);
    }
    auto end1 = chrono::high_resolution_clock::now();

    // MemoryPool benchmark
    MemoryPool pool(32, N);
    auto start2 = chrono::high_resolution_clock::now();
    vector<void*> ptrs2;
    for (int i = 0; i < N; i++) {
        ptrs2.push_back(pool.allocate());
    }
    for (void* p : ptrs2) {
        pool.deallocate(p);
    }
    auto end2 = chrono::high_resolution_clock::now();

    auto t1 = chrono::duration_cast<chrono::milliseconds>(end1 - start1).count();
    auto t2 = chrono::duration_cast<chrono::milliseconds>(end2 - start2).count();

    cout << "malloc/free time: " << t1 << " ms\n";
    cout << "MemoryPool time:  " << t2 << " ms\n";
}

int main(){
    benchmark();
    return 0;
}
