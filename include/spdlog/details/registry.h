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
// Loggers registy of unique name->logger pointer
// An attempt to create a logger with an alreasy existing name will be ignored
// If user requests a non existing logger, nullptr will be returned
// This class is thread safe
#include <mutex>
#include <unordered_map>

#include "spdlog/fwd.h"
#include "spdlog/level.h"

namespace spdlog
{
namespace details
{

class registry
{
#ifdef SPDLOG_NO_REGISTRY_MUTEX
    using mutex_t = null_mutex;
#else
    using mutex_t = std::mutex;
#endif
public:

    void register_logger(std::shared_ptr<logger> logger);

    std::shared_ptr<logger> get(const std::string& logger_name);

    template<class It>
    std::shared_ptr<logger> create(const std::string& logger_name, const It& sinks_begin, const It& sinks_end);

    void drop(const std::string& logger_name);

    void drop_all();

    std::shared_ptr<logger> create(const std::string& logger_name, sinks_init_list sinks);

    std::shared_ptr<logger> create(const std::string& logger_name, sink_ptr sink);


    void formatter(formatter_ptr f);

    void set_pattern(const std::string& pattern);

    void set_level(level::level_enum log_level);

    void set_async_mode( const std::function<void()>& worker_warmup_cb, const std::chrono::milliseconds& flush_interval_ms);

    void set_sync_mode();

    static registry& instance();

private:
    void register_logger_impl(std::shared_ptr<logger> logger);
    registry();
    registry(const registry&) = delete;
    const registry& operator=(const registry&) = delete;
    mutex_t _mutex;
    std::unordered_map <std::string, std::shared_ptr<logger>> _loggers;
    formatter_ptr _formatter;
    level::level_enum _level;
    bool _async_mode;
    std::function<void()> _worker_warmup_cb;
    std::chrono::milliseconds _flush_interval_ms;
};
} // ns details
} // ns spdlog

#include "./registry-inl.h"
