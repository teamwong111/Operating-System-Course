#include "super_block.h"
#include "disk.h"

extern Disk_manager disk_manager;

Sb_manager::Sb_manager() {
    super_block = new Superblock();
}

Sb_manager::~Sb_manager() { }

void Sb_manager::save_superblock() {
    disk_manager.write_disk(super_block, sizeof(Superblock), 0);
    delete super_block;
}

void Sb_manager::format_disk() {
    super_block->s_isize = 10000;//随便，无所谓
    super_block->s_nfree = 0;
    super_block->s_free[0] = -1;
    disk_manager.open_disk();
    for(int i = 0; i < 512; ++i)
        disk_manager.write_disk(super_block, sizeof(Superblock), i*1024);
    char free_block1[512], free_block2[512];
    memset(free_block1, 0, 512);
    memset(free_block2, 0, 512);
    for (int i = 0; i < 15360; ++i) {
        if (super_block->s_nfree >= 100) {
            memcpy(free_block2, &super_block->s_nfree, sizeof(int) + sizeof(super_block->s_free));
            disk_manager.write_disk(&free_block2, 512);
            super_block->s_nfree = 0;
        }
        else {
            disk_manager.write_disk(free_block1, 512);
        }
        super_block->s_free[super_block->s_nfree++] = i + 1024;
    }
    disk_manager.write_disk(super_block, sizeof(Superblock), 0);
}

void Sb_manager::get_superblock() {
    disk_manager.read_disk(super_block, sizeof(Superblock), 0);
}