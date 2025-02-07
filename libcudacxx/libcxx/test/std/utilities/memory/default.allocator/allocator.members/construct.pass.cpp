//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// allocator:
// template <class... Args> void construct(pointer p, Args&&... args);

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"

int A_constructed = 0;

struct A
{
    int data;
    A() {++A_constructed;}

    A(const A&) {++A_constructed;}

    explicit A(int) {++A_constructed;}
    A(int, int*) {++A_constructed;}

    ~A() {--A_constructed;}
};

int move_only_constructed = 0;

class move_only
{
    move_only(const move_only&) = delete;
    move_only& operator=(const move_only&)= delete;

public:
    move_only(move_only&&) {++move_only_constructed;}
    move_only& operator=(move_only&&) {return *this;}

    move_only() {++move_only_constructed;}
    ~move_only() {--move_only_constructed;}

public:
    int data; // unused other than to make sizeof(move_only) == sizeof(int).
              // but public to suppress "-Wunused-private-field"
};

int main(int, char**)
{
    {
    std::allocator<A> a;
    assert(globalMemCounter.checkOutstandingNewEq(0));
    assert(A_constructed == 0);

    globalMemCounter.last_new_size = 0;
    A* ap = a.allocate(3);
    DoNotOptimize(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(globalMemCounter.checkLastNewSizeEq(3 * sizeof(int)));
    assert(A_constructed == 0);

    a.construct(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(A_constructed == 1);

    a.destroy(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(A_constructed == 0);

    a.construct(ap, A());
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(A_constructed == 1);

    a.destroy(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(A_constructed == 0);

    a.construct(ap, 5);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(A_constructed == 1);

    a.destroy(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(A_constructed == 0);

    a.construct(ap, 5, (int*)0);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(A_constructed == 1);

    a.destroy(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(A_constructed == 0);

    a.deallocate(ap, 3);
    DoNotOptimize(ap);
    assert(globalMemCounter.checkOutstandingNewEq(0));
    assert(A_constructed == 0);
    }
    {
    std::allocator<move_only> a;
    assert(globalMemCounter.checkOutstandingNewEq(0));
    assert(move_only_constructed == 0);

    globalMemCounter.last_new_size = 0;
    move_only* ap = a.allocate(3);
    DoNotOptimize(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(globalMemCounter.checkLastNewSizeEq(3 * sizeof(int)));
    assert(move_only_constructed == 0);

    a.construct(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(move_only_constructed == 1);

    a.destroy(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(move_only_constructed == 0);

    a.construct(ap, move_only());
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(move_only_constructed == 1);

    a.destroy(ap);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    assert(move_only_constructed == 0);

    a.deallocate(ap, 3);
    DoNotOptimize(ap);
    assert(globalMemCounter.checkOutstandingNewEq(0));
    assert(move_only_constructed == 0);
    }

  return 0;
}
