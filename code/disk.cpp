#include "disk.h"

Disk_manager::Disk_manager()
{
    fp = fopen(disk_name, "rb+");
}

Disk_manager::~Disk_manager()
{
    if (fp)
    {
        fclose(fp);
    }
}

bool Disk_manager::is_exist()
{
    return fp != NULL;
}

void Disk_manager::open_disk()
{
    fp = fopen(disk_name, "wb+");
    if (fp == NULL)
    {
        cout << "打开镜像" << disk_name << "失败！";
        exit(-1);
    }
}

void Disk_manager::write_disk(const void *buffer, unsigned int size, int offset)
{
    if (offset >= 0)
    {
        fseek(fp, offset, 0);
    }
    fwrite(buffer, size, 1, fp);
}

void Disk_manager::read_disk(void *buffer, unsigned int size, int offset)
{
    if (offset >= 0)
    {
        fseek(fp, offset, 0);
    }
    fread(buffer, size, 1, fp);
}