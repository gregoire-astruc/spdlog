/*************************************************************************/
/* spdlog - an extremely fast and easy to use c++11 logging library.     */
/* Copyright (c) 2014 Gabi Melman.                                       */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "spdlog/details/file_helper.h"

#include <thread>

#include "spdlog/exception.h"
#include "spdlog/details/os.h"

namespace spdlog
{
namespace details
{

void file_helper::open(const std::string& fname, bool truncate /* = false */)
{

    close();
    const char* mode = truncate ? "wb" : "ab";
    _filename = fname;
    for (int tries = 0; tries < open_tries; ++tries)
    {
        if (!os::fopen_s(&_fd, fname, mode))
            return;

        std::this_thread::sleep_for(std::chrono::milliseconds(open_interval));
    }

    throw spdlog_ex("Failed opening file " + fname + " for writing");
}

void file_helper::reopen(bool truncate)
{
    if (_filename.empty())
        throw spdlog_ex("Failed re opening file - was not opened before");
    open(_filename, truncate);

}

void file_helper::flush() {
    std::fflush(_fd);
}

void file_helper::close()
{
    if (_fd)
    {
        std::fclose(_fd);
        _fd = nullptr;
    }
}

void file_helper::write(const log_msg& msg)
{

    size_t size = msg.formatted.size();
    auto data = msg.formatted.data();
    if (std::fwrite(data, 1, size, _fd) != size)
        throw spdlog_ex("Failed writing to file " + _filename);

    if (_force_flush)
        std::fflush(_fd);

}

bool file_helper::file_exists(const std::string& name)
{
    FILE* file;
    if (!os::fopen_s(&file, name.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}
} // ns details
} // ns spdlog
