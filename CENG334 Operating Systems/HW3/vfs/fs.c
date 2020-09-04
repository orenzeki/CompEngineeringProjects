#include "fs.h"
#include "ext2_fs/ext2.h"

#include <stdlib.h>
#include <string.h>

#define BLOCK_OFFSET(block, blockSize) (1024 + (block-1)*blockSize)
#define INODE_OFFSET(inode_offset, inode) (inode_offset + ((inode)-1)*sizeof(struct ext2_inode))


int init_fs(const char *image_path) {
    current_fs = initialize_ext2(image_path);
    current_sb = current_fs->get_superblock(current_fs);
    return !(current_fs && current_sb);
}

struct file *openfile(char *path) {
    struct file *f = malloc(sizeof(struct file));
    f->f_path = malloc(strlen(path) + 1);
    strcpy(f->f_path, path);
    struct dentry *dir = pathwalk(path);
    if (!dir) {
        return NULL;
    }
    f->f_inode = dir->d_inode;
    free(dir);
    if (f->f_inode->f_op->open(f->f_inode, f)) {
        return NULL;
    }
    return f;
}

int closefile(struct file *f) {
    if (f->f_op->release(f->f_inode, f)) {
        printf("Error closing file\n");
    }
    free(f);
    f = NULL;
    return 0;
}

int readfile(struct file *f, char *buf, int size, loffset_t *offset) {
    if (*offset >= f->f_inode->i_size) {
        return 0;
    }
    if (*offset + size >= f->f_inode->i_size) {
        size = f->f_inode->i_size - *offset;
    }
    // May add llseek call
    return f->f_op->read(f, buf, size, offset);
}

struct dentry *pathwalk(char *path) {

    if(path[0] == '/' || path[0] == '.'){
        if(path[1] == '\0')
            return current_sb->s_root;
    }

    uint i_no = get_inode_no(path, EXT2_ROOT_INO);
    if(!i_no){
        return NULL;
    }
    struct inode my_inode;

    my_inode.i_ino= i_no;
    my_inode.i_sb = current_sb;
    current_sb->s_op->read_inode(&my_inode);

    struct dentry *dir_entry = malloc(sizeof(struct dentry));
    dir_entry->d_name = path;

    dir_entry = my_inode.i_op->lookup(&my_inode, dir_entry);
    return dir_entry;
}

int statfile(struct dentry *d_entry, struct kstat *k_stat){
    return d_entry->d_inode->i_op->getattr(d_entry, k_stat);
}
