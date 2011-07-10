#ifndef _SW_FS_H_
#define _SW_FS_H_

#include "buffer.h"
#include "types.h"

#include <string>


namespace sw
{

const mask_t default_file_mask = 0644;
const mask_t default_dir_mask = 0755;

/**
 * open a file
 * [in] fname: file path
 * [in] mode: Y_READ, Y_WRITE, Y_RDWR
 */
handle_t open(const std::string& fname, mode_t mode, mask_t mask = default_file_mask);

/**
 * write data to a file
 * [in] fd: file descriptor
 * [in] buff: data
 */
size_t write(handle_t fd, const buffer_t& buff);

/**
 * write data to a file
 * [in] fd: file descriptor
 * [in] buff: data
 */
size_t write(handle_t fd, const char* buff, int size);


/**
 *
 * read data from a file
 * [in] fd: file descriptor
 * [out] buff: data
 */
size_t read(handle_t fd, buffer_t& buff);


/**
 *
 * read data from a file
 * [in] fd: file descriptor
 * [out] buff: data
 */
size_t read(handle_t fd, char* buff, int& size);

/**
 * close a file
 * [in] fd: file descriptor
 */
void close(handle_t fd);

/**
 * create a directory
 * [in] path: directory path
 */
void mkdir(const std::string& path, mask_t mask = default_dir_mask);
}

#endif
