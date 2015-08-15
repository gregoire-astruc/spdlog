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
#include "spdlog/exception.h"

namespace spdlog
{
namespace sinks
{
namespace utility
{
    std::chrono::system_clock::time_point next_rotation_tp(int hour, int min);

    //Create filename for the form basename.YYYY-MM-DD.extension
    std::string calc_filename(const std::string& basename, const std::string& extension);
}
/*
* Rotating file sink based on date. rotates at midnight
*/
template<class Mutex>
class daily_file_sink :public base_sink < Mutex >
{
public:
    //create daily file sink which rotates on given time
    daily_file_sink(
        const std::string& base_filename,
        const std::string& extension,
        int rotation_hour,
        int rotation_minute,
        bool force_flush = false) : _base_filename(base_filename),
        _extension(extension),
        _rotation_h(rotation_hour),
        _rotation_m(rotation_minute),
        _file_helper(force_flush)
    {
        if (rotation_hour < 0 || rotation_hour > 23 || rotation_minute < 0 || rotation_minute > 59)
            throw spdlog_ex("daily_file_sink: Invalid rotation time in ctor");
        _rotation_tp = utility::next_rotation_tp(_rotation_h, _rotation_m);
        _file_helper.open(utility::calc_filename(_base_filename, _extension));
    }

    void flush() override
    {
        _file_helper.flush();
    }

protected:
    void _sink_it(const details::log_msg& msg) override
    {
        if (std::chrono::system_clock::now() >= _rotation_tp)
        {
            _file_helper.open(utility::calc_filename(_base_filename, _extension));
            _rotation_tp = utility::next_rotation_tp(_rotation_h, _rotation_m);
        }
        _file_helper.write(msg);
    }

private:
    std::string _base_filename;
    std::string _extension;
    int _rotation_h;
    int _rotation_m;
    std::chrono::system_clock::time_point _rotation_tp;
    details::file_helper _file_helper;
};
} // ns sinks
} // ns spdlog
