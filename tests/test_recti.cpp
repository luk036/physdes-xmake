#include <doctest/doctest.h>  // for ResultBuilder, CHECK, Expression_lhs

#include <recti/halton_int.hpp>  // for recti
#include <recti/recti.hpp>       // for Rect, HSegment, VSegment
// #include <random>
#include <ostream>  // for operator<<

#include "recti/interval.hpp"  // for interval, min_dist, overlap
#include "recti/point.hpp"     // for Point, operator<<, operator+, operat...
#include "recti/vector2.hpp"   // for operator/, vector2

// using std::randint;
using namespace recti;

// template <typename T> struct my_point : Point<T, T> { double data; };

TEST_CASE("Point test") {
    auto a = Point<int>{4, 8};
    auto b = Point<int>{5, 6};
    auto v = (b - a) / 2;  // integer division

    CHECK(a < b);
    CHECK(a <= b);
    CHECK(!(a == b));
    CHECK(a != b);
    CHECK(b > a);
    CHECK(b >= a);
    CHECK((a + v) + v != b);  // due to integer division
    CHECK((a - v) + v == a);

    CHECK(a.flip_xy().flip_xy() == a);
    CHECK(a.flip_y().flip_y() == a);
}

TEST_CASE("Rectangle test") {
    auto xrng1 = interval{4, 8};
    auto yrng1 = interval{5, 7};
    auto r1 = Rect{xrng1, yrng1};
    auto xrng2 = interval{5, 7};
    auto yrng2 = interval{6, 6};
    auto r2 = Rect{xrng2, yrng2};
    auto p = Point{7, 6};
    auto v = Vector2{5, 6};

    CHECK(r1 != r2);
    CHECK((r1 - v) + v == r1);

    CHECK(r1 <= p);
    CHECK(r1.contains(p));
    CHECK(r1.contains(r2));
    CHECK(r1.overlaps(r2));
    CHECK(overlap(r1, r2));

    CHECK(r1.min_dist_with(r2) == 0);
    CHECK(min_dist(r1, r2) == 0);
}

TEST_CASE("Segment test") {
    auto xrng1 = interval{4, 8};
    auto yrng1 = interval{5, 7};
    auto s1 = HSegment{xrng1, 6};
    auto s2 = VSegment{5, yrng1};

    CHECK(s1.overlaps(s2));
}
