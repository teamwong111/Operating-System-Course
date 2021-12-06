#include <iostream>
#include <stdio.h>
using namespace std;
class Disk_manager
{
private:
    const char *disk_name = "D:\\1853862.img"; // 磁盘镜像文件名
    FILE *fp;                                  // 磁盘文件指针
public:
    Disk_manager();
    ~Disk_manager();
    bool is_exist();                                                         // 检查镜像文件是否存在
    void open_disk();                                                        // 打开镜像文件
    void write_disk(const void *buffer, unsigned int size, int offset = -1); // 实际写磁盘
    void read_disk(void *buffer, unsigned int size, int offset = -1);        // 实际读磁盘
};