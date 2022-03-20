#pragma once

#include "fractions.hpp"
#include "proj_plane.hpp"

/** @file include/proj_plane.hpp
 *  This is a C++ Library header.
 */

/**
 @todo: projectivity >=
**/

namespace fun {

    /**
     * @brief
     *
     * @tparam K
     * @param[in] a
     * @param[in] b
     * @param[in] c
     * @param[in] d
     * @return auto
     */
    template <ring K> constexpr auto ratio_ratio(const K& a, const K& b, const K& c, const K& d) {
        if constexpr (Integral<K>) {
            return Fraction(a, b) / Fraction(c, d);
        } else {
            return (a * d) / (b * c);
        }
    }

    /**
     * @brief Cross Ratio
     *
     * @tparam P
     * @tparam L
     * @param[in] A point \in P
     * @param[in] B point \in P
     * @param[in] l line \in P
     * @param[in] m line \in P
     * @return cross ratio R(A,B;l,m)
     *
     * @todo rewrite by projecting to the y-axis first [:2]
     */
    template <typename P, typename L>
    requires Projective_plane<P, L>
    constexpr auto x_ratio(const P& A, const P& B, const L& l, const L& m) {
        return ratio_ratio(A.dot(l), A.dot(m), B.dot(l), B.dot(m));
    }

    /**
     * @brief
     *
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @param[in] D
     * @return constexpr auto
     */
    template <Projective_plane_coord2 P>
    constexpr auto R(const P& A, const P& B, const P& C, const P& D)

    {
        using K = Value_type<P>;
        if (cross0(A, B) != K(0)) {  // Project points to yz-plane
            return R0(A, B, C, D);
        }
        // Project points to xz-plane
        return R1(A, B, C, D);
    }

    /**
     * @brief
     *
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @param[in] D
     * @return constexpr auto
     */
    template <Projective_plane2 P>
    constexpr auto R(const P& A, const P& B, const P& C, const P& D) {
        const auto O = (C * D).aux();
        return x_ratio(A, B, O * C, O * D);
    }

    /**
     * @brief
     *
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @param[in] D
     * @return constexpr auto
     */
    template <Projective_plane_coord2 P>
    constexpr auto R0(const P& A, const P& B, const P& C, const P& D) {
        return ratio_ratio(cross0(A, C), cross0(A, D), cross0(B, C), cross0(B, D));
    }

    /**
     * @brief
     *
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @param[in] D
     * @return constexpr auto
     */
    template <Projective_plane_coord2 P>
    constexpr auto R1(const P& A, const P& B, const P& C, const P& D) {
        return ratio_ratio(cross1(A, C), cross1(A, D), cross1(B, C), cross1(B, D));
    }

}  // namespace fun
