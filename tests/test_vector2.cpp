/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include <doctest/doctest.h>

#include <ostream>
#include <recti/vector2.hpp>

using namespace recti;

TEST_CASE("Vector2") {
    const auto a = 3;
    const auto b = 4;
    const auto c = 5;
    const auto d = 6;

    const auto p = Vector2{a, b};
    const auto q = Vector2{c, d};

    CHECK_EQ(Vector2{8, 10}, (p + q));
    CHECK(Vector2{8, 2} != (p + q));
    CHECK_EQ(Vector2{-2, -2}, (p - q));
    CHECK_EQ(Vector2{6, 8}, (p * 2));
    CHECK_EQ(Vector2{4, 5}, (p + q) / 2);

    CHECK(p != q);
    CHECK_EQ(p + q, q + p);
    CHECK_EQ(p - q, -(q - p));
    CHECK_EQ(p * 3, 3 * p);
    CHECK_EQ(p + (q - p) / 2, (p + q) / 2);

    const auto r = Vector2{-b, c};
    CHECK_EQ((p + q) + r, p + (q + r));
}
