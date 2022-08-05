#include <doctest/doctest.h>  // for ResultBuilder, CHECK, Expression_lhs
#include <fmt/core.h>

#include <gsl/span>              // for span
#include <recti/halton_int.hpp>  // for vdcorput, recti
#include <recti/rpolygon.hpp>    // for RPolygon, RPolygon_is_clockwise, cre...
#include <vector>                // for vector

#include "recti/point.hpp"  // for Point, operator>

// using std::randint;
using namespace recti;

TEST_CASE("Rectilinear Polygon test (y-mono)") {
    auto S = std::vector<Point<int>>{{-2, 2},  {0, -1}, {-5, 1}, {-2, 4}, {0, -4},  {-4, 3},
                                     {-6, -2}, {5, 1},  {2, 2},  {3, -3}, {-3, -4}, {1, 4}};
    auto is_anticlockwise = create_ymono_rpolygon(S.begin(), S.end());
    auto P = RPolygon<int>(S);
    CHECK(is_anticlockwise);
    CHECK_EQ(P.signed_area(), 45);
    CHECK(!rpolygon_is_clockwise<int>(S));
    CHECK(!point_in_rpolygon<int>(S, Point{4, 5}));
}

TEST_CASE("Rectilinear Polygon test (x-mono)") {
    auto S = std::vector<Point<int>>{{-2, 2},  {0, -1}, {-5, 1}, {-2, 4}, {0, -4},  {-4, 3},
                                     {-6, -2}, {5, 1},  {2, 2},  {3, -3}, {-3, -4}, {1, 4}};
    auto is_anticlockwise = create_xmono_rpolygon(S.begin(), S.end());
    auto P = RPolygon<int>(S);
    CHECK(!is_anticlockwise);
    CHECK_EQ(P.signed_area(), -53);
    CHECK(rpolygon_is_clockwise<int>(S));
}

TEST_CASE("Rectilinear Polygon test (y-mono 50)") {
    auto hgenX = vdcorput(3, 7);
    auto hgenY = vdcorput(2, 11);
    auto S = std::vector<Point<int>>{};
    for (auto i = 0; i != 50; ++i) {
        S.emplace_back(Point<int>(int(hgenX()), int(hgenY())));
    }
    auto is_anticlockwise = create_ymono_rpolygon(S.begin(), S.end());
    // fmt::print(
    //     "\n<svg viewBox='0 0 2187 2048'
    //     xmlns='http://www.w3.org/2000/svg'>\n");
    // fmt::print("  <polygon points='");
    // auto p0 = S.back();
    // for (auto&& p1 : S)
    // {
    //     fmt::print("{},{} {},{} ", p0.x(), p0.y(), p1.x(), p0.y());
    //     p0 = p1;
    // }
    // fmt::print("'\n");
    // fmt::print("  fill='#88C0D0' stroke='black' />\n");
    // for (auto&& p : S)
    // {
    //     fmt::print("  <circle cx='{}' cy='{}' r='10' />\n", p.x(), p.y());
    // }

    auto q = Point<int>(int(hgenX()), int(hgenY()));
    // fmt::print(
    //     "  <circle cx='{}' cy='{}' r='10' fill='#BF616A' />\n", q.x(), q.y());
    fmt::print("</svg>\n");

    auto P = RPolygon<int>(S);
    CHECK(!is_anticlockwise);
    CHECK_EQ(P.signed_area(), -2032128);
    CHECK(rpolygon_is_clockwise<int>(S));
    CHECK(!point_in_rpolygon<int>(S, q));
    // puts("Hello world1\n");
}
