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
#include "spdlog/sinks/file/daily.h"
#include "spdlog/details/os.h"

namespace spdlog
{
namespace sinks
{
namespace utility
{
std::chrono::system_clock::time_point next_rotation_tp(int hour, int min)
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        time_t tnow = std::chrono::system_clock::to_time_t(now);
        tm date = spdlog::details::os::localtime(tnow);
        date.tm_hour = hour;
        date.tm_min = min;
        date.tm_sec = 0;
        auto rotation_time = std::chrono::system_clock::from_time_t(std::mktime(&date));
        if (rotation_time > now)
            return rotation_time;
        else
            return system_clock::time_point(rotation_time + hours(24));
    }

//Create filename for the form basename.YYYY-MM-DD.extension
std::string calc_filename(const std::string& basename, const std::string& extension)
{
    std::tm tm = spdlog::details::os::localtime();
    fmt::MemoryWriter w;
    w.write("{}_{:04d}-{:02d}-{:02d}_{:02d}-{:02d}.{}", basename, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, extension);
    return w.str();
}
} // ns details
} // ns sinks
} // ns spdlog
