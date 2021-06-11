#include <iostream>
#include <string.h>
using namespace std;

class Superblock {
public:
    int	s_isize;// 盘块数
    int	s_nfree;// 直接管理的空闲盘块数量
    int	s_free[100];// 直接管理的空闲盘块索引表
    int	padding[154];// 填充使SuperBlock块大小等于1024字节，占据2个扇区
};

class Sb_manager {
public:
    Superblock* super_block;
    Sb_manager();
    ~Sb_manager();
    void format_disk();
    void get_superblock();
    void save_superblock();
};
