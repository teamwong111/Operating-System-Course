#include "inode.h"
#include "buffer.h"
#include "super_block.h"
#include "disk.h"

extern Disk_manager disk_manager;
extern Buffer_manager buffer_manager;
extern Sb_manager sb_manager;

Inode::Inode()
{
    isdir = 0;
    mode = 0;
    offset = 0;
    file_size = 0;
    child_num = 0;
    memset(name, 0, sizeof(name));
    memset(d_addr, 0, sizeof(d_addr));
    memset(child, 0, sizeof(child));
    father = NULL;
}

Inode_manager::Inode_manager()
{
    root = new Inode();
    strcpy(root->name, "[ tj-os ");
    cur = root;
}

Inode_manager::~Inode_manager() {}

void Inode_manager::save_inode()
{
    int index = 0;
    queue<Inode *> inode_queue;
    inode_queue.push(root);
    while (!inode_queue.empty())
    {
        Inode *front = inode_queue.front();
        disk_manager.write_disk(front, sizeof(Inode), sizeof(Inode) * index + 1024);
        index++;
        inode_queue.pop();
        for (unsigned int i = 0; i < front->child_num; ++i)
        {
            inode_queue.push(front->child[i]);
        }
        delete front;
    }
}

void Inode_manager::get_inode()
{
    int index = 0;
    queue<Inode *> inode_queue;
    disk_manager.read_disk(root, sizeof(Inode), sizeof(Inode) * index + 1024);
    inode_queue.push(root);
    if (strcmp(root->name, "[ tj-os ") != 0)
    {
        root->isdir = 0;
        root->mode = 0;
        root->offset = 0;
        root->file_size = 0;
        root->child_num = 0;
        memset(root->d_addr, 0, sizeof(root->d_addr));
        memset(root->child, 0, sizeof(root->child));
        root->father = NULL;
        strcpy(root->name, "[ tj-os ");
    }
    while (!inode_queue.empty())
    {
        Inode *front = inode_queue.front();
        inode_queue.pop();
        for (unsigned int i = 0; i < front->child_num; ++i)
        {
            index++;
            Inode *sub = new Inode();
            disk_manager.read_disk(sub, sizeof(Inode), sizeof(Inode) * index + 1024);
            inode_queue.push(sub);
            sub->father = front;
            front->child[i] = sub;
        }
    }
}

string Inode_manager::get_current_path()
{
    string result = "";
    stack<string> paths;
    paths.push(cur->name);
    Inode *node = cur;
    while (node->father != NULL)
    {
        paths.push(node->father->name);
        node = node->father;
    }
    while (!paths.empty())
    {
        result += paths.top();
        result += '/';
        paths.pop();
    }
    return result;
}

void Inode_manager::ls()
{
    for (unsigned int i = 0; i < cur->child_num; ++i)
    {
        cout << cur->child[i]->name << endl;
    }
}

void Inode_manager::mkdir(string name)
{
    if (cur->child_num >= 10)
    {
        cout << "此目录下文件数到达上限" << endl;
        return;
    }
    for (unsigned int i = 0; i < cur->child_num; ++i)
    {
        if (strcmp(cur->child[i]->name, name.c_str()) == 0)
        {
            cout << "命名有冲突" << endl;
            return;
        }
    }
    Inode *node = new Inode();
    node->father = cur;
    strcpy(node->name, name.c_str());
    node->isdir = 1;
    node->mode = 0;
    node->offset = 0;
    node->file_size = 0;
    cur->child[cur->child_num] = node;
    cur->child_num += 1;
}

void Inode_manager::cd(string name)
{
    if (name == "..")
    {
        if (cur == root)
        {
            cout << "现在在根目录" << endl;
        }
        else
        {
            cur = cur->father;
        }
    }
    else
    {
        for (unsigned int i = 0; i < cur->child_num; ++i)
        {
            if (cur->child[i]->isdir == 1 && strcmp(cur->child[i]->name, name.c_str()) == 0)
            {
                cur = cur->child[i];
                return;
            }
        }
        cout << "当前目录下没有要去往的地方";
    }
}

void Inode_manager::create(string name)
{
    if (cur->child_num >= 10)
    {
        cout << "此目录下文件数到达上限" << endl;
        return;
    }
    for (unsigned int i = 0; i < cur->child_num; ++i)
    {
        if (strcmp(cur->child[i]->name, name.c_str()) == 0)
        {
            cout << "命名有冲突" << endl;
            return;
        }
    }
    Inode *node = new Inode();
    node->father = cur;
    strcpy(node->name, name.c_str());
    node->isdir = 0;
    node->mode = 0;
    node->offset = 0;
    node->file_size = 0;
    cur->child[cur->child_num] = node;
    cur->child_num += 1;
}

void Inode_manager::delete_file(string name)
{
    for (unsigned int i = 0; i < cur->child_num; ++i)
    {
        if (strcmp(cur->child[i]->name, name.c_str()) == 0)
        {
            Inode *node = cur->child[i];
            for (unsigned int j = i; j < cur->child_num - 1; ++j)
            {
                cur->child[j] = cur->child[j + 1];
            }
            cur->child[cur->child_num - 1] = NULL;
            cur->child_num -= 1;
            if (node->isdir == false)
            {
                for (int j = 0; j < 10; ++j)
                {
                    if (node->d_addr[j] != 0)
                    {
                        Free(node->d_addr[j]);
                    }
                }
            }
            delete node;
            return;
        }
    }
    cout << "没找到这个文件" << endl;
    return;
}

void Inode_manager::open_file(string name, string config)
{
    for (unsigned int i = 0; i < cur->child_num; ++i)
    {
        if (strcmp(cur->child[i]->name, name.c_str()) == 0)
        {
            if (config == "-r")
            {
                cur->child[i]->mode = 1;
            }
            else if (config == "-w")
            {
                cur->child[i]->mode = 2;
            }
            else if (config == "-rw")
            {
                cur->child[i]->mode = 3;
            }
            else
            {
                cout << "模式错误" << endl;
            }
            return;
        }
    }
    cout << "找不到这个文件" << endl;
}

void Inode_manager::close_file(string name)
{
    for (unsigned int i = 0; i < cur->child_num; ++i)
    {
        if (strcmp(cur->child[i]->name, name.c_str()) == 0)
        {
            cur->child[i]->mode = 0;
            return;
        }
    }
    cout << "找不到这个文件" << endl;
}

void Inode_manager::write_file(string name, string filename, string str_size)
{
    int size = atoi(str_size.c_str());
    if (size < 1)
    {
        cout << "要写入的字节数<=0" << endl;
        return;
    }
    fstream fin(filename, ios::in | ios::binary);
    if (!fin)
    {
        cout << filename << "打开失败!" << endl;
        return;
    }
    char *buffer = new char[size];
    fin.read(buffer, size);
    fin.close();
    int buffer_offset = 0;
    bool isexist = false;
    for (unsigned int i = 0; i < cur->child_num; ++i)
    {
        if (strcmp(cur->child[i]->name, name.c_str()) == 0)
        {
            isexist = true;
            Inode *now = cur->child[i];
            if (now->mode == 2 || now->mode == 3)
            {
                now->file_size += size;
                while (size)
                {
                    int lbn = now->offset / 512;
                    int offset = now->offset % 512;
                    int nbytes = min(512 - offset, size);
                    int bn = Bmap(lbn);
                    Buffer *pb;
                    if (512 == nbytes)
                    {
                        pb = buffer_manager.GetBlk(bn); // 如果写入数据正好满一个字符块，则为其分配缓存
                    }
                    else
                    {
                        pb = buffer_manager.Bread(bn); // 写入数据不满一个字符块，先读后写（读出该字符块以保护不需要重写的数据)
                    }
                    char *start = pb->buf + offset; // 缓存中数据的起始写位置 写操作: 从用户目标区拷贝数据到缓冲区
                    memcpy(start, buffer + buffer_offset, nbytes);
                    buffer_offset += nbytes;
                    now->offset += nbytes;
                    size -= nbytes;
                    pb->is_need_write = true; // 将缓存标记为延迟写，该缓存移作它用时才写入硬盘
                }
                delete buffer;
            }
            else
            {
                cout << "这文件没打开" << endl;
                return;
            }
        }
    }
    if (isexist == false)
    {
        cout << "找不到这个文件" << endl;
    }
}

void Inode_manager::read_file(string name, string isfile, string filename, string str_size)
{
    int size = atoi(str_size.c_str());
    if (size < 1)
    {
        cout << "要读的字节数<=0" << endl;
        return;
    }
    char *buffer = new char[size];
    int buffer_offset = 0;
    int file_size = size;
    bool isexist = false;
    for (unsigned int i = 0; i < cur->child_num; ++i)
    {
        if (strcmp(cur->child[i]->name, name.c_str()) == 0)
        {
            isexist = true;
            Inode *now = cur->child[i];
            file_size = min(file_size, now->file_size - now->offset);
            if (now->mode == 1 || now->mode == 3)
            {
                while (size)
                {
                    int remain = now->file_size - now->offset; // 文件剩余大小
                    if (remain <= 0)
                    {
                        break;
                    }
                    int lbn = now->offset / 512;
                    int offset = now->offset % 512;
                    int nbytes = min(512 - offset, size); //需传送的字节数，见s29p15
                    int bn = Bmap(lbn);
                    nbytes = min(nbytes, remain);          // 传送的字节数量还取决于剩余文件的长度
                    Buffer *pb = buffer_manager.Bread(bn); // 物理块读到缓存
                    char *start = pb->buf + offset;        // 缓存中数据起始读位置
                    memcpy(buffer + buffer_offset, start, nbytes);
                    buffer_offset += nbytes;
                    now->offset += nbytes;
                    size -= nbytes;
                }
                if (isfile == "-f")
                {
                    cout << "这是读出来的:" << endl;
                    for (int j = 0; j < file_size; ++j)
                    {
                        cout << buffer[j];
                    }
                    cout << endl;
                }
                break;
            }
            else
            {
                cout << "这文件没打开" << endl;
                return;
            }
        }
    }
    if (isfile == "-o")
    {
        fstream fout(filename, ios::out | ios::binary);
        if (!fout)
        {
            cout << filename << "打开失败!" << endl;
            return;
        }
        fout.write(buffer, file_size);
        fout.close();
    }
    delete buffer;
    if (isexist == false)
    {
        cout << "找不到这个文件" << endl;
    }
}

void Inode_manager::seek_file(string name, string str_offset, string str_origin)
{
    int offset = atoi(str_offset.c_str());
    int origin = atoi(str_origin.c_str());
    bool isexist = false;
    for (unsigned int i = 0; i < cur->child_num; ++i)
    {
        if (strcmp(cur->child[i]->name, name.c_str()) == 0)
        {
            isexist = true;
            Inode *now = cur->child[i];
            switch (origin)
            {
            case 0:
                now->offset = offset;
                break;
            case 1: // 读写位置加offset(可正可负)
                now->offset += offset;
                break;
            case 2: // 读写位置调整为文件长度加offset
                now->offset = now->file_size + offset;
                break;
            default:
                cout << "模式错误" << endl;
                break;
            }
            break;
        }
    }
    if (isexist == false)
    {
        cout << "找不到这个文件" << endl;
    }
}

int Inode_manager::Bmap(int lbn)
{
    Buffer *first_pb, *second_pb;
    int phy_block_no, index;
    int *i_table;
    if (lbn < 6)
    { // 如果是小型文件，从基本索引表[0-5]中获得物理盘块号即可
        phy_block_no = cur->d_addr[lbn];
        if (phy_block_no == 0)
        { // 如果该逻辑块号还没有相应的物理盘块号与之对应，则分配一个物理块
            first_pb = Alloc();
            if (first_pb != NULL)
            {
                phy_block_no = first_pb->block_no;
                first_pb->is_need_write = true;
                cur->d_addr[lbn] = phy_block_no;
            }
        }
        return phy_block_no;
    }
    if (lbn < 262)
    { // 大型文件
        index = (lbn - 6) / 128 + 6;
    }
    else
    { // 巨型文件
        index = (lbn - 262) / (128 * 128) + 8;
    }
    phy_block_no = cur->d_addr[index];
    if (phy_block_no)
    {
        first_pb = buffer_manager.Bread(phy_block_no);
    }
    else
    { // 若该项为零，则表示不存在相应的间接索引表块
        first_pb = Alloc();
        if (first_pb == 0)
        {
            return 0;
        }
        cur->d_addr[index] = first_pb->block_no;
    }
    i_table = (int *)first_pb->buf;
    if (index >= 8)
    {
        // 对于巨型文件的情况，first_pb中是二次间接索引表，还需根据逻辑块号，经由二次间接索引表找到一次间接索引表
        index = ((lbn - 262) / 128) % 128;
        phy_block_no = i_table[index];
        if (phy_block_no)
        {
            second_pb = buffer_manager.Bread(phy_block_no);
        }
        else
        {
            second_pb = Alloc();
            if (second_pb == NULL)
            {
                return 0;
            }
            i_table[index] = second_pb->block_no;
            first_pb->is_need_write = true;
        }
        first_pb = second_pb;
        i_table = (int *)second_pb->buf;
    }

    if (lbn < 262)
    {
        index = (lbn - 6) % 128;
    }
    else
    {
        index = (lbn - 262) % 128;
    }
    phy_block_no = i_table[index];
    second_pb = Alloc();
    if (phy_block_no == 0 && second_pb != NULL)
    {
        phy_block_no = second_pb->block_no;
        i_table[index] = phy_block_no;
        second_pb->is_need_write = true;
        first_pb->is_need_write = true;
    }
    else
    {
        first_pb->is_need_write = true;
    }
    return phy_block_no;
}

Buffer *Inode_manager::Alloc()
{
    int block_no;
    Buffer *pb;
    block_no = sb_manager.super_block->s_free[--sb_manager.super_block->s_nfree];
    if (block_no <= 0)
    {
        sb_manager.super_block->s_nfree = 0;
        cout << "没有磁盘空间了" << endl;
        return NULL;
    }
    if (sb_manager.super_block->s_nfree <= 0)
    {
        pb = buffer_manager.Bread(block_no);
        int *p = (int *)pb->buf;
        sb_manager.super_block->s_nfree = *p++;
        memcpy(sb_manager.super_block->s_free, p, sizeof(sb_manager.super_block->s_free));
    }
    pb = buffer_manager.GetBlk(block_no);
    if (pb)
    {
        buffer_manager.Bclear(pb);
    }
    return pb;
}

void Inode_manager::Free(int blkno)
{
    Buffer *pb = buffer_manager.GetBlk(blkno);
    buffer_manager.Bclear(pb);
    if (sb_manager.super_block->s_nfree >= 100)
    {
        int *p = (int *)pb->buf;
        *p++ = sb_manager.super_block->s_nfree;
        memcpy(p, sb_manager.super_block->s_free, sizeof(int) * 100);
        sb_manager.super_block->s_nfree = 0;
        disk_manager.write_disk(pb->buf, 512, pb->block_no * 512);
    }
    sb_manager.super_block->s_free[sb_manager.super_block->s_nfree++] = blkno;
}