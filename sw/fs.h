#ifndef _SW_FS_H_
#define _SW_FS_H_


#include "buffer.h"
#include "types.h"

#include "log.h"

#include <string>

namespace sw
{
    class File
    {
        private:
            handle_t m_handle;
            std::string m_path;
            static Logger* m_log;

        public:

            static const mask_t default_file_mask = 0644;
            static const mask_t default_dir_mask = 0755;

            File();
            File(const std::string& path, mode_t mode,  mask_t mask = default_file_mask);
            ~File();

            /**
             * open a file
             * [in] fname: file path
             * [in] mode: Y_READ, Y_WRITE, Y_RDWR
             */

            void open(const std::string& path, mode_t mode, mask_t mask = default_file_mask);
            /**
             * write data to a file
             * [in] fd: file descriptor
             * [in] buff: data
             */

            size_t write(const char* buffer, const size_t& size);
            /**
             * write data to a file
             * [in] fd: file descriptor
             * [in] buff: data
             */

            size_t write(const buffer_t& buff);
            /**
             *
             * read data from a file
             * [in] fd: file descriptor
             * [out] buff: data
             */

            size_t read(buffer_t& buff);
            /**
             *
             * read data from a file
             * [in] fd: file descriptor
             * [out] buff: data
             */

            size_t read(char* buf, size_t& size);

            bool isDirectory();
            bool isFile();

            /**
             * create a directory
             * [in] path: directory path
             */

            void mkdir(mask_t mask = default_dir_mask);
    };

}

#endif
