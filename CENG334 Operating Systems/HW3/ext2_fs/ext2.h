#ifndef _EXT2_H_
#define _EXT2_H_

#include "ext2_fs.h"
#include "vfs/fs.h"
#include <stdio.h>

struct file_system_type myfs;

struct file_system_type *initialize_ext2(const char *image_path);

struct ext2_group_desc* grp_dsc_lst;

unsigned int grp_count;

int dsize(struct ext2_dir_entry *dirent);

unsigned int get_inode_no(char *target_name, unsigned int current_inode);

struct ext2_inode read_ext2_inode(unsigned int iID);

#endif
