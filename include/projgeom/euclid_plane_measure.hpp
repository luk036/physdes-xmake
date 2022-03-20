#pragma once

#include "euclid_plane.hpp"
#include "fractions.hpp"

namespace fun {

    /**
     * @brief
     *
     * @tparam K
     * @param[in] x1
     * @param[in] z1
     * @param[in] x2
     * @param[in] z2
     * @return auto
     */
    template <typename K>
    requires Integral<K>
    inline constexpr auto quad1(const K& x1, const K& z1, const K& x2, const K& z2) {
        return sq(Fraction<K>(x1, z1) - Fraction<K>(x2, z2));
    }

    /**
     * @brief
     *
     * @tparam K
     * @param[in] x1
     * @param[in] z1
     * @param[in] x2
     * @param[in] z2
     * @return auto
     */
    template <typename K>
    // requires (!Integral<K>)
    inline constexpr auto quad1(const K& x1, const K& z1, const K& x2, const K& z2) {
        return sq(x1 / z1 - x2 / z2);
    }

    /**
     * @brief
     *
     * @param[in] a1
     * @param[in] a2
     * @return auto
     */
    template <Projective_plane_coord2 P> inline constexpr auto quadrance(const P& a1, const P& a2) {
        return quad1(a1[0], a1[2], a2[0], a2[2]) + quad1(a1[1], a1[2], a2[1], a2[2]);
    }

    template <typename... Args> inline constexpr auto quadrance_copy(const Args&... args) {
        return std::make_tuple(quadrance(args.first, args.second)...);
    }

    // Projective_plane2 { L }
    // constexpr auto sbase(const L &l1, const L &l2, const Integer &d) {
    //     return Fraction(d, omgB(l1, l1)) * Fraction(d, omgB(l2, l2));
    // }

    /**
     * @brief
     *
     * @param[in] l1
     * @param[in] l2
     * @param[in] d
     * @return auto
     */
    template <Projective_plane_coord2 L, typename T>
    inline constexpr auto sbase(const L& l1, const L& l2, const T& d) {
        using K = Value_type<L>;
        if constexpr (Integral<K>) {
            return Fraction<K>(d, dot1(l1, l1)) * Fraction<K>(d, dot1(l2, l2));
        } else {
            return (d * d) / (dot1(l1, l1) * dot1(l2, l2));
        }
    }

    /**
     * @brief
     *
     * @param[in] l1
     * @param[in] l2
     * @return auto
     */
    template <Projective_plane_coord2 L> inline constexpr auto spread(const L& l1, const L& l2) {
        return sbase(l1, l2, cross2(l1, l2));
    }

    /**
     * @brief
     *
     * @param[in] triangle
     * @return auto
     */
    template <Projective_plane_coord2 P>
    inline constexpr auto tri_quadrance(const Triple<P>& triangle) {
        const auto& [a1, a2, a3] = triangle;
        return std::array{quadrance(a2, a3), quadrance(a1, a3), quadrance(a1, a2)};
    }

    /**
     * @brief
     *
     * @param[in] trilateral
     * @return auto
     */
    template <Projective_plane_coord2 L>
    inline constexpr auto tri_spread(const Triple<L>& trilateral) {
        const auto& [a1, a2, a3] = trilateral;
        return std::array{spread(a2, a3), spread(a1, a3), spread(a1, a2)};
    }

    /**
     * @brief
     *
     * @param[in] l1
     * @param[in] l2
     * @return auto
     */
    template <Projective_plane_coord2 L> inline constexpr auto cross_s(const L& l1, const L& l2) {
        return sbase(l1, l2, dot1(l1, l2));
    }

#include <cmath>

    /**
     * @brief
     *
     * @param[in] a
     * @param[in] b
     * @return auto
     */
    template <Projective_plane_coord2 P> inline constexpr auto distance(const P& a, const P& b) {
        return std::sqrt(double(quadrance(a, b)));
    }

    /**
     * @brief
     *
     * @param[in] l
     * @param[in] m
     * @return auto
     */
    template <Projective_plane_coord2 L> inline constexpr auto angle(const L& l, const L& m) {
        return std::asin(std::sqrt(double(spread(l, m))));
    }

}  // namespace fun
