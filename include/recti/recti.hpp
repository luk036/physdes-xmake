#pragma once

#include "interval.hpp"  // for Interval
#include "point.hpp"     // for Point

namespace recti {

#pragma pack(push, 1)
    /**
     * @brief Rectangle (Rectilinear)
     *
     * @tparam T
     */
    template <typename T> struct Rectangle : Point<Interval<T>> {
        /**
         * @brief Construct a new Rectangle object
         *
         * @param[in] x
         * @param[in] y
         */
        constexpr Rectangle(Interval<T> &&x, Interval<T> &&y) noexcept
            : Point<Interval<T>>{std::move(x), std::move(y)} {}

        /**
         * @brief Construct a new Rectangle object
         *
         * @param[in] x
         * @param[in] y
         */
        constexpr Rectangle(const Interval<T> &x, const Interval<T> &y) : Point<Interval<T>>{x, y} {}

        /**
         * @brief Construct a new Rectangle object from the base object (implicitly)
         *
         * @param[in] p
         */
        constexpr Rectangle(Point<Interval<T>> &&p) noexcept
            : Point<Interval<T>>{std::move(p)} {}

        /**
         * @brief Construct a new Rectangle object from the base object (implicitly)
         *
         * @param[in] p
         */
        constexpr Rectangle(const Point<Interval<T>> &p)
            : Point<Interval<T>>{p} {}

        /**
         * @brief
         *
         * @return Point<T>
         */
        [[nodiscard]] constexpr auto ll() const -> Point<T> {
            return {this->x().lb(), this->y().lb()};
        }

        /**
         * @brief
         *
         * @return Point<T>
         */
        [[nodiscard]] constexpr auto ur() const -> Point<T> {
            return {this->x().ub(), this->y().ub()};
        }

        /**
         * @brief
         *
         * @return constexpr T
         */
        [[nodiscard]] constexpr auto area() const -> T { return this->x().len() * this->y().len(); }
    };
#pragma pack(pop)

#pragma pack(push, 1)
    /**
     * @brief Horizontal Line Segment
     *
     * @tparam T
     */
    template <typename T> struct HSegment : Point<Interval<T>, T> {
        /**
         * @brief Construct a new HSegment object
         *
         * @param[in] x
         * @param[in] y
         */
        constexpr HSegment(Interval<T> &&x, T &&y) noexcept
            : Point<Interval<T>, T>{std::move(x), std::move(y)} {}

        /**
         * @brief Construct a new HSegment object
         *
         * @param[in] x
         * @param[in] y
         */
        constexpr HSegment(const Interval<T> &x, const T &y) : Point<Interval<T>, T>{x, y} {}

        /**
         * @brief Construct a new HSegment object from the base object (implicitly)
         *
         * @param[in] p
         */
        constexpr HSegment(Point<Interval<T>, T> &&p) noexcept
            : Point<Interval<T>, T>{std::move(p)} {}

        /**
         * @brief Construct a new HSegment object from the base object (implicitly)
         *
         * @param[in] p
         */
        constexpr HSegment(const Point<Interval<T>, T> &p)
            : Point<Interval<T>, T>{p} {}

    };
#pragma pack(pop)

#pragma pack(push, 1)
    /**
     * @brief VSegment Line Segment
     *
     * @tparam T
     */
    template <typename T> struct VSegment : Point<T, Interval<T>> {
        /**
         * @brief Construct a new VSegment object
         *
         * @param[in] x
         * @param[in] y
         */
        constexpr VSegment(T &&x, Interval<T> &&y) noexcept
            : Point<T, Interval<T>>{std::move(x), std::move(y)} {}

        /**
         * @brief Construct a new VSegment object
         *
         * @param[in] x
         * @param[in] y
         */
        constexpr VSegment(const T &x, const Interval<T> &y) : Point<T, Interval<T>>{x, y} {}

        /**
         * @brief Construct a new VSegment object from the base object (implicitly)
         *
         * @param[in] p
         */
        constexpr VSegment(Point<T, Interval<T>> &&p) noexcept
            : Point<T, Interval<T>>{std::move(p)} {}

        /**
         * @brief Construct a new VSegment object from the base object (implicitly)
         *
         * @param[in] p
         */
        constexpr VSegment(const Point<T, Interval<T>> &p)
            : Point<T, Interval<T>>{p} {}

    };
#pragma pack(pop)

}  // namespace recti
