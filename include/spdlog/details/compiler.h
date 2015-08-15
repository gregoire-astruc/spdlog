#pragma once
//visual studio does not support noexcept yet
#ifndef _MSC_VER
#define SPDLOG_NOEXCEPT noexcept
#else
#if _MSC_VER < 1900
#define SPDLOG_NOEXCEPT throw()
#endif
#endif
