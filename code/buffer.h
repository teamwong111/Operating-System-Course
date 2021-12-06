#pragma once
#include <iostream>
#include <queue>
#include <unordered_map>
#include <string.h>
using namespace std;
class Buffer
{
public:
    bool is_need_write; //是否需要延迟写
    int block_no;       //磁盘物理块号
    char buf[512];      //缓存
};

class Buffer_manager
{
public:
    queue<Buffer *> buf_blocks_queue;     //共100个缓存块
    unordered_map<int, Buffer *> buf_map; //hash表，方便查询
    Buffer_manager();
    ~Buffer_manager();
    Buffer *GetBlk(int block_no); //申请缓存
    Buffer *Bread(int block_no);  //读磁盘
    void Bclear(Buffer *bp);      //清空缓存
    void save_buffer();           //保存缓存
};