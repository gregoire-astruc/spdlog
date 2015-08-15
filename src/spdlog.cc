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
#include "spdlog/spdlog.h"

//
// Global registry functions
//
#include "spdlog/details/registry.h"
#include "spdlog/sinks/file_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/syslog_sink.h"
#include "spdlog/details/config.h"

namespace spdlog
{
void register_logger(std::shared_ptr<logger> logger)
{
    return details::registry::instance().register_logger(logger);
}

std::shared_ptr<logger> get(const std::string& name)
{
    return details::registry::instance().get(name);
}

void drop(const std::string &name)
{
    details::registry::instance().drop(name);
}

// Create multi/single threaded rotating file logger
std::shared_ptr<logger> rotating_logger_mt(const std::string& logger_name, const std::string& filename, size_t max_file_size, size_t max_files, bool force_flush)
{
    return create<sinks::rotating_file_sink_mt>(logger_name, filename, "txt", max_file_size, max_files, force_flush);
}

std::shared_ptr<logger> rotating_logger_st(const std::string& logger_name, const std::string& filename, size_t max_file_size, size_t max_files, bool force_flush)
{
    return create<sinks::rotating_file_sink_st>(logger_name, filename, "txt", max_file_size, max_files, force_flush);
}

// Create file logger which creates new file at midnight):
std::shared_ptr<logger> daily_logger_mt(const std::string& logger_name, const std::string& filename, int hour, int minute, bool force_flush)
{
    return create<sinks::daily_file_sink_mt>(logger_name, filename, "txt", hour, minute, force_flush);
}
std::shared_ptr<logger> daily_logger_st(const std::string& logger_name, const std::string& filename, int hour, int minute, bool force_flush)
{
    return create<sinks::daily_file_sink_st>(logger_name, filename, "txt", hour, minute, force_flush);
}


// Create stdout/stderr loggers
std::shared_ptr<logger> stdout_logger_mt(const std::string& logger_name)
{
    return details::registry::instance().create(logger_name, sinks::stdout_sink_mt::instance());
}

std::shared_ptr<logger> stdout_logger_st(const std::string& logger_name)
{
    return details::registry::instance().create(logger_name, sinks::stdout_sink_st::instance());
}

std::shared_ptr<logger> stderr_logger_mt(const std::string& logger_name)
{
    return details::registry::instance().create(logger_name, sinks::stderr_sink_mt::instance());
}

std::shared_ptr<logger> stderr_logger_st(const std::string& logger_name)
{
    return details::registry::instance().create(logger_name, sinks::stderr_sink_st::instance());
}

#ifdef __linux__
// Create syslog logger
std::shared_ptr<logger> syslog_logger(const std::string& logger_name, const std::string& syslog_ident, int syslog_option)
{
    return create<sinks::syslog_sink>(logger_name, syslog_ident, syslog_option);
}
#endif


//Create logger with multiple sinks

std::shared_ptr<logger> create(const std::string& logger_name, sinks_init_list sinks)
{
    return details::registry::instance().create(logger_name, sinks);
}

void set_formatter(formatter_ptr f)
{
    details::registry::instance().formatter(f);
}

void set_pattern(const std::string& format_string)
{
    return details::registry::instance().set_pattern(format_string);
}

void set_level(level::level_enum log_level)
{
    return details::registry::instance().set_level(log_level);
}


void set_async_mode(const std::function<void()>& worker_warmup_cb, const std::chrono::milliseconds& flush_interval_ms)
{
    details::registry::instance().set_async_mode(worker_warmup_cb, flush_interval_ms);
}

void set_sync_mode()
{
    details::registry::instance().set_sync_mode();
}

void drop_all()
{
    details::registry::instance().drop_all();
}
}

