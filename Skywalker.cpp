#include <iostream>
#include <vector>
#include <cstdint>

class MemoryPool {
private:
    struct Block {
        Block* next;
    };
    
    Block* freeList;
    char* pool;
    size_t blockSize;
    size_t poolSize;
    
public:
    MemoryPool(size_t blockSize, size_t numBlocks) 
        : blockSize(blockSize), poolSize(blockSize * numBlocks) {
        
        pool = new char[poolSize];
        freeList = reinterpret_cast<Block*>(pool);
        
        Block* current = freeList;
        for(size_t i = 0; i < numBlocks - 1; ++i) {
            Block* next = reinterpret_cast<Block*>(
                pool + (i + 1) * blockSize
            );
            current->next = next;
            current = next;
        }
        current->next = nullptr;
    }
    
    void* allocate() {
        if(freeList == nullptr)
            throw std::bad_alloc();
            
        Block* block = freeList;
        freeList = freeList->next;
        return block;
    }
    
    void deallocate(void* ptr) {
        if(ptr == nullptr) return;
        
        Block* block = static_cast<Block*>(ptr);
        block->next = freeList;
        freeList = block;
    }
    
    ~MemoryPool() {
        delete[] pool;
    }
};

struct Point {
    int x, y, z;
};

int main() {
    MemoryPool pool(sizeof(Point), 10);
    
    std::vector<Point*> points;
    
    // Allocate points from pool
    for(int i = 0; i < 5; ++i) {
        Point* p = static_cast<Point*>(pool.allocate());
        p->x = i;
        p->y = i * 2;
        p->z = i * 3;
        points.push_back(p);
    }
    
    // Use points
    for(const auto& p : points) {
        std::cout << "Point: (" << p->x << ", " << p->y << ", " << p->z << ")\n";
    }
    
    // Return to pool
    for(auto& p : points) {
        pool.deallocate(p);
    }
    
    return 0;
}
