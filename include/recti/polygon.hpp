#pragma once

#include <algorithm>
#include <gsl/span>
#include <vector>

#include "recti.hpp"

namespace recti {

    /**
     * @brief Polygon
     *
     * @tparam T
     */
    template <typename T> class Polygon {
      private:
        Point<T> _origin;
        std::vector<Vector2<T>> _vecs;

      public:
        /**
         * @brief Construct a new Polygon object
         *
         * @param[in] pointset
         */
        explicit constexpr Polygon(gsl::span<const Point<T>> pointset) : _origin{pointset.front()} {
            auto it = pointset.begin();
            for (++it; it != pointset.end(); ++it) {
                this->_vecs.push_back(*it - this->_origin);
            }
        }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return constexpr Point&
         */
        constexpr auto operator+=(const Vector2<T> &rhs) -> Polygon & {
            this->_origin += rhs;
            return *this;
        }

        /**
         * @brief
         *
         * @return T
         */
        [[nodiscard]] constexpr auto signed_area_x2() const -> T {
            auto &&vs = this->_vecs;
            auto n = vs.size();
            assert(n >= 2);
            auto res = vs[0].x() * vs[1].y() - vs[n - 1].x() * vs[n - 2].y();
            auto it = vs.begin();
            for (++it; it != std::prev(vs.end()); ++it) {
                res += it->x() * (std::next(it)->y() - std::prev(it)->y());
            }
            return res;
        }

        /**
         * @brief
         *
         * @return Point<T>
         */
        [[nodiscard]] auto lb() const -> Point<T>;

        /**
         * @brief
         *
         * @return Point<T>
         */
        [[nodiscard]] auto ub() const -> Point<T>;
    };

    /**
     * @brief
     *
     * @tparam Stream
     * @tparam T
     * @param[out] out
     * @param[in] r
     * @return Stream&
     */
    template <class Stream, typename T> auto operator<<(Stream &out, const Polygon<T> &r)
        -> Stream & {
        for (auto &&p : r) {
            out << "  \\draw " << p << ";\n";
        }
        return out;
    }

    /**
     * @brief Create a mono Polygon object
     *
     * @tparam FwIter
     * @tparam Compare
     * @param[in] first
     * @param[in] last
     * @param dir
     */
    template <typename FwIter, typename Compare>
    inline void create_mono_polygon(FwIter &&first, FwIter &&last, Compare &&dir) {
        assert(first != last);

        auto max_pt = *std::max_element(first, last, dir);
        auto min_pt = *std::min_element(first, last, dir);
        auto d = max_pt - min_pt;
        auto middle
            = std::partition(first, last, [&](const auto &a) { return d.cross(a - min_pt) <= 0; });
        std::sort(first, middle, dir);
        std::sort(middle, last, dir);
        std::reverse(middle, last);
    }

    /**
     * @brief Create a xmono Polygon object
     *
     * @tparam FwIter
     * @param[in] first
     * @param[in] last
     */
    template <typename FwIter> inline void create_xmono_polygon(FwIter &&first, FwIter &&last) {
        return create_mono_polygon(first, last, std::less<>());
    }

    /**
     * @brief Create a ymono Polygon object
     *
     * @tparam FwIter
     * @param[in] first
     * @param[in] last
     */
    template <typename FwIter> inline void create_ymono_polygon(FwIter &&first, FwIter &&last) {
        return create_mono_polygon(first, last, [](const auto &a, const auto &b) {
            return std::tie(a.y(), a.x()) < std::tie(b.y(), b.x());
        });
    }

    /**
     * @brief determine if a Point is within a Polygon
     *
     * The code below is from Wm. Randolph Franklin <wrf@ecse.rpi.edu>
     * (see URL below) with some minor modifications for integer. It returns
     * true for strictly interior points, false for strictly exterior, and ub
     * for points on the boundary.  The boundary behavior is complex but
     * determined; in particular, for a partition of a region into polygons,
     * each Point is "in" exactly one Polygon.
     * (See p.243 of [O'Rourke (C)] for a discussion of boundary behavior.)
     *
     * See http://www.faqs.org/faqs/graphics/algorithms-faq/ Subject 2.03
     *
     * @tparam T
     * @param[in] S
     * @param[in] q
     * @return true
     * @return false
     */
    template <typename T>
    inline auto point_in_polygon(gsl::span<const Point<T>> S, const Point<T> &q) -> bool {
        auto c = false;
        auto p0 = S.back();
        for (auto &&p1 : S) {
            if ((p1.y() <= q.y() && q.y() < p0.y()) || (p0.y() <= q.y() && q.y() < p1.y())) {
                auto d = (q - p0).cross(p1 - p0);
                if (p1.y() > p0.y()) {
                    if (d < 0) {
                        c = !c;
                    }
                } else {  // v1.y() < v0.y()
                    if (d > 0) {
                        c = !c;
                    }
                }
            }
            p0 = p1;
        }
        return c;
    }

    /**
     * @brief Polygon is clockwise
     *
     * @tparam T
     * @param[in] S
     * @return true
     * @return false
     */
    template <typename T> inline auto polygon_is_clockwise(gsl::span<const Point<T>> S) -> bool {
        auto it1 = std::min_element(S.begin(), S.end());
        auto it0 = it1 != S.begin() ? std::prev(it1) : S.end() - 1;
        auto it2 = std::next(it1) != S.end() ? std::next(it1) : S.begin();
        return (*it1 - *it0).cross(*it2 - *it1) < 0;
    }

}  // namespace recti
