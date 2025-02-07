//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//


// <string_view>

// void remove_prefix(size_type _n)

#include <string_view>
#include <cassert>

#include "test_macros.h"

template<typename CharT>
void test ( const CharT *s, size_t len ) {
    typedef std::basic_string_view<CharT> SV;
    {
    SV sv1 ( s );
    assert ( sv1.size() == len );
    assert ( sv1.data() == s );

    if ( len > 0 ) {
        sv1.remove_prefix ( 1 );
        assert ( sv1.size() == (len - 1));
        assert ( sv1.data() == (s + 1));
        sv1.remove_prefix ( len - 1 );
    }

    assert ( sv1.size() == 0 );
    sv1.remove_prefix ( 0 );
    assert ( sv1.size() == 0 );
    }
}

#if TEST_STD_VER > 2011
constexpr size_t test_ce ( size_t n, size_t k ) {
    typedef std::basic_string_view<char> SV;
    SV sv1{ "ABCDEFGHIJKL", n };
    sv1.remove_prefix ( k );
    return sv1.size();
}
#endif

int main(int, char**) {
    test ( "ABCDE", 5 );
    test ( "a", 1 );
    test ( "", 0 );

    test ( L"ABCDE", 5 );
    test ( L"a", 1 );
    test ( L"", 0 );

    test ( u"ABCDE", 5 );
    test ( u"a", 1 );
    test ( u"", 0 );

    test ( U"ABCDE", 5 );
    test ( U"a", 1 );
    test ( U"", 0 );

#if TEST_STD_VER > 2011
    {
    static_assert ( test_ce ( 5, 0 ) == 5, "" );
    static_assert ( test_ce ( 5, 1 ) == 4, "" );
    static_assert ( test_ce ( 5, 5 ) == 0, "" );
    static_assert ( test_ce ( 9, 3 ) == 6, "" );
    }
#endif

  return 0;
}
