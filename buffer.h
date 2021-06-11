#pragma once
#include <iostream>
#include <queue>
#include <unordered_map>
#include <string.h>
using namespace std;
class Buffer {
public:
    bool is_need_write;//是否需要延迟写
    int block_no;
    char buf[512];
};

class Buffer_manager {
public:
    queue <Buffer*> buf_blocks_queue;//共100个缓存块
    unordered_map<int, Buffer*> buf_map;
    Buffer_manager();
    ~Buffer_manager();
    Buffer* GetBlk(int block_no);
    Buffer* Bread(int block_no);
    void Bclear(Buffer *bp);
    void save_buffer();
};