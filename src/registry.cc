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
#include "spdlog/details/registry.h"

#include "spdlog/level.h"
#include "spdlog/async_logger.h"

namespace spdlog
{
namespace details
{
void registry::register_logger(std::shared_ptr<logger> logger)
{
    std::lock_guard<mutex_t> lock(_mutex);
    register_logger_impl(logger);
}


std::shared_ptr<logger> registry::get(const std::string& logger_name)
{
    std::lock_guard<mutex_t> lock(_mutex);
    auto found = _loggers.find(logger_name);
    return found == _loggers.end() ? nullptr : found->second;
}

void registry::drop(const std::string& logger_name)
{
    std::lock_guard<mutex_t> lock(_mutex);
    _loggers.erase(logger_name);
}

void registry::drop_all()
{
    std::lock_guard<mutex_t> lock(_mutex);
    _loggers.clear();
}
std::shared_ptr<logger> registry::create(const std::string& logger_name, sinks_init_list sinks)
{
    return create(logger_name, sinks.begin(), sinks.end());
}

std::shared_ptr<logger> registry::create(const std::string& logger_name, sink_ptr sink)
{
    return create(logger_name, { sink });
}


void registry::formatter(formatter_ptr f)
{
    std::lock_guard<mutex_t> lock(_mutex);
    _formatter = f;
    for (auto& l : _loggers)
        l.second->set_formatter(_formatter);
}

void registry::set_pattern(const std::string& pattern)
{
    std::lock_guard<mutex_t> lock(_mutex);
    _formatter = std::make_shared<pattern_formatter>(pattern);
    for (auto& l : _loggers)
        l.second->set_formatter(_formatter);
}

void registry::set_level(level::level_enum log_level)
{
    std::lock_guard<mutex_t> lock(_mutex);
    for (auto& l : _loggers)
        l.second->set_level(log_level);
    _level = log_level;
}

void registry::set_async_mode(const std::function<void()>& worker_warmup_cb, const std::chrono::milliseconds& flush_interval_ms)
{
    std::lock_guard<mutex_t> lock(_mutex);
    _async_mode = true;
    _worker_warmup_cb = worker_warmup_cb;
    _flush_interval_ms = flush_interval_ms;
}

void registry::set_sync_mode()
{
    std::lock_guard<mutex_t> lock(_mutex);
    _async_mode = false;
}

registry& registry::instance()
{
    static registry s_instance;
    return s_instance;
}

void registry::register_logger_impl(std::shared_ptr<logger> logger)
{
    auto logger_name = logger->name();
    if (_loggers.find(logger_name) != std::end(_loggers))
        throw spdlog_ex("logger with name " + logger_name + " already exists");
    _loggers[logger->name()] = logger;
}

registry::registry():
    _level{level::level_enum::info},
    _async_mode{false},
    _worker_warmup_cb{nullptr}
{
    // nothing much.
}
} // ns details
} // ns spdlog
