// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_STRING_FUNCS_H_
#define _CTLE_STRING_FUNCS_H_

/// @file string_funcs.h
/// @brief Functions for parsing and converting strings.

#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>

namespace ctle
{

/// @brief A span of characters, with start and end pointers.
/// @tparam _Ty The type of the characters in the span, char or wchar_t.
template<class _Ty> struct string_span
{
	string_span() = default;
	string_span(const _Ty* _start, const _Ty* _end) : start(_start), end(_end) {}

	const _Ty* start = {}; // first char of span
	const _Ty* end = {}; // the character after the last char of the span

	/// @brief get the length of the span, returns 0 if the span is invalid
	size_t length() const noexcept { return (end > start) ? end - start : 0; }

	/// @brief make a copy to a basic_string, returns an empty string if the span is invalid (end<=start)
	operator std::basic_string<_Ty>() noexcept { return (end > start) ? (std::basic_string<_Ty>(this->start, this->end)) : (std::basic_string<_Ty>()); }
};

/// @brief Parse a type from a string. 
/// @details For regular numbers, from_string assumes base - 10 and from_hex_string assumes base - 16 value. from_hex_string is only defined for unsigned integer values.
/// @param str The string span to parse.
/// @param success Set to false if the conversion fails. Set to true before calling the function.
/// @note The success parameter must be set to true before calling the functions!
/// @return The parsed value.
template<class _Ty> _Ty from_string(const string_span<char>& str, bool& success) noexcept;
template<class _Ty> _Ty from_hex_string(const string_span<char>& str, bool& success) noexcept; ///< @copydoc template<class _Ty> _Ty from_string(const string_span<char>& str, bool& success) noexcept

/// @brief Parse a type from a string. 
/// @details For regular numbers, from_string assumes base - 10 and from_hex_string assumes base - 16 value. from_hex_string is only defined for unsigned integer values.
/// @param start The beginning of the string to parse
/// @param end The end of the string to parse
/// @param success Set to false if the conversion fails. Set to true before calling the function.
/// @note The success parameter must be set to true before calling the functions!
/// @return The parsed value.
template<class _Ty> inline _Ty from_string(const char* start, const char* end, bool& success) noexcept { return from_string<_Ty>(string_span<char>(start, end), success); }
template<class _Ty> inline _Ty from_hex_string(const char* start, const char* end, bool& success) noexcept { return from_hex_string<_Ty>(string_span<char>(start, end), success); } ///< @copydoc template<class _Ty> inline _Ty from_string(const char* start, const char* end, bool& success) noexcept

/// @brief Parse a type from a string. 
/// @details For regular numbers, from_string assumes base - 10 and from_hex_string assumes base - 16 value. from_hex_string is only defined for unsigned integer values.
/// @param str The string to parse.
/// @param success Set to false if the conversion fails. Set to true before calling the function.
/// @note The success parameter must be set to true before calling the functions!
/// @return The parsed value.
template<class _Ty> inline _Ty from_string(const std::string& str, bool& success) noexcept { return from_string<_Ty>(string_span<char>(str.data(), str.data() + str.size()), success); }
template<class _Ty> inline _Ty from_hex_string(const std::string& str, bool& success) noexcept { return from_hex_string<_Ty>(string_span<char>(str.data(), str.data() + str.size()), success); } ///< @copydoc template<class _Ty> inline _Ty from_string(const std::string& str, bool& success) noexcept

/// @brief Parse a type from a string. 
/// @details For regular numbers, from_string assumes base - 10 and from_hex_string assumes base - 16 value. from_hex_string is only defined for unsigned integer values.
/// @param str The string span to parse.
/// @throws std::out_of_range if the result is outside the range of the destination type.
/// @throws std::invalid_argument if the string is not a valid representation of the destination type.
/// @return The parsed value.
template<class _Ty> _Ty from_string(const string_span<char>& str);
template<class _Ty> _Ty from_hex_string(const string_span<char>& str); ///< @copydoc template<class _Ty> _Ty from_string(const string_span<char>& str)

/// @brief Parse a type from a string. 
/// @details For regular numbers, from_string assumes base - 10 and from_hex_string assumes base - 16 value. from_hex_string is only defined for unsigned integer values.
/// @param start The beginning of the string to parse
/// @param end The end of the string to parse
/// @throws std::out_of_range if the result is outside the range of the destination type.
/// @throws std::invalid_argument if the string is not a valid representation of the destination type.
/// @return The parsed value.
template<class _Ty> inline _Ty from_string(const char* start, const char* end) { return from_string<_Ty>(string_span<char>(start, end)); }
template<class _Ty> inline _Ty from_hex_string(const char* start, const char* end) { return from_hex_string<_Ty>(string_span<char>(start, end)); } ///< @copydoc template<class _Ty> inline _Ty from_string(const char* start, const char* end)

/// @brief Parse a type from a string. 
/// @details For regular numbers, from_string assumes base - 10 and from_hex_string assumes base - 16 value. from_hex_string is only defined for unsigned integer values.
/// @param str The string to parse.
/// @throws std::out_of_range if the result is outside the range of the destination type.
/// @throws std::invalid_argument if the string is not a valid representation of the destination type.
/// @return The parsed value.
template<class _Ty> inline _Ty from_string(const std::string& str) { return from_string<_Ty>(string_span<char>(str.data(), str.data() + str.size())); }
template<class _Ty> inline _Ty from_hex_string(const std::string& str) { return from_hex_string<_Ty>(string_span<char>(str.data(), str.data() + str.size())); } ///< @copydoc template<class _Ty> inline _Ty from_string(const std::string& str)

/// @brief Print a type to a string. 
/// @details For regular numbers, to_string prints base - 10 and to_hex_string prints base - 16 value. to_hex_string is only defined for unsigned integer values.
template<class _Ty> std::string to_string(const _Ty& val);
template<class _Ty> std::string to_hex_string(const _Ty& val); ///< @copydoc template<class _Ty> std::string to_string(const _Ty& val)

/// @brief Get number of chars from the beginning of a string span, which DOES match a control list. (opposite of strcspn_t)
/// @tparam _Ty The string character type, char or wchar_t.
/// @param start The beginning of the string span to search.
/// @param end The end of the string span to search.
/// @param control The list of characters to match.
/// @returns The number of charactes in the beginning of the string span, which match the control list.
/// @note No bounds checking is performed, so the control list must be a valid null-terminated string.
/// @note The chars in control are required to be in the lower ascii set (not extended characters)
template<class _Ty> size_t strspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept;
template<class _Ty> inline size_t strspn_t(const string_span<_Ty>& span, const _Ty* control) noexcept { return strspn_t(span.start, span.end, control); } ///< @copydoc template<class _Ty> inline size_t strspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept
template<class _Ty> inline size_t strspn_t(const std::basic_string<_Ty>& str, const _Ty* control) noexcept { return strspn_t(str.data(), str.data() + str.size(), control); }///< @copydoc template<class _Ty> inline size_t strspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept

/// @brief Get number of chars from the beginning of a string span, which DOES NOT match a control list. (opposite of strspn_t)
/// @tparam _Ty The string character type, char or wchar_t.
/// @param start The beginning of the string span to search.
/// @param end The end of the string span to search.
/// @param control The list of characters to not match.
/// @returns The number of charactes in the beginning of the string span, which does not match the control list.
/// @note No bounds checking is performed, so the control list must be a valid null-terminated string.
/// @note The chars in control are required to be in the lower set (not extended characters)
template<class _Ty> size_t strcspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept;
template<class _Ty> inline size_t strcspn_t(const string_span<_Ty>& span, const _Ty* control) noexcept { return strcspn_t(span.start, span.end, control); }
template<class _Ty> inline size_t strcspn_t(const std::basic_string<_Ty>& str, const _Ty* control) noexcept { return strspn_t(str.data(), str.data() + str.size(), control); }

/// @brief Get number of chars from the beginning of a string span, until the last occurance of a character from a control list.
/// @details The function looks in reverse from the end of the span to the start, and stops when hitting the 
/// first character from the control list. However, instead of returning the number of chars in the end of the string, it 
/// returns the number of chars from the beginning of the string.
/// @tparam _Ty The string character type, char or wchar_t.
/// @param start The beginning of the string span to search.
/// @param end The end of the string span to search.
/// @param control The list of characters to match.
/// @returns The number of characters to the last occurance of a character from the control list.
/// @note No bounds checking is performed, so the control list must be a valid null-terminated string.
/// @note The chars in control are required to be in the lower set (not extended characters)
template<class _Ty> size_t strcrspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept;
template<class _Ty> inline size_t strcrspn_t(const string_span<_Ty>& span, const _Ty* control) noexcept { return strcrspn_t(span.start, span.end, control); }///< @copydoc template<class _Ty> inline size_t strcrspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept
template<class _Ty> inline size_t strcrspn_t(const std::basic_string<_Ty>& str, const _Ty* control) noexcept { return strcrspn_t(str.data(), str.data() + str.size(), control); }///< @copydoc template<class _Ty> inline size_t strcrspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept

/// @brief Given a start and end pointer to a string, and a list of deliminators, return the first token.
/// @details the implementation does not modify the input string, and does not hold any internal state.
/// @tparam _Ty The string character type, char or wchar_t.
/// @param start The beginning of the string span to search.
/// @param end The end of the string span to search.
/// @param delims The list of characters to use as deliminators.
/// @note The chars in delims are required to be in the lower set (not extended characters)
template<class _Ty> string_span<_Ty> strtok_t(const _Ty* start, const _Ty* end, const _Ty* delims) noexcept;
template<class _Ty> inline string_span<_Ty> strtok_t(const string_span<_Ty>& span, const _Ty* delims) noexcept { return strtok_t(span.start, span.end, delims); }
template<class _Ty> inline std::basic_string<_Ty> strtok_t(const std::basic_string<_Ty>& str, const _Ty* delims) noexcept { return strtok_t(str.data(), str.data() + str.size(), delims); }

/// @brief Locate first occurrence of character in string
/// @details get count of characters between start and end, until first occurance of a character 
/// @tparam _Ty The string character type, char or wchar_t.
/// @param start The beginning of the string span to search.
/// @param end The end of the string span to search.
/// @param character The character to search for.
/// @returns The number of characters to the first occurance of the character.
template<class _Ty> size_t strchr_t(const _Ty* start, const _Ty* end, _Ty character) noexcept;
template<class _Ty> inline size_t strchr_t(const string_span<_Ty>& span, _Ty character) noexcept { return strchr_t(span.start, span.end, character); } ///< @copydoc template<class _Ty> size_t strchr_t(const _Ty* start, const _Ty* end, _Ty character) noexcept
template<class _Ty> inline size_t strchr_t(const std::basic_string<_Ty>& str, _Ty character) noexcept { return strchr_t(str.data(), str.data() + str.size(), character); } ///< @copydoc template<class _Ty> size_t strchr_t(const _Ty* start, const _Ty* end, _Ty character) noexcept

/// @brief Locate last occurrence of character in string
/// @details get length of span of characters between start and end, until first occurance of a character 
/// @tparam _Ty The string character type, char or wchar_t.
/// @param start The beginning of the string span to search.
/// @param end The end of the string span to search.
/// @param character The character to search for.
/// @returns The number of characters to the last occurance of the character.
template<class _Ty> size_t strrchr_t(const _Ty* start, const _Ty* end, _Ty character) noexcept;
template<class _Ty> inline size_t strrchr_t(const string_span<_Ty>& span, _Ty character) noexcept { return strrchr_t(span.start, span.end, character); } ///< @copydoc template<class _Ty> size_t strrchr_t(const _Ty* start, const _Ty* end, _Ty character) noexcept
template<class _Ty> inline size_t strrchr_t(const std::basic_string<_Ty>& str, _Ty character) noexcept { return strrchr_t(str.data(), str.data() + str.size(), character); } ///< @copydoc template<class _Ty> size_t strrchr_t(const _Ty* start, const _Ty* end, _Ty character) noexcept

/// @brief given a start and end pointer to a string, parse an unsigned decimal number.
/// @details if values other than numbers are found, the parsing stops and the value is returned, so trim any preceeding white space
/// @note the implementation does not modify the input string, and does not hold any internal state.
/// @tparam _Ty The string character type, char or wchar_t.
/// @param start The beginning of the string span to search.
/// @param end The end of the string span to search.
/// @returns The number parsed from the string.
template<class _Ty> uint64_t stou64_t(const _Ty* start, const _Ty* end) noexcept;
template<class _Ty> inline uint64_t stou64_t(const string_span<_Ty>& span) noexcept { return stou64_t(span.start, span.end); }
template<class _Ty> inline uint64_t stou64_t(const std::basic_string<_Ty>& str) noexcept { return stou64_t(str.data(), str.data() + str.size()); }

/// @brief given a start and end pointer to a string, parse a signed decimal number.
/// @details if values other than numbers are found, the parsing stops and the value is returned, so trim any preceeding white space
/// @note the implementation does not modify the input string, and does not hold any internal state.
/// @tparam _Ty The string character type, char or wchar_t.
/// @param start The beginning of the string span to search.
/// @param end The end of the string span to search.
/// @returns The number parsed from the string.
template<class _Ty> int64_t stoi64_t(const _Ty* start, const _Ty* end) noexcept;
template<class _Ty> inline int64_t stoi64_t(const string_span<_Ty>& span) noexcept { return stoi64_t(span.start, span.end); }
template<class _Ty> inline int64_t stoi64_t(const std::basic_string<_Ty>& str) noexcept { return stoi64_t(str.data(), str.data() + str.size()); }

/// @brief given a start and end pointer to a string, lex into a vector of tokens. 
/// @details The lexer handles whitespace and quoted strings, as well as separator characters. 
/// @note For performance, pre-reserve the vector the estimated nunber of tokens
/// @note The lexer cannot handle multi-char separators, but will instead return each char in the separators set as an individual token
/// @param dest The vector to store the tokens in.
/// @param start The beginning of the string span to search.
/// @param end The end of the string span to search.
/// @param separators The list of characters to use as separators. Zero-terminated.
/// @param quotes The list of characters to use as quotes. Zero-terminated.
/// @param whitespaces The list of characters to use as whitespaces. Zero-terminated.
/// @returns True if the lexer succeeded, false if the lexer failed.
template<class _Ty> bool lex_t(std::vector<string_span<_Ty>>* dest, const _Ty* start, const _Ty* end, const _Ty* separators = nullptr, const _Ty* quotes = nullptr, const _Ty* whitespaces = nullptr) noexcept;
template<class _Ty> inline bool lex_t(std::vector<string_span<_Ty>>* dest, const string_span<_Ty>& span, const _Ty* separators = nullptr, const _Ty* quotes = nullptr, const _Ty* whitespaces = nullptr) noexcept { return lex_t(dest, span.start, span.end, separators, quotes, whitespaces); }
template<class _Ty> inline bool lex_t(std::vector<string_span<_Ty>>* dest, const std::basic_string<_Ty>& str, const _Ty* separators = nullptr, const _Ty* quotes = nullptr, const _Ty* whitespaces = nullptr) noexcept { return lex_t(dest, str.data(), str.data() + str.size(), separators, quotes, whitespaces); }

void _bytes_from_hex_string(void* bytes, size_t count, const char* hex_string, bool& success) noexcept;
std::string _bytes_to_hex_string(const void* bytes, size_t count);

/// @brief Convert an utf-8 string to a unicode wide string.
bool string_to_wstring( const std::string &srcStr, std::wstring &destStr );

/// @brief Convert a unicode wide string to a wide string.
bool wstring_to_string( const std::wstring &srcStr, std::string &destStr );

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION
#include "endianness.h"

#if defined(_WIN32)
#define _ADD_CTLE_HEADERS_WIN_STD
#elif defined(linux)
#define _ADD_CTLE_HEADERS_LINUX_STD
#endif
#include "os.inl"

namespace ctle				  
{

// Note: The success flag must be set to true before calling the function, since it is only set to false if the conversion fails.
inline uint8_t decode_hex_char(char c, bool& success) noexcept
{
	if (c >= '0' && c <= '9')
		return uint8_t(c - '0');
	else if (c >= 'a' && c <= 'f')
		return uint8_t((c - 'a') + 10);
	else if (c >= 'A' && c <= 'F')
		return uint8_t((c - 'A') + 10);

	success = false;
	return 0;
}

// retrieves bytes from a hex string of known length.
// note: the count is equal to the number of bytes, and the hex string is assumed to be twice the count (since two hex values is combined into one byte)
void _bytes_from_hex_string(void* bytes, size_t count, const char* hex_string, bool& success) noexcept
{
	success = true;
	uint8_t* p = (uint8_t*)bytes;
	for (size_t i = 0; i < count; ++i)
	{
		uint8_t hn = uint8_t(decode_hex_char(hex_string[i * 2 + 0], success) << 4);
		uint8_t ln = decode_hex_char(hex_string[i * 2 + 1], success);
		p[i] = uint8_t(hn | ln);
	}
}

// retrieves bytes from a hex string of known length.
// versions which does not throw instead clears the success flag if the conversion fails. 
// note: the count is equal to the number of bytes, and the hex string is assumed to be twice the count (since two hex values is combined into one byte)
// Note: The flag must be set before calling the function
template <class T> T from_hex_string(const string_span<char>& str, bool& success) noexcept
{
	uint8_t bytes[sizeof(T)];
	_bytes_from_hex_string(bytes, sizeof(T), str.start, success);
	return from_bigendian<T>(bytes);
}
template <> uint8_t from_hex_string<uint8_t>(const string_span<char>& str, bool& success) noexcept
{
	uint8_t value;
	_bytes_from_hex_string(&value, sizeof(uint8_t), str.start, success);
	return value;
}
template uint16_t from_hex_string<uint16_t>(const string_span<char>& str, bool& success) noexcept;
template uint32_t from_hex_string<uint32_t>(const string_span<char>& str, bool& success) noexcept;
template uint64_t from_hex_string<uint64_t>(const string_span<char>& str, bool& success) noexcept;

template <class T> T from_hex_string(const string_span<char>& str)
{
	bool success = true;
	auto value = from_hex_string<T>(str, success);
	if (!success)
		throw std::runtime_error("Could not convert hex string to bytes, the string contains invalid (non-hex) characters.");
	return value;
}
template uint8_t from_hex_string<uint8_t>(const string_span<char>& str);
template uint16_t from_hex_string<uint16_t>(const string_span<char>& str);
template uint32_t from_hex_string<uint32_t>(const string_span<char>& str);
template uint64_t from_hex_string<uint64_t>(const string_span<char>& str);

// writes array of bytes to string of hex values. the hex values will be
// in the same order as the bytes, so if you need to convert a litte-endian
// word into hex, be sure to flip the byte order before.
std::string _bytes_to_hex_string(const void* bytes, size_t count)
{
	static const char hexchars[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

	std::string ret;
	const uint8_t *p = (const uint8_t *)bytes;
	for( size_t i = 0; i < count; ++i )
	{
		ret += hexchars[( ( *p ) >> 4 ) & 0xf]; // high nibble
		ret += hexchars[( *p ) & 0xf]; // low nibble
		++p;
	}
	return ret;
}

template <class T> std::string to_hex_string(const T& value)
{
	uint8_t bytes[sizeof(T)];
	to_bigendian(bytes, value); 
	return _bytes_to_hex_string(bytes, sizeof(bytes));
}
template <> std::string to_hex_string(const uint8_t& value) { return _bytes_to_hex_string(&value, sizeof(value)); }
template std::string to_hex_string<uint16_t>(const uint16_t& value);
template std::string to_hex_string<uint32_t>(const uint32_t& value);
template std::string to_hex_string<uint64_t>(const uint64_t& value);

template<class _Ty> size_t strspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept
{
	const _Ty* ptr = start;

	// scan until end at maximum
	while (ptr < end)
	{
		// look through delimiters for a match
		bool found = false;
		for (const _Ty* d = control; *d != _Ty(0); ++d)
		{
			if (*d == *ptr)
			{
				found = true;
				break;
			}
		}

		// if not found, we are at the end of the span, early out
		if (!found)
		{
			// return the span to before this pointer
			return ptr - start;
		}

		// step
		++ptr;
	}

	// at the end
	return end - start;
}
template size_t strspn_t<char>(const char* start, const char* end, const char* control) noexcept;
template size_t strspn_t<wchar_t>(const wchar_t* start, const wchar_t* end, const wchar_t* control) noexcept;

template<class _Ty> size_t strcspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept
{
	const _Ty* ptr = start;

	// scan until end at maximum
	while (ptr < end)
	{
		// look through delimiters for a match
		for (const _Ty* d = control; *d != _Ty(0); ++d)
		{
			if (*d == *ptr)
			{
				// we have a match, early out
				// return the span to before this pointer
				return ptr - start;
			}
		}

		// no match, step one
		++ptr;
	}

	// not found, return the full span from start to end
	return end - start;
}
template size_t strcspn_t<char>(const char* start, const char* end, const char* control) noexcept;
template size_t strcspn_t<wchar_t>(const wchar_t* start, const wchar_t* end, const wchar_t* control) noexcept;

template<class _Ty> size_t strcrspn_t(const _Ty* start, const _Ty* end, const _Ty* control) noexcept
{
	const _Ty* ptr = end - 1;

	// scan until end at maximum
	while (ptr >= start)
	{
		// look through delimiters for a match
		for (const _Ty* d = control; *d != _Ty(0); ++d)
		{
			if (*d == *ptr)
			{
				// we have a match, early out
				// return the span to before this pointer
				return ptr - start;
			}
		}

		// no match, step one
		--ptr;
	}

	// not found, return the full span from start to end
	return end - start;
}
template size_t strcrspn_t<char>(const char* start, const char* end, const char* control) noexcept;
template size_t strcrspn_t<wchar_t>(const wchar_t* start, const wchar_t* end, const wchar_t* control) noexcept;

template<class _Ty> string_span<_Ty> strtok_t(const _Ty* start, const _Ty* end, const _Ty* delims) noexcept
{
	// skip over initial delimiters
	size_t span = strspn_t<_Ty>(start, end, delims);
	const _Ty* tokStart = start + span;

	// get span until next occurance of delimiter
	span = strcspn_t<_Ty>(tokStart, end, delims);
	const _Ty* tokEnd = tokStart + span;

	return { tokStart, tokEnd };
}
template string_span<char> strtok_t<char>(const char* start, const char* end, const char* delims) noexcept;
template string_span<wchar_t> strtok_t<wchar_t>(const wchar_t* start, const wchar_t* end, const wchar_t* delims) noexcept;

template<class _Ty> size_t strchr_t(const _Ty* start, const _Ty* end, _Ty character) noexcept
{
	const _Ty* ptr = start;

	// scan until end at maximum
	while (ptr < end)
	{
		if (character == *ptr)
		{
			// we have a match, early out
			// return the span to before this pointer
			return ptr - start;
		}

		// no match, step one
		++ptr;
	}

	// not found, return the full span from start to end
	return end - start;
}
template size_t strchr_t<char>(const char* start, const char* end, char character) noexcept;
template size_t strchr_t<wchar_t>(const wchar_t* start, const wchar_t* end, wchar_t cocharacterntrol) noexcept;

template<class _Ty> size_t strrchr_t(const _Ty* start, const _Ty* end, _Ty character) noexcept
{
	const _Ty* ptr = end - 1;

	// scan until start at maximum
	while (ptr >= start)
	{
		if (character == *ptr)
		{
			// we have a match, early out
			// return the span to before this pointer
			return ptr - start;
		}

		// no match, step one
		--ptr;
	}

	// not found, return the full span from start to end
	return end - start;
}
template size_t strrchr_t<char>(const char* start, const char* end, char character) noexcept;
template size_t strrchr_t<wchar_t>(const wchar_t* start, const wchar_t* end, wchar_t character) noexcept;

template<class _Ty> uint64_t stou64_t(const _Ty* start, const _Ty* end) noexcept
{
	const _Ty* ptr = start;
	uint64_t value = 0;

	if (ptr >= end)
		return 0;

	while (ptr < end)
	{
		size_t c = *ptr - _Ty('0');
		if (c >= 10)
			break; // not a digit

		value = value * 10 + c;
		++ptr;
	}

	return value;
}
template uint64_t stou64_t<char>(const char* start, const char* end) noexcept;
template uint64_t stou64_t<wchar_t>(const wchar_t* start, const wchar_t* end) noexcept;

template<class _Ty> int64_t stoi64_t(const _Ty* start, const _Ty* end) noexcept
{
	const _Ty* ptr = start;
	bool sign = false;

	if (ptr >= end)
		return 0;

	// check for preceeding sign
	if (*ptr == _Ty('-'))
	{
		sign = true;
		++ptr;
	}

	int64_t value = int64_t(stou64_t(ptr, end));
	return sign ? -value : value;
}
template int64_t stoi64_t<char>(const char* start, const char* end) noexcept;
template int64_t stoi64_t<wchar_t>(const wchar_t* start, const wchar_t* end) noexcept;

template<class _Ty> bool lex_t(std::vector<string_span<_Ty>>* dest, const _Ty* start, const _Ty* end, const _Ty* separators, const _Ty* quotes, const _Ty* whitespaces) noexcept
{
	static const _Ty default_separators[] = { (_Ty)',', (_Ty)'*', (_Ty)'/', (_Ty)'(', (_Ty)')', (_Ty)'[', (_Ty)']', (_Ty)'=', (_Ty)'{', '}', (_Ty)0 };
	static const _Ty default_quotes[] = { (_Ty)'"', (_Ty)'\'', (_Ty)0 };
	static const _Ty default_whitespaces[] = { (_Ty)' ', (_Ty)'\t', (_Ty)'\r', (_Ty)'\n', (_Ty)0 };

	if(!separators)
		separators = default_separators;
	if(!quotes)
		quotes = default_quotes;
	if(!whitespaces)
		whitespaces = default_whitespaces;

	const _Ty* ptr = start;

	// lambda to skip over any whitespace 
	auto ws_skip = [&]()
		{
			for (; ptr < end; ++ptr)
			{
				bool found = false;
				for (const _Ty* d = whitespaces; *d != _Ty(0); ++d)
				{
					if (*d == *ptr)
					{
						found = true;
						break;
					}
				}
				if (!found)
					return true;
			}
			return false;
		};

	// check if in list
	auto is_a = [&](const _Ty* char_list)
		{
			for (const _Ty* d = char_list; *d != _Ty(0); ++d)
			{
				if (*d == *ptr)
					return *ptr;
			}
			return _Ty(0);
		};

	// main parse loop
	while (ptr < end)
	{
		// skip whitespace, break if nothing left
		if (!ws_skip())
			break;

		// if this is a separator, output as a specific token, and continue
		if (is_a(separators))
		{
			dest->emplace_back(string_span<_Ty>({ ptr,ptr + 1 }));
			++ptr;
			continue;
		}

		// if this is the start of a string in quotes, parse the whole string as a token
		if (_Ty c = is_a(quotes))
		{
			string_span<_Ty> str_span;

			// scan until we find another of the same quotation mark, (skip the marks in the token span)
			++ptr;
			str_span.start = ptr;
			for (;;)
			{
				if (ptr >= end)
					return false; // error: reached end of string without an end quote
				if (*ptr == c)
				{
					str_span.end = ptr;
					break;
				}
				++ptr;
			}

			dest->emplace_back(str_span);
			++ptr;
			continue;
		}

		// not a string or separator, parse as token until we reach any other character
		string_span<_Ty> token_span;
		token_span.start = ptr;
		++ptr;
		for (;;)
		{
			if (ptr >= end
				|| is_a(separators)
				|| is_a(whitespaces)
				|| is_a(quotes))
			{
				token_span.end = ptr;
				dest->emplace_back(token_span);
				break;
			}
			++ptr;
		}
	}

	return true;
}
template bool lex_t<char>(std::vector<string_span<char>>* dest, const char* start, const char* end, const char* separators, const char* quotes, const char* whitespaces ) noexcept;
template bool lex_t<wchar_t>(std::vector<string_span<wchar_t>>* dest, const wchar_t* start, const wchar_t* end, const wchar_t* separators, const wchar_t* quotes, const wchar_t* whitespaces ) noexcept;

#if defined(_WIN32)

bool string_to_wstring( const std::string &srcStr, std::wstring &destStr )
{
	int size = MultiByteToWideChar( CP_UTF8, 0, srcStr.data(), (int)srcStr.size(), nullptr, 0 );
	if( size==0 )
	{
		return false;
	}

	destStr.resize( size );
	if( MultiByteToWideChar( CP_UTF8, 0, srcStr.data(), (int)srcStr.size(), destStr.data(), size )==0 )
	{
		return false;
	}

	return true;
}

bool wstring_to_string( const std::wstring &srcStr, std::string &destStr )
{
	int size = WideCharToMultiByte( CP_UTF8, 0, srcStr.data(), (int)srcStr.size(), nullptr, 0, nullptr, nullptr );
	if( size==0 )
	{
		return false;
	}

	destStr.resize( size );
	if( WideCharToMultiByte( CP_UTF8, 0, srcStr.data(), (int)srcStr.size(), destStr.data(), size, nullptr, nullptr )==0 )
	{
		return false;
	}

	return true;
}

#elif defined( __linux__ )

// keep a utf-8 locale around for conversions
static class _utf_8_locale
{
public:
	_utf_8_locale()
	{
		utf8_locale = newlocale(LC_CTYPE_MASK, "en_US.UTF-8", nullptr);
	}
	~_utf_8_locale()
	{
		freelocale(utf8_locale);
	}
	locale_t get_locale() const noexcept
	{
		return utf8_locale;
	}

private:
	locale_t utf8_locale;

} utf_8_locale;

bool string_to_wstring( const std::string &srcStr, std::wstring &destStr )
{
	locale_t old_locale = uselocale( utf_8_locale.get_locale() );

	size_t size = std::mbstowcs( nullptr, srcStr.c_str(), 0 );
	if( size==static_cast<size_t>(-1) )
	{
		uselocale( old );
		ec = std::make_error_code( std::errc::illegal_byte_sequence );
		return false;
	}

	destStr.resize( size );
	std::mbstowcs( destStr.data(), srcStr.c_str(), size );

	uselocale( old_locale );
	return out;
}

bool wstring_to_string( const std::wstring &srcStr, std::string &destStr )
{
	locale_t old_locale = uselocale( utf_8_locale.get_locale() );

	size_t size = std::wcstombs( nullptr, srcStr.c_str(), 0 );
	if( len!=static_cast<size_t>(-1) )
	{
		uselocale( old_locale );
		return false;
	}

	destStr.resize( size );
	std::wcstombs( destStr.data(), srcStr.c_str(), size );

	uselocale( old_locale );
	return true;
}

#endif// defined(_WIN32) elif defined(linux)

}
// namespace ctle

#endif//#CTLE_IMPLEMENTATION

#endif//_CTLE_STRING_FUNCS_H_
