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
         * @param[in] xcoord
         * @param[in] ycoord
         */
        constexpr Rectangle(Interval<T> &&xcoord, Interval<T> &&ycoord) noexcept
            : Point<Interval<T>>{std::move(xcoord), std::move(ycoord)} {}

        /**
         * @brief Construct a new Rectangle object
         *
         * @param[in] xcoord
         * @param[in] ycoord
         */
        constexpr Rectangle(const Interval<T> &xcoord, const Interval<T> &ycoord) : Point<Interval<T>>{xcoord, ycoord} {}

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
            return {this->xcoord().lb(), this->ycoord().lb()};
        }

        /**
         * @brief
         *
         * @return Point<T>
         */
        [[nodiscard]] constexpr auto ur() const -> Point<T> {
            return {this->xcoord().ub(), this->ycoord().ub()};
        }

        /**
         * @brief
         *
         * @return constexpr T
         */
        [[nodiscard]] constexpr auto area() const -> T { return this->xcoord().len() * this->ycoord().len(); }
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
         * @param[in] xcoord
         * @param[in] ycoord
         */
        constexpr HSegment(Interval<T> &&xcoord, T &&ycoord) noexcept
            : Point<Interval<T>, T>{std::move(xcoord), std::move(ycoord)} {}

        /**
         * @brief Construct a new HSegment object
         *
         * @param[in] xcoord
         * @param[in] ycoord
         */
        constexpr HSegment(const Interval<T> &xcoord, const T &ycoord) : Point<Interval<T>, T>{xcoord, ycoord} {}

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
         * @param[in] xcoord
         * @param[in] ycoord
         */
        constexpr VSegment(T &&xcoord, Interval<T> &&ycoord) noexcept
            : Point<T, Interval<T>>{std::move(xcoord), std::move(ycoord)} {}

        /**
         * @brief Construct a new VSegment object
         *
         * @param[in] xcoord
         * @param[in] ycoord
         */
        constexpr VSegment(const T &xcoord, const Interval<T> &ycoord) : Point<T, Interval<T>>{xcoord, ycoord} {}

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
