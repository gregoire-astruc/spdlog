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
namespace spdlog
{
//Log level enum
namespace level
{
enum class level_enum
{
    trace = 0,
    debug = 1,
    info = 2,
    notice = 3,
    warn = 4,
    err = 5,
    critical = 6,
    alert = 7,
    emerg = 8,
    off = 9
};

static const char* level_names[] { "trace", "debug", "info", "notice", "warning", "error", "critical", "alert", "emerg", "off"};

static const char* short_level_names[] { "T", "D", "I", "N", "W", "E", "C", "A", "M", "O"};

inline const char* to_str(spdlog::level::level_enum l)
{
    return level_names[static_cast<int>(l)];
}

inline const char* to_short_str(spdlog::level::level_enum l)
{
    return short_level_names[static_cast<int>(l)];
}
} //level
} //spdlog
