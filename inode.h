#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <string.h>
#include "buffer.h"
using namespace std;
class Inode {
public:
    int isdir;// 是否是目录文件
    int mode;// 0: start(close) 1: open & r 2: open & w 3: open & rw
    int offset;// 文件当前偏移地址
    int file_size;// 文件大小
    unsigned int child_num;// 子节点数量
    char name[12]; // (目录)文件名
    int d_addr[10]; // 指向文件数据区
    Inode* child[10]; // 子节点位置
    Inode* father; // 父节点位置
    Inode();
};

class Inode_manager {
public:
    Inode* root;
    Inode* cur;
    Inode_manager();
    ~Inode_manager();
    void get_inode();
    string get_current_path();
    void ls();
    void mkdir(string name);
    void cd(string name);
    void create(string name);
    void delete_file(string name);
    void open_file(string name, string config);
    void close_file(string name);
    void write_file(string name, string filename, string str_size);
    void read_file(string name, string isfile, string filename, string str_size);
    void seek_file(string name, string str_offset, string str_origin);
    int Bmap(int lbn);
    Buffer* Alloc();
    void save_inode();
};