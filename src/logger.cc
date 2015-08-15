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
#include "spdlog/logger.h"

#include <memory>

#include "spdlog/details/line_logger.h"
#include "spdlog/formatters/pattern_formatter.h"
#include "spdlog/sinks/sink.h"

// ctor with sinks as init list
spdlog::logger::logger(const std::string& logger_name, sinks_init_list sinks_list) :
    logger(logger_name, sinks_list.begin(), sinks_list.end()) {}


// ctor with single sink
spdlog::logger::logger(const std::string& logger_name, spdlog::sink_ptr single_sink) :
    logger(logger_name, {
    single_sink
}) {}


spdlog::logger::~logger() = default;


void spdlog::logger::set_formatter(spdlog::formatter_ptr msg_formatter)
{
    _set_formatter(msg_formatter);
}

void spdlog::logger::set_pattern(const std::string& pattern)
{
    _set_pattern(pattern);
}

//
// log only if given level>=logger's log level
//

spdlog::details::line_logger spdlog::logger::_log_if_enabled(level::level_enum lvl)
{
    return details::line_logger(this, lvl, should_log(lvl));
}


//
// logger.info() << ".." call  style
//
spdlog::details::line_logger spdlog::logger::trace()
{
    return _log_if_enabled(level::level_enum::trace);
}

spdlog::details::line_logger spdlog::logger::debug()
{
    return _log_if_enabled(level::level_enum::debug);
}

spdlog::details::line_logger spdlog::logger::info()
{
    return _log_if_enabled(level::level_enum::info);
}

spdlog::details::line_logger spdlog::logger::notice()
{
    return _log_if_enabled(level::level_enum::notice);
}

spdlog::details::line_logger spdlog::logger::warn()
{
    return _log_if_enabled(level::level_enum::warn);
}

spdlog::details::line_logger spdlog::logger::error()
{
    return _log_if_enabled(level::level_enum::err);
}

spdlog::details::line_logger spdlog::logger::critical()
{
    return _log_if_enabled(level::level_enum::critical);
}

spdlog::details::line_logger spdlog::logger::alert()
{
    return _log_if_enabled(level::level_enum::alert);
}

spdlog::details::line_logger spdlog::logger::emerg()
{
    return _log_if_enabled(level::level_enum::emerg);
}

//
// name and level
//
const std::string& spdlog::logger::name() const
{
    return _name;
}

void spdlog::logger::set_level(spdlog::level::level_enum log_level)
{
    _level.store(static_cast<int>(log_level));
}

spdlog::level::level_enum spdlog::logger::level() const
{
    return static_cast<spdlog::level::level_enum>(_level.load(std::memory_order_relaxed));
}

bool spdlog::logger::should_log(spdlog::level::level_enum msg_level) const
{
    return msg_level >= level();
}

//
// protected virtual called at end of each user log call (if enabled) by the line_logger
//
void spdlog::logger::_log_msg(details::log_msg& msg)
{
    _formatter->format(msg);
    for (auto &sink : _sinks)
        sink->log(msg);
}

void spdlog::logger::_set_pattern(const std::string& pattern)
{
    _formatter = std::make_shared<pattern_formatter>(pattern);
}

void spdlog::logger::_set_formatter(formatter_ptr msg_formatter)
{
    _formatter = msg_formatter;
}

void spdlog::logger::flush() {
    for (auto& sink : _sinks)
        sink->flush();
}
