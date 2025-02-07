//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// iterator erase(const_iterator p);

#include <string>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

template <class S>
void
test(S s, typename S::difference_type pos, S expected)
{
    typename S::const_iterator p = s.begin() + pos;
    typename S::iterator i = s.erase(p);
    LIBCPP_ASSERT(s.__invariants());
    assert(s[s.size()] == typename S::value_type());
    assert(s == expected);
    assert(i - s.begin() == pos);
}

int main(int, char**)
{
    {
    typedef std::string S;
    test(S("abcde"), 0, S("bcde"));
    test(S("abcde"), 1, S("acde"));
    test(S("abcde"), 2, S("abde"));
    test(S("abcde"), 4, S("abcd"));
    test(S("abcdefghij"), 0, S("bcdefghij"));
    test(S("abcdefghij"), 1, S("acdefghij"));
    test(S("abcdefghij"), 5, S("abcdeghij"));
    test(S("abcdefghij"), 9, S("abcdefghi"));
    test(S("abcdefghijklmnopqrst"), 0, S("bcdefghijklmnopqrst"));
    test(S("abcdefghijklmnopqrst"), 1, S("acdefghijklmnopqrst"));
    test(S("abcdefghijklmnopqrst"), 10, S("abcdefghijlmnopqrst"));
    test(S("abcdefghijklmnopqrst"), 19, S("abcdefghijklmnopqrs"));
    }
    {
    typedef std::basic_string<char, std::char_traits<char>, min_allocator<char>> S;
    test(S("abcde"), 0, S("bcde"));
    test(S("abcde"), 1, S("acde"));
    test(S("abcde"), 2, S("abde"));
    test(S("abcde"), 4, S("abcd"));
    test(S("abcdefghij"), 0, S("bcdefghij"));
    test(S("abcdefghij"), 1, S("acdefghij"));
    test(S("abcdefghij"), 5, S("abcdeghij"));
    test(S("abcdefghij"), 9, S("abcdefghi"));
    test(S("abcdefghijklmnopqrst"), 0, S("bcdefghijklmnopqrst"));
    test(S("abcdefghijklmnopqrst"), 1, S("acdefghijklmnopqrst"));
    test(S("abcdefghijklmnopqrst"), 10, S("abcdefghijlmnopqrst"));
    test(S("abcdefghijklmnopqrst"), 19, S("abcdefghijklmnopqrs"));
    }

  return 0;
}
