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
#include "spdlog/logger.h"

namespace spdlog
{
namespace details
{
template<typename It>
std::shared_ptr<logger> registry::create(const std::string& logger_name, const It& sinks_begin, const It& sinks_end)
{

    std::shared_ptr<logger> new_logger;

    std::lock_guard<mutex_t> lock(_mutex);


    if (_async_mode)
        new_logger = std::make_shared<async_logger>(logger_name, sinks_begin, sinks_end, _worker_warmup_cb, _flush_interval_ms);
    else
        new_logger = std::make_shared<logger>(logger_name, sinks_begin, sinks_end);

    if (_formatter)
        new_logger->set_formatter(_formatter);

    new_logger->set_level(_level);
    register_logger_impl(new_logger);
    return new_logger;
}
} // ns details
} // ns spdlog

