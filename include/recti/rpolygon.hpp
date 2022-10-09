#pragma once

#include <algorithm>
#include <gsl/span>
#include <vector>

#include "recti.hpp"

namespace recti {

    /**
     * @brief Rectilinear Polygon
     *
     * @tparam T
     */
    template <typename T> class RPolygon {
      private:
        Point<T> _origin{};
        std::vector<Vector2<T>> _vecs{};  // @todo: add custom allocator support

      public:
        /**
         * @brief Construct a new RPolygon object
         *
         * @param[in] pointset
         */
        explicit constexpr RPolygon(gsl::span<const Point<T>> pointset)
            : _origin{pointset.front()} {
            for (auto itr = std::next(pointset.begin()); itr != pointset.end(); ++itr) {
                this->_vecs.push_back(*itr - this->_origin);
            }
        }

        /**
         * @brief
         *
         * @param[in] rhs
         * @return constexpr Point&
         */
        constexpr auto operator+=(const Vector2<T> &vector) -> RPolygon & {
            this->_origin += vector;
            return *this;
        }

        /**
         * @brief
         *
         * @return T
         */
        [[nodiscard]] constexpr auto signed_area() const -> T {
            assert(this->_vecs.size() >= 1);
            auto first = this->_vecs.begin();
            auto res = first->x() * first->y();
            for (auto itr = std::next(first); itr != this->_vecs.end(); ++itr) {
                res += itr->x() * (itr->y() - std::prev(itr)->y());
            }
            return res;
        }

        /**
         * @brief
         *
         * @tparam U
         * @param[in] rhs
         * @return true
         * @return false
         */
        template <typename U> auto contains(const Point<U> &rhs) const -> bool;

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
     * @brief Create a xmono RPolygon object
     *
     * @tparam FwIter
     * @param[in] first
     * @param[in] last
     * @return true
     * @return false
     */
    template <typename FwIter> inline auto create_xmono_rpolygon(FwIter &&first, FwIter &&last)
        -> bool {
        assert(first != last);

        const auto leftmost = *std::min_element(first, last);
        const auto rightmost = *std::max_element(first, last);
        const auto is_anticlockwise = rightmost.ycoord() <= leftmost.ycoord();
        auto r2l
            = [&leftmost](const auto &elem) -> bool { return elem.ycoord() <= leftmost.ycoord(); };
        auto l2r
            = [&leftmost](const auto &elem) -> bool { return elem.ycoord() >= leftmost.ycoord(); };
        const auto middle = is_anticlockwise ? std::partition(first, last, std::move(r2l))
                                             : std::partition(first, last, std::move(l2r));
        std::sort(first, middle);
        std::sort(middle, last, std::greater<>());  // in which standard?
        return is_anticlockwise;
    }

    /**
     * @brief Create a ymono RPolygon object
     *
     * @tparam FwIter
     * @param[in] first
     * @param[in] last
     * @return true
     * @return false
     */
    template <typename FwIter> inline auto create_ymono_rpolygon(FwIter &&first, FwIter &&last)
        -> bool {
        assert(first != last);

        auto upward = [](const auto &rhs, const auto &lhs) -> bool {
            return std::tie(rhs.ycoord(), rhs.xcoord()) < std::tie(lhs.ycoord(), lhs.xcoord());
        };
        auto downward = [](const auto &rhs, const auto &lhs) -> bool {
            return std::tie(rhs.ycoord(), rhs.xcoord()) > std::tie(lhs.ycoord(), lhs.xcoord());
        };
        const auto botmost = *std::min_element(first, last, upward);
        const auto topmost = *std::max_element(first, last, upward);
        const auto is_anticlockwise = topmost.xcoord() >= botmost.xcoord();
        auto t2b
            = [&botmost](const auto &elem) -> bool { return elem.xcoord() >= botmost.xcoord(); };
        auto b2t
            = [&botmost](const auto &elem) -> bool { return elem.xcoord() <= botmost.xcoord(); };
        const auto middle = is_anticlockwise ? std::partition(first, last, std::move(t2b))
                                             : std::partition(first, last, std::move(b2t));
        std::sort(first, middle, std::move(upward));
        std::sort(middle, last, std::move(downward));
        return is_anticlockwise;
    }

    /**
     * @brief
     *
     * @tparam T
     * @tparam FwIter
     * @param[in] first
     * @param[in] last
     */
    template <typename FwIter> inline void create_test_rpolygon(FwIter &&first, FwIter &&last) {
        assert(first != last);

        auto upwd = [](const auto &rhs, const auto &lhs) -> bool {
            return std::tie(rhs.ycoord(), rhs.xcoord()) < std::tie(lhs.ycoord(), lhs.xcoord());
        };
        auto down = [](const auto &rhs, const auto &lhs) -> bool {
            return std::tie(rhs.ycoord(), rhs.xcoord()) > std::tie(lhs.ycoord(), lhs.xcoord());
        };
        auto left = [](const auto &rhs, const auto &lhs) {
            return std::tie(rhs.xcoord(), rhs.ycoord()) < std::tie(lhs.xcoord(), lhs.ycoord());
        };
        auto right = [](const auto &rhs, const auto &lhs) {
            return std::tie(rhs.xcoord(), rhs.ycoord()) > std::tie(lhs.xcoord(), lhs.ycoord());
        };

        auto min_pt = *std::min_element(first, last, upwd);
        auto max_pt = *std::max_element(first, last, upwd);
        auto d_x = max_pt.xcoord() - min_pt.xcoord();
        auto d_y = max_pt.ycoord() - min_pt.ycoord();
        auto middle = std::partition(first, last, [&min_pt, &d_x, &d_y](const auto &elem) -> bool {
            return d_x * (elem.ycoord() - min_pt.ycoord())
                   < (elem.xcoord() - min_pt.xcoord()) * d_y;
        });
        auto max_pt1 = *std::max_element(first, middle, left);
        auto middle2 = std::partition(first, middle, [&max_pt1](const auto &elem) -> bool {
            return elem.ycoord() < max_pt1.ycoord();
        });
        auto min_pt2 = *std::min_element(middle, last, left);
        auto middle3 = std::partition(middle, last, [&min_pt2](const auto &elem) -> bool {
            return elem.ycoord() > min_pt2.ycoord();
        });

        if (d_x < 0) {  // clockwise
            std::sort(first, middle2, down);
            std::sort(middle2, middle, left);
            std::sort(middle, middle3, upwd);
            std::sort(middle3, last, right);
        } else {  // anti-clockwise
            std::sort(first, middle2, left);
            std::sort(middle2, middle, upwd);
            std::sort(middle, middle3, right);
            std::sort(middle3, last, down);
        }
    }

    /**
     * @brief determine if a Point is within a Polygon
     *
     * The code below is from Wm. Randolph Franklin <wrf@ecse.rpi.edu>
     * (see URL below) with some minor modifications for rectilinear. It returns
     * true for strictly interior points, false for strictly exterior, and ub
     * for points on the boundary.  The boundary behavior is complex but
     * determined; in particular, for a partition of a region into polygons,
     * each Point is "in" exactly one Polygon.
     * (See p.243 of [O'Rourke (C)] for a discussion of boundary behavior.)
     *
     * See http://www.faqs.org/faqs/graphics/algorithms-faq/ Subject 2.03
     *
     * @tparam T
     * @param[in] pointset
     * @param[in] ptq
     * @return true
     * @return false
     */
    template <typename T>
    inline auto point_in_rpolygon(gsl::span<const Point<T>> pointset, const Point<T> &ptq) -> bool {
        auto res = false;
        auto pt0 = pointset.back();
        for (auto &&pt1 : pointset) {
            if ((pt1.ycoord() <= ptq.ycoord() && ptq.ycoord() < pt0.ycoord())
                || (pt0.ycoord() <= ptq.ycoord() && ptq.ycoord() < pt1.ycoord())) {
                if (pt1.xcoord() > ptq.xcoord()) {
                    res = !res;
                }
            }
            pt0 = pt1;
        }
        return res;
    }

    /**
     * @brief Polygon is clockwise
     *
     * @tparam T
     * @param[in] pointset
     * @return true
     * @return false
     */
    template <typename T> inline auto rpolygon_is_clockwise(gsl::span<const Point<T>> pointset)
        -> bool {
        auto it1 = std::min_element(pointset.begin(), pointset.end());
        auto it0 = it1 != pointset.begin() ? std::prev(it1) : pointset.end() - 1;
        if (it1->ycoord() < it0->ycoord()) {
            return false;
        }
        if (it1->ycoord() > it0->ycoord()) {
            return true;
        }
        // it1->ycoord() == it0->ycoord()
        auto it2 = std::next(it1) != pointset.end() ? std::next(it1) : pointset.begin();
        return it2->ycoord() > it1->ycoord();
    }

}  // namespace recti
