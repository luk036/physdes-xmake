#pragma once

#include <algorithm>  // import std::min, std::max
#include <cassert>
#include <utility>  // import std::move

#include "interval.hpp"
#include "point.hpp"

namespace recti {

#pragma pack(push, 1)
    /**
     * @brief merge_obj
     *
     * object includes a single Point, segment, or a region
     *
     * @tparam T
     */
    template <typename T1, typename T2> class MergeObj : public Point<T1, T2> {
      public:
        /**
         * @brief Construct a new Point object
         *
         * @param[in] xcoord
         * @param[in] ycoord
         */
        constexpr MergeObj(T1 &&xcoord, T2 &&ycoord) noexcept : Point<T1, T2>{std::move(xcoord), std::move(ycoord)} {}

        // /**
        //  * @brief Construct a new Point object
        //  *
        //  * @param[in] xcoord
        //  * @param[in] ycoord
        //  */
        // constexpr MergeObj(const T1& xcoord, const T2& ycoord) : Point<T1, T2>{xcoord + ycoord, xcoord - ycoord}
        // {}

        /**
         * @brief Add a vector (translation)
         *
         * @tparam U
         * @param[in] rhs
         * @return MergeObj&
         */
        template <typename U> constexpr auto operator+=(const Vector2<U> &rhs) -> MergeObj & {
            this->_x += rhs.x() + rhs.y();
            this->_y += rhs.x() - rhs.y();
            return *this;
        }

        /**
         * @brief Substract a vector (translation)
         *
         * @tparam U
         * @param[in] rhs
         * @return MergeObj&
         */
        template <typename U> constexpr auto operator-=(const Vector2<U> &rhs) -> MergeObj & {
            this->_x -= rhs.x() + rhs.y();
            this->_y -= rhs.x() - rhs.y();
            return *this;
        }

        /**
         * @brief Add
         *
         * @tparam U
         * @param[in] xcoord
         * @param[in] ycoord
         * @return Vector2<T>
         */
        template <typename U>  //
        friend constexpr auto operator+(MergeObj xcoord, const Vector2<U> &ycoord) -> MergeObj {
            return xcoord += ycoord;
        }

        /**
         * @brief Substract
         *
         * @tparam U
         * @param[in] xcoord
         * @param[in] ycoord
         * @return Vector2<T>
         */
        template <typename U>  //
        friend constexpr auto operator-(MergeObj xcoord, const Vector2<U> &ycoord) -> MergeObj {
            return xcoord -= ycoord;
        }

        /**
         * @brief overlap
         *
         * @tparam U1
         * @tparam U2
         * @param other
         * @return true
         * @return false
         */
        template <typename U1, typename U2>  //
        [[nodiscard]] constexpr auto min_dist_with(const MergeObj<U1, U2> &other) const {
            return std::max(min_dist(this->_x, other._x), min_dist(this->_y, other._y));
        }

        template <typename R>  //
        friend constexpr auto enlarge(const MergeObj &lhs, const R &alpha) {
            auto xcoord = enlarge(lhs.xcoord(), alpha);
            auto ycoord = enlarge(lhs.ycoord(), alpha);
            return MergeObj<decltype(xcoord), decltype(ycoord)>{std::move(xcoord), std::move(ycoord)};
        }

        /**
         * @brief overlap
         *
         * @tparam U1
         * @tparam U2
         * @param other
         * @return true
         * @return false
         */
        template <typename U1, typename U2>  //
        [[nodiscard]] constexpr auto merge_with(const MergeObj<U1, U2> &other) const {
            auto alpha = this->min_dist_with(other);
            auto half = alpha / 2;
            auto trr1 = enlarge(*this, half);
            auto trr2 = enlarge(other, alpha - half);
            return intersection(trr1, trr2);
        }

        // /**
        //  * @brief minimum distance with
        //  *
        //  * @param[in] other
        //  * @return constexpr auto
        //  */
        // [[nodiscard]] constexpr auto min_dist_change_with(MergeObj& other) {
        //     auto minDist = this->min_dist_with(other);
        //     auto mobj1 = this->enlarge_with(minDist);
        //     auto mobj2 = other.enlarge_with(minDist);
        //     other = mobj1.intersection_with(other);
        //     *this = mobj2.intersection_with(*this);
        //     return minDist;
        // }
    };
#pragma pack(pop)

    // template <typename U1, typename U2>  //
    // inline constexpr auto min_dist_change_merge(U1& lhs, U2& rhs) {
    //     if constexpr (std::is_scalar_v<U1>) {
    //         if constexpr (std::is_scalar_v<U2>) {
    //             return std::abs(lhs - rhs);
    //         } else {
    //             return rhs.min_dist_change_with(lhs);
    //         }
    //     } else {
    //         return lhs.min_dist_change_with(rhs);
    //     }
    // }

}  // namespace recti
