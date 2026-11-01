#include <utils/file.h>

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
