// ReSharper disable CppUE4CodingStandardNamingViolationWarning
#ifndef RD_CPP_TO_STRING_H
#define RD_CPP_TO_STRING_H

#include <string>
#include <thread>
#include <sstream>
#include <atomic>
#include <future>
#include <locale>

#include "ww898/utf_converters.hpp"

#include <thirdparty.hpp>

namespace rd
{
namespace detail
{
using std::to_string;

inline std::string to_string(std::string const& val)
{
	return val;
}

inline std::string to_string(const char* val)
{
	return val;
}

inline std::string to_string(std::wstring const& val)
{
	return ww898::utf::conv<std::string::value_type>(val);
}

inline std::string to_string(std::thread::id const& id)
{
	std::ostringstream ss;
	ss << id;
	return ss.str();
}

inline std::string to_string(std::exception const& e)
{
	return std::string(e.what());
}

inline std::string to_string(std::future_status const& status)
{
	switch (status)
	{
		case std::future_status::ready:
			return "ready";
		case std::future_status::timeout:
			return "timeout";
		case std::future_status::deferred:
			return "deferred";
		default:
			return "unknown";
	}
}

template <typename Rep, typename Period>
inline std::string to_string(std::chrono::duration<Rep, Period> const& time)
{
	return std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(time).count()) + "ms";
}

template <typename T>
inline std::string to_string(T const* val)
{
	return val ? to_string(*val) : "nullptr";
}

template <typename T>
inline std::string to_string(std::atomic<T> const& value)
{
	return to_string(value.load());
}

template <typename T>
inline std::string to_string(optional<T> const& val)
{
	if (val.has_value())
	{
		return to_string(*val);
	}
	else
	{
		return "nullopt";
	}
}

template <typename F, typename S>
inline std::string to_string(const std::pair<F, S> p)
{
	return "(" + to_string(p.first) + ", " + to_string(p.second) + ")";
}

template <template <class, class> class C, typename T, typename A>
std::string to_string(C<T, A> const& v)
{
	std::string res = "[";
	for (const auto& Tool : v)
	{
		res += to_string(Tool);
		res += ",";
	}
	res += "]";
	return res;
}

template <class T>
std::string as_string(T const& t)
{
	return to_string(t);
}

using std::to_wstring;

inline std::wstring to_wstring(std::string const& s)
{
	return ww898::utf::conv<std::wstring::value_type>(s);
}

template <class T>
std::wstring as_wstring(T const& t)
{
	return to_wstring(t);
}
}	 // namespace detail

template <typename T>
std::string to_string(T const& val)
{
	return detail::as_string(val);
}

template <typename T>
std::wstring to_wstring(T const& val)
{
	return detail::as_wstring(val);
}
}	 // namespace rd

#endif	  // RD_CPP_TO_STRING_H
