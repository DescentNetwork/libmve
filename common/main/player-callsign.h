/*
 * Portions of this file are copyright Rebirth contributors and licensed as
 * described in COPYING.txt.
 * Portions of this file are copyright Parallax Software and licensed
 * according to the Parallax license below.
 * See COPYING.txt for license details.
 */

#pragma once

#ifdef __cplusplus
#include <algorithm>
#include <cctype>
#include <cstddef>
#include "dxxsconf.h"
#include <array>

#define CALLSIGN_LEN                8       // so can use as filename (was: 12)

struct callsign_t
{
	static const std::size_t array_length = CALLSIGN_LEN + 1;
	operator const void *() const = delete;
	using array_t = std::array<char, array_length>;
	typedef char elements_t[array_length];
	array_t a;
	static char lower_predicate(char c)
	{
		return std::tolower(static_cast<unsigned>(c));
	}
	void copy(const char *s, std::size_t N)
	{
		a = {};
		std::copy_n(s, std::min(a.size() - 1, N), begin(a));
	}
	void copy_lower(const char *s, std::size_t N)
	{
		a = {};
		std::transform(s, std::next(s, std::min(a.size() - 1, N)), begin(a), lower_predicate);
	}
	void lower()
	{
		auto ba = begin(a);
		a.back() = 0;
		std::transform(ba, std::prev(end(a)), ba, lower_predicate);
	}
	[[nodiscard]]
	elements_t &buffer()
	{
		return *reinterpret_cast<elements_t *>(a.data());
	}
	template <std::size_t N>
		void operator=(const char (&s)[N])
		{
			static_assert(N <= array_length, "string too long");
			copy(s, N);
		}
	template <std::size_t N>
		void copy_lower(const char (&s)[N])
		{
			static_assert(N <= array_length, "string too long");
			copy_lower(s, N);
		}
	void fill(char c) { a.fill(c); }
	const char &operator[](std::size_t i) const
	{
		return a[i];
	}
	operator const char *() const
	{
		return &a[0];
	};
	bool operator==(const callsign_t &r) const
	{
		return a == r.a;
	}
	bool operator!=(const callsign_t &r) const
	{
		return !(*this == r);
	}
};
static_assert(sizeof(callsign_t) == CALLSIGN_LEN + 1, "callsign_t too big");

#endif
