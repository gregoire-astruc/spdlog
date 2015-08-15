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

#pragma once
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/file/fwd.h"
#include "spdlog/details/file_helper.h"

namespace spdlog
{
namespace sinks
{
/*
* Rotating file sink based on size
*/
template<class Mutex>
class rotating_file_sink : public base_sink < Mutex >
{
public:
    rotating_file_sink(const std::string &base_filename, const std::string &extension,
                       std::size_t max_size, std::size_t max_files,
                       bool force_flush = false) :
        _base_filename(base_filename),
        _extension(extension),
        _max_size(max_size),
        _max_files(max_files),
        _current_size(0),
        _file_helper(force_flush)
    {
        _file_helper.open(calc_filename(_base_filename, 0, _extension));
    }

    void flush() override
    {
        _file_helper.flush();
    }

protected:
    void _sink_it(const details::log_msg& msg) override
    {
        _current_size += msg.formatted.size();
        if (_current_size > _max_size)
        {
            _rotate();
            _current_size = msg.formatted.size();
        }
        _file_helper.write(msg);
    }

private:
    static std::string calc_filename(const std::string& filename, std::size_t index, const std::string& extension)
    {
        fmt::MemoryWriter w;
        if (index)
            w.write("{}.{}.{}", filename, index, extension);
        else
            w.write("{}.{}", filename, extension);
        return w.str();
    }

    // Rotate files:
    // log.txt -> log.1.txt
    // log.1.txt -> log2.txt
    // log.2.txt -> log3.txt
    // log.3.txt -> delete

    void _rotate()
    {
        _file_helper.close();
        for (auto i = _max_files; i > 0; --i)
        {
            std::string src = calc_filename(_base_filename, i - 1, _extension);
            std::string target = calc_filename(_base_filename, i, _extension);

            if (details::file_helper::file_exists(target))
            {
                if (std::remove(target.c_str()) != 0)
                {
                    throw spdlog_ex("rotating_file_sink: failed removing " + target);
                }
            }
            if (details::file_helper::file_exists(src) && std::rename(src.c_str(), target.c_str()))
            {
                throw spdlog_ex("rotating_file_sink: failed renaming " + src + " to " + target);
            }
        }
        _file_helper.reopen(true);
    }
    std::string _base_filename;
    std::string _extension;
    std::size_t _max_size;
    std::size_t _max_files;
    std::size_t _current_size;
    details::file_helper _file_helper;
};
} // ns sinks
} // ns spdlog
