#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <string.h>
#include "buffer.h"
using namespace std;
class Inode
{
public:
    int isdir;              // 是否是目录文件
    int mode;               // 0: start(close) 1: open & r 2: open & w 3: open & rw
    int offset;             // 文件当前偏移地址
    int file_size;          // 文件大小
    unsigned int child_num; // 子节点数量
    char name[12];          // (目录)文件名
    int d_addr[10];         // 指向文件数据区
    Inode *child[10];       // 子节点位置
    Inode *father;          // 父节点位置
    Inode();
};

class Inode_manager
{
public:
    Inode *root; //根目录
    Inode *cur;  //当前目录
    Inode_manager();
    ~Inode_manager();
    void get_inode();                                                             //读取inode
    string get_current_path();                                                    //获取当前路径
    void ls();                                                                    //列目录
    void mkdir(string name);                                                      //创建目录
    void cd(string name);                                                         //跳转目录
    void create(string name);                                                     //新建文件
    void delete_file(string name);                                                //删除文件
    void open_file(string name, string config);                                   //打开文件
    void close_file(string name);                                                 //关闭文件
    void write_file(string name, string filename, string str_size);               //写文件
    void read_file(string name, string isfile, string filename, string str_size); //读文件
    void seek_file(string name, string str_offset, string str_origin);            //定位文件读写指针
    int Bmap(int lbn);                                                            //逻辑块号与物理块号的转换
    Buffer *Alloc();                                                              //分配磁盘
    void Free(int blkno);                                                         //回收磁盘
    void save_inode();                                                            //保存inode
};