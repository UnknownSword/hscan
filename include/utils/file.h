#ifndef __UTILS_FILE_H
#define __UTILS_FILE_H

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

off_t file_size(int);
off_t file_size_beg(int);

#endif
