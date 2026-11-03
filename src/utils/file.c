#include <utils/file.h>

int file_type(const char *path)
{
    struct stat st;

    if (stat(path, &st) == -1)
        return FILETYPE_ERROR;

    if (S_ISREG(st.st_mode))
        return FILETYPE_FILE;

    if (S_ISDIR(st.st_mode))
        return FILETYPE_DIRECTORY;

    return FILETYPE_ERROR;
}

off_t file_size(int fd){
	off_t first = lseek(fd, 0, SEEK_CUR); // get position
	off_t res = lseek(fd, 0, SEEK_END);	  // get end position ( size )
	lseek(fd, first, SEEK_SET);			  // set position back
	return res - 1;
}
off_t file_size_beg(int fd){
	off_t res = lseek(fd, 0, SEEK_END);	  // get end position ( size )
	lseek(fd, 1, SEEK_SET);			  // set position back
	return res - 1;
}
