#include "ext2.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define BASE_OFFSET 1024

struct super_operations s_op;
struct inode_operations i_op;
struct file_operations f_op;

char fs_name[] = "ext2";

uint bsize = 0;

#define INODE_SIZE sizeof(struct ext2_inode)
#define BLOCK_OFFSET(block, bsize) (BASE_OFFSET + (block - 1) * bsize)
#define INODE_OFFSET(inode_offset, inode) (inode_offset + ((inode)-1)*INODE_SIZE)
#define GROUP_NUMBER(ipg, inode_no) ((inode_no-1) / ipg)+1
#define INODE_NUMBER(ipg, inode_no) ((inode_no-1) % ipg)+1

struct ext2_inode read_ext2_inode(uint iID) {
    struct ext2_inode ino;
    uint grp_no = GROUP_NUMBER(current_sb->s_inodes_per_group, iID);
    uint inode_no = INODE_NUMBER(current_sb->s_inodes_per_group, iID);
    uint base_off = BASE_OFFSET + ((grp_dsc_lst[grp_no-1].bg_inode_table-1)*current_sb->s_blocksize);
    uint off = INODE_OFFSET(base_off, inode_no);
    lseek(myfs.file_descriptor, off, SEEK_SET);
    read(myfs.file_descriptor, &ino, sizeof(struct ext2_inode));
    return ino;
}

void read_directory(struct ext2_inode *inode, struct ext2_dir_entry *dir_entry, char* name, uint size, int index){

}

void fill_inode(struct inode *i, struct ext2_inode inode){
    i->i_mode = inode.i_mode;
    i->i_nlink = inode.i_links_count;
    i->i_uid = inode.i_uid;
    i->i_gid = inode.i_gid;
    i->i_size = inode.i_size;
    i->i_atime = inode.i_atime;
    i->i_mtime = inode.i_mtime;
    i->i_ctime = inode.i_ctime;
    i->i_blocks = inode.i_blocks;
    for (int j = 0; j < 15; ++j) {
        i->i_block[j] = inode.i_block[j];
    }
    i->i_op = &i_op;
    i->f_op = &f_op;
    i->i_flags = inode.i_flags;
}

struct super_block *get_superblock(struct file_system_type *fst){
    struct ext2_super_block superblock;
    struct super_block *sblock = malloc (sizeof (struct super_block));
    lseek(fst->file_descriptor, BASE_OFFSET, SEEK_SET);
    read(fst->file_descriptor, &superblock, sizeof(superblock));
    if (superblock.s_magic != EXT2_SUPER_MAGIC) {
        fprintf(stderr, "Not a Ext2 filesystem\n");
        exit(1);
    }
    bsize = BASE_OFFSET << superblock.s_log_block_size;

    grp_count = superblock.s_inodes_count / superblock.s_inodes_per_group;
    grp_dsc_lst = malloc(grp_count*sizeof(struct ext2_group_desc));

    lseek(fst->file_descriptor, BASE_OFFSET+bsize, SEEK_SET);
    read(fst->file_descriptor, grp_dsc_lst, grp_count*sizeof(struct ext2_group_desc));
    struct ext2_inode r_inode;
    uint grp_no = GROUP_NUMBER(superblock.s_inodes_per_group, EXT2_ROOT_INO);
    uint inode_no = INODE_NUMBER(superblock.s_inodes_per_group, EXT2_ROOT_INO);
    uint base_off = 1024 + ((grp_dsc_lst[grp_no-1].bg_inode_table-1)*bsize);
    uint off = INODE_OFFSET(base_off, inode_no);
    lseek(myfs.file_descriptor, off, SEEK_SET);
    read(myfs.file_descriptor, &r_inode, sizeof(struct ext2_inode));

    struct ext2_dir_entry dir_entry;
    lseek(fst->file_descriptor, (r_inode.i_block[0]), SEEK_SET);
    read(fst->file_descriptor, &dir_entry, sizeof(struct ext2_dir_entry));



    sblock->s_block_group_nr = superblock.s_block_group_nr;
    sblock->s_blocks_count = superblock.s_blocks_count;
    sblock->s_blocks_per_group = superblock.s_blocks_per_group;
    sblock->s_blocksize = bsize;

    sblock->s_blocksize_bits = (10+superblock.s_log_block_size);
    sblock->s_first_data_block = superblock.s_first_data_block;
    sblock->s_first_ino = superblock.s_first_ino;
    sblock->s_free_blocks_count = superblock.s_free_blocks_count;
    sblock->s_free_inodes_count = superblock.s_free_inodes_count;
    sblock->s_inode_size = superblock.s_inode_size;
    sblock->s_inodes_count = superblock.s_inodes_count;
    sblock->s_inodes_per_group = superblock.s_inodes_per_group;
    sblock->s_magic = superblock.s_magic;
    sblock->s_maxbytes = superblock.s_inode_size * bsize;
    sblock->s_minor_rev_level = superblock.s_minor_rev_level;
    sblock->s_op = &s_op;
    sblock->s_rev_level = superblock.s_rev_level;
    sblock->s_type = fst;

    struct inode *r = malloc(sizeof(struct inode));
    r->i_ino = EXT2_ROOT_INO;
    r->i_sb = sblock;
    fill_inode(r, r_inode);


    sblock->s_root = malloc(sizeof(struct dentry));
    sblock->s_root->d_flags = r->i_flags;
    sblock->s_root->d_inode = r;
    sblock->s_root->d_parent = sblock->s_root;
    sblock->s_root->d_name = ".\0";
    sblock->s_root->d_sb = sblock;

    return sblock;
}

void read_inode_function(struct inode *iNode){
    struct ext2_inode i_node;

    if(bsize == BASE_OFFSET){
        lseek(myfs.file_descriptor, BLOCK_OFFSET(grp_dsc_lst->bg_inode_table, bsize)+(iNode->i_ino-1)*sizeof(struct ext2_inode), SEEK_SET);
    }else{
        lseek(myfs.file_descriptor, grp_dsc_lst->bg_inode_table*bsize+(iNode->i_ino-1)*sizeof(struct ext2_inode), SEEK_SET);
    }
    read(myfs.file_descriptor, &i_node , sizeof(struct ext2_inode));

    iNode->i_sb = current_sb;
    fill_inode(iNode, i_node);

}

int statfs_function(struct super_block *sblock, struct kstatfs *kstat_fs){
    kstat_fs->f_bfree = sblock->s_free_blocks_count;
    kstat_fs->f_blocks = sblock->s_blocks_count;
    kstat_fs->f_bsize = sblock->s_blocksize;
    kstat_fs->f_finodes = sblock->s_free_inodes_count;
    kstat_fs->f_inode_size = sblock->s_inode_size;
    kstat_fs->f_inodes = sblock->s_inodes_count;
    kstat_fs->f_magic = sblock->s_magic;
    kstat_fs->f_minor_rev_level = sblock->s_minor_rev_level;
    kstat_fs->f_rev_level = sblock->s_rev_level;
    kstat_fs->name = fs_name;
    kstat_fs->f_namelen = strlen(fs_name);

    return 0;
}

int dsize(struct ext2_dir_entry *dent) {
    int dir_size = sizeof(struct ext2_dir_entry);
    dir_size += dent->name_len * sizeof(char);
    dir_size += 4 - ((dir_size)%4);
    return dir_size;
}

uint get_inode_no(char *path, uint inode) {

    int p1=0, p2=0;
    if (path[0] == '/' && path[1] != '\0') {
        p1 = 1;
    }
    for (p2 = p1; path[p2] != '\0' && path[p2] != '/'; ++p2);
    if (p2 == p1) {
        return inode;
    }
    if(!inode){
        return 0;
    }
    struct ext2_inode my_inode;
    my_inode = read_ext2_inode(inode);
    for (int i = 0; i < 12; ++i) {
        uint size_cnt = 0;
        while (size_cnt < current_sb->s_blocksize) {
            struct ext2_dir_entry dir_entry;
            lseek(myfs.file_descriptor, BLOCK_OFFSET(my_inode.i_block[i],current_sb->s_blocksize)+size_cnt, SEEK_SET);
            read(myfs.file_descriptor, &dir_entry, sizeof(struct ext2_dir_entry));
            char *dir_name = malloc((dir_entry.name_len+1)*sizeof(char));
            read(myfs.file_descriptor, dir_name, dir_entry.name_len*sizeof(char));
            dir_name[dir_entry.name_len] = '\0';

            if (!strncmp(dir_name, &path[p1], p2-p1)) {

                free(dir_name);
                return get_inode_no(&path[p2], dir_entry.inode);
            }
            free(dir_name);

            uint last_size = dsize(&dir_entry);
            if (last_size != dir_entry.rec_len) {
                size_cnt = -1;
                break;
            }
            else {
                size_cnt += last_size;
            }
        }
        if (size_cnt == -1) {
            break;
        }
    }
    return 0;
}
struct dentry *lookup_function(struct inode *i_node, struct dentry *d_entry){
    struct inode i;
    struct  dentry *d = malloc(sizeof(struct dentry));
    d_entry->d_sb = current_sb;
    d_entry->d_inode = i_node;
    d_entry->d_flags = i_node->i_flags;

    if(strcmp(d_entry->d_name,".") == 0)
        return d_entry;

    i.i_ino = get_inode_no("..", i_node->i_ino);
    i.i_sb = current_sb;
    current_sb->s_op->read_inode(&i);

    char *temp = malloc(sizeof(char)*strlen(d_entry->d_name));
    int path_length = strlen(d_entry->d_name);

    if(d_entry->d_name[path_length-1] == '/'){
        path_length--;
    }
    int index=0;
    for(int j=path_length-1; j>=0; j--){
        if(d_entry->d_name[j]=='/'){
            break;
        }
        temp[index] = d_entry->d_name[j];
        index++;
    }

    char dirname[index+1];
    for(int k=0; k<index; k++){
        dirname[k]=temp[index-k-1];
    }
    dirname[index]='\0';
    int n = path_length-index-1;
    if(n<=0 || (n==1 && d_entry->d_name[0] == '/')){
        d->d_name = ".";
    } else{
        d_entry->d_name[n]='\0';
        d->d_name = d_entry->d_name;
    }
    d_entry->d_parent = i.i_op->lookup(&i, d);

    d_entry->d_name = dirname;
    return d_entry;
}

int readlink_function(struct dentry *d_entry, char *a, int b){

}

int readdir_function(struct inode *inode, filldir_t call_back){
    uint inode_no = inode->i_ino;
    struct  ext2_inode my_inode = read_ext2_inode(inode_no);
    int dir_cnt=0;
    for(int index=0; index<12; index++){
        uint size = 0;
        while (size < current_sb->s_blocksize) {
            struct ext2_dir_entry dir_entry;
            char *name = malloc((dir_entry.name_len+1)* sizeof(char));
            lseek(myfs.file_descriptor, BLOCK_OFFSET(my_inode.i_block[index], bsize) + size,
                  SEEK_SET);
            read(myfs.file_descriptor, &dir_entry, sizeof(struct ext2_dir_entry));
            read(myfs.file_descriptor, name, dir_entry.name_len*sizeof(char));
            if(strcmp(name, "")!=0){
                if(dir_entry.inode !=0){
                    if(dir_entry.file_type != EXT2_FT_SYMLINK){
                        dir_cnt++;
                        call_back(name, dir_entry.name_len ,dir_entry.inode);
                    }
                }
            }
            uint new_size = dsize(&dir_entry);

            if (new_size != dir_entry.rec_len) {
                break;
            }
            else {
                size += new_size;
            }
        }
    }
    return dir_cnt;
}

int getattr_function(struct dentry *d_entry, struct kstat *k_stat){
    k_stat->size = d_entry->d_inode->i_size;
    k_stat->atime = d_entry->d_inode->i_atime;
    k_stat->blksize = d_entry->d_inode->i_sb->s_blocksize;
    k_stat->blocks = d_entry->d_inode->i_blocks;
    k_stat->ctime = d_entry->d_inode->i_ctime;
    k_stat->gid = d_entry->d_inode->i_gid;
    k_stat->ino = d_entry->d_inode->i_ino;
    k_stat->mode = d_entry->d_inode->i_mode;
    k_stat->mtime = d_entry->d_inode->i_mtime;
    k_stat->nlink = d_entry->d_inode->i_nlink;
    k_stat->uid = d_entry->d_inode->i_uid;

    return 0;
}

loffset_t llseek_function(struct file *my_file, loffset_t offset, int whence){
}

ssize_t read_function(struct file *my_file, char *my_buffer, size_t my_size, loffset_t *offset){
}

int open_function(struct inode *i_node, struct file *new_file){
}

int release_function(struct inode *i_node, struct file *new_file){
}

struct file_system_type *initialize_ext2(const char *image_path) {

  s_op = (struct super_operations){
          .read_inode = read_inode_function,
          .statfs = statfs_function,
  };

  i_op = (struct inode_operations) {
          .getattr = getattr_function,
          .lookup = lookup_function,
          .readdir = readdir_function,
          .readlink = readlink_function,
  };

  f_op = (struct file_operations){
      .llseek = llseek_function,
      .open = open_function,
      .read = read_function,
      .release = release_function,
  };

  myfs.name = fs_name;
  myfs.file_descriptor = open(image_path, O_RDONLY);
  myfs.get_superblock = get_superblock;

   return &myfs;
}
