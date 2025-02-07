//===----------------------------------------------------------------------===////
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===////

// FIXME: This file is copied from libcxx/src/include/atomic_support.h. Instead
// of duplicating the file in libc++abi we should require that the libc++
// sources are available when building libc++abi.

#ifndef ATOMIC_SUPPORT_H
#define ATOMIC_SUPPORT_H

#include "__config"
#include "memory" // for __libcpp_relaxed_load

#if defined(__clang__) && __has_builtin(__atomic_load_n)             \
                       && __has_builtin(__atomic_store_n)            \
                       && __has_builtin(__atomic_add_fetch)          \
                       && __has_builtin(__atomic_exchange_n)         \
                       && __has_builtin(__atomic_compare_exchange_n) \
                       && defined(__ATOMIC_RELAXED)                  \
                       && defined(__ATOMIC_CONSUME)                  \
                       && defined(__ATOMIC_ACQUIRE)                  \
                       && defined(__ATOMIC_RELEASE)                  \
                       && defined(__ATOMIC_ACQ_REL)                  \
                       && defined(__ATOMIC_SEQ_CST)
#   define _LIBCXXABI_HAS_ATOMIC_BUILTINS
#elif defined(_CCCL_COMPILER_NVHPC)
#   define _LIBCXXABI_HAS_ATOMIC_BUILTINS
#elif !defined(__clang__) && defined(_GNUC_VER) && _GNUC_VER >= 407
#   define _LIBCXXABI_HAS_ATOMIC_BUILTINS
#endif

#if !defined(_LIBCXXABI_HAS_ATOMIC_BUILTINS) && !defined(_LIBCXXABI_HAS_NO_THREADS)
# if defined(_LIBCUDACXX_WARNING)
    _LIBCUDACXX_WARNING("Building libc++ without __atomic builtins is unsupported")
# else
#   error Building libc++ without __atomic builtins is unsupported
# endif
#endif

_LIBCUDACXX_BEGIN_NAMESPACE_STD

namespace {

#if defined(_LIBCXXABI_HAS_ATOMIC_BUILTINS) && !defined(_LIBCXXABI_HAS_NO_THREADS)

enum __libcpp_atomic_order {
    _AO_Relaxed = __ATOMIC_RELAXED,
    _AO_Consume = __ATOMIC_CONSUME,
    _AO_Acquire = __ATOMIC_ACQUIRE,
    _AO_Release = __ATOMIC_RELEASE,
    _AO_Acq_Rel = __ATOMIC_ACQ_REL,
    _AO_Seq     = __ATOMIC_SEQ_CST
};

template <class _ValueType, class _FromType>
inline _LIBCUDACXX_INLINE_VISIBILITY
void __libcpp_atomic_store(_ValueType* __dest, _FromType __val,
                           int __order = _AO_Seq)
{
    __atomic_store_n(__dest, __val, __order);
}

template <class _ValueType, class _FromType>
inline _LIBCUDACXX_INLINE_VISIBILITY
void __libcpp_relaxed_store(_ValueType* __dest, _FromType __val)
{
    __atomic_store_n(__dest, __val, _AO_Relaxed);
}

template <class _ValueType>
inline _LIBCUDACXX_INLINE_VISIBILITY
_ValueType __libcpp_atomic_load(_ValueType const* __val,
                                int __order = _AO_Seq)
{
    return __atomic_load_n(__val, __order);
}

template <class _ValueType, class _AddType>
inline _LIBCUDACXX_INLINE_VISIBILITY
_ValueType __libcpp_atomic_add(_ValueType* __val, _AddType __a,
                               int __order = _AO_Seq)
{
    return __atomic_add_fetch(__val, __a, __order);
}

template <class _ValueType>
inline _LIBCUDACXX_INLINE_VISIBILITY
_ValueType __libcpp_atomic_exchange(_ValueType* __target,
                                    _ValueType __value, int __order = _AO_Seq)
{
    return __atomic_exchange_n(__target, __value, __order);
}

template <class _ValueType>
inline _LIBCUDACXX_INLINE_VISIBILITY
bool __libcpp_atomic_compare_exchange(_ValueType* __val,
    _ValueType* __expected, _ValueType __after,
    int __success_order = _AO_Seq,
    int __fail_order = _AO_Seq)
{
    return __atomic_compare_exchange_n(__val, __expected, __after, true,
                                       __success_order, __fail_order);
}

#else // _LIBCUDACXX_HAS_NO_THREADS

enum __libcpp_atomic_order {
    _AO_Relaxed,
    _AO_Consume,
    _AO_Acquire,
    _AO_Release,
    _AO_Acq_Rel,
    _AO_Seq
};

template <class _ValueType, class _FromType>
inline _LIBCUDACXX_INLINE_VISIBILITY
void __libcpp_atomic_store(_ValueType* __dest, _FromType __val,
                           int = 0)
{
    *__dest = __val;
}

template <class _ValueType, class _FromType>
inline _LIBCUDACXX_INLINE_VISIBILITY
void __libcpp_relaxed_store(_ValueType* __dest, _FromType __val)
{
    *__dest = __val;
}

template <class _ValueType>
inline _LIBCUDACXX_INLINE_VISIBILITY
_ValueType __libcpp_atomic_load(_ValueType const* __val,
                                int = 0)
{
    return *__val;
}

template <class _ValueType, class _AddType>
inline _LIBCUDACXX_INLINE_VISIBILITY
_ValueType __libcpp_atomic_add(_ValueType* __val, _AddType __a,
                               int = 0)
{
    return *__val += __a;
}

template <class _ValueType>
inline _LIBCUDACXX_INLINE_VISIBILITY
_ValueType __libcpp_atomic_exchange(_ValueType* __target,
                                    _ValueType __value, int  = _AO_Seq)
{
    _ValueType old = *__target;
    *__target = __value;
    return old;
}

template <class _ValueType>
inline _LIBCUDACXX_INLINE_VISIBILITY
bool __libcpp_atomic_compare_exchange(_ValueType* __val,
    _ValueType* __expected, _ValueType __after,
    int = 0, int = 0)
{
    if (*__val == *__expected) {
        *__val = __after;
        return true;
    }
    *__expected = *__val;
    return false;
}

#endif // _LIBCUDACXX_HAS_NO_THREADS

} // end namespace

_LIBCUDACXX_END_NAMESPACE_STD

#endif // ATOMIC_SUPPORT_H
