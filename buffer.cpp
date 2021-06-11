#include "buffer.h"
#include "disk.h"

extern Disk_manager disk_manager;

Buffer_manager::Buffer_manager() {
    while (!buf_blocks_queue.empty()) {
        buf_blocks_queue.pop();
    }
    buf_map.clear();
}

Buffer_manager::~Buffer_manager() { }

void Buffer_manager::save_buffer() {
    buf_map.clear();
    while (!buf_blocks_queue.empty()) {
        Buffer* pb = buf_blocks_queue.front();
        buf_blocks_queue.pop();
        disk_manager.write_disk(pb->buf, 512, pb->block_no*512);
        delete pb;
    }
}

Buffer* Buffer_manager::GetBlk(int block_no) {
    Buffer* pb;
    if (buf_map.find(block_no) != buf_map.end()) {//先从已分配的找，看有没有
        pb = buf_map[block_no];
        return pb;
    }
    else if(buf_blocks_queue.size() < 100) {//看有没有继续分配的空闲空间
        pb = new Buffer();
        pb->block_no = block_no;
        buf_blocks_queue.push(pb);
        buf_map[block_no] = pb;
        return pb;
    }
    else {//取出最早分配进去的，换掉
        buf_map.erase(buf_blocks_queue.front()->block_no);
        if(buf_blocks_queue.front()->is_need_write == true) {
            disk_manager.write_disk(buf_blocks_queue.front()->buf, 512, buf_blocks_queue.front()->block_no*512);
        }
        Buffer* del_buf = buf_blocks_queue.front();
        buf_blocks_queue.pop();
        delete del_buf;
        pb = new Buffer();
        pb->block_no = block_no;
        buf_blocks_queue.push(pb);
        buf_map[block_no] = pb;
        return pb;
    }
}

Buffer* Buffer_manager::Bread(int block_no) {
    Buffer* pb = GetBlk(block_no);
	if (pb->is_need_write == true) {
		return pb;
	}
    else {
        disk_manager.read_disk(pb->buf, 512, pb->block_no*512);
        pb->is_need_write = true;
        return pb;
    }
}

void Buffer_manager::Bclear(Buffer *bp) {
	memset(bp->buf, 0, 512);
	return;
}
