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
* Trivial file sink with single file as target
*/
template<class Mutex>
class simple_file_sink : public base_sink < Mutex >
{
public:
    explicit simple_file_sink(const std::string &filename,
                              bool force_flush = false) :
        _file_helper(force_flush)
    {
        _file_helper.open(filename);
    }
    void flush() override
    {
        _file_helper.flush();
    }

protected:
    void _sink_it(const details::log_msg& msg) override
    {
        _file_helper.write(msg);
    }
private:
    details::file_helper _file_helper;
};
} // ns sinks
} // ns spdlog
