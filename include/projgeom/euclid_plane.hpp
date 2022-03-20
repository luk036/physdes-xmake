#pragma once

#include <type_traits>

#include "pg_common.hpp"   // import cross2, dot1
#include "proj_plane.hpp"  // import pg_point, involution, tri_func, quad_func, plucker
#include "proj_plane_concepts.h"

namespace fun {

    /**
     * @brief
     *
     * @param[in] l
     * @return auto
     */
    template <Projective_plane_coord2 L>  // // and requires p[i]
    constexpr auto fB(const L& l) -> typename L::dual {
        return {l[0], l[1], 0};
    }

    /**
     * @brief
     *
     * @param[in] l
     * @param[in] m
     * @return true
     * @return false
     */
    template <Projective_plane_coord2 L> constexpr auto is_perpendicular(const L& l, const L& m)
        -> bool {
        return dot1(l, m) == 0;
    }

    /**
     * @brief
     *
     * @param[in] l
     * @param[in] m
     * @return true
     * @return false
     */
    template <Projective_plane_coord2 L> constexpr auto is_parallel(const L& l, const L& m)
        -> bool {
        return cross2(l, m) == 0;
    }

    /**
     * @brief
     *
     * @param[in] a
     * @param[in] l
     * @return L
     */
    template <typename P, typename L>
    requires Projective_plane_coord<P, L>
    constexpr auto altitude(const P& a, const L& l) -> L { return a * fB(l); }

    /**
     * @brief
     *
     * @param[in] tri
     * @return auto
     */
    template <Projective_plane_coord2 P> constexpr auto tri_altitude(const Triple<P>& tri) {
        const auto& [a1, a2, a3] = tri;
        return std::array{altitude(a1, a2 * a3), altitude(a2, a3 * a1), altitude(a3, a1 * a2)};
    }

    /**
     * @brief
     *
     * @param[in] tri
     * @return P
     */
    template <Projective_plane_coord2 P> constexpr auto orthocenter(const Triple<P>& tri) -> P {
        const auto& [a1, a2, a3] = tri;
        const auto t1 = altitude(a1, a2 * a3);
        const auto t2 = altitude(a2, a1 * a3);
        return t1 * t2;
    }

    /**
     * @brief
     *
     * @param[in] m
     * @return auto
     */
    template <Projective_plane_coord2 L> constexpr auto reflect(const L& m) {
        return involution{m, fB(m)};
    }

    /**
     * @brief
     *
     * @param[in] a
     * @param[in] b
     * @return P
     */
    template <Projective_plane_coord2 P> constexpr auto midpoint(const P& a, const P& b) -> P {
        return plucker(b[2], a, a[2], b);
    }

    /**
     * @brief
     *
     * @param[in] tri
     * @return auto
     */
    template <Projective_plane_coord2 P> constexpr auto tri_midpoint(const Triple<P>& tri)
        -> Triple<P> {
        const auto& [a1, a2, a3] = tri;
        return {midpoint(a1, a2), midpoint(a2, a3), midpoint(a1, a3)};
    }

    /**
     * @brief
     *
     * @param[in] lda1
     * @param[in] mu1
     * @return P
     */
    template <Projective_plane_coord2 P>
    constexpr auto uc_point(const Value_type<P>& lda1, const Value_type<P>& mu1) {
        const auto lda2 = lda1 * lda1;
        const auto mu2 = mu1 * mu1;
        return P{lda2 - mu2, 2 * lda1 * mu1, lda2 + mu2};
    }

    /**
     * @brief Archimedes's function
     *
     * @tparam _Q
     * @param[in] a
     * @param[in] b
     * @param[in] c
     * @return auto
     */
    template <ordered_ring _Q> constexpr auto Ar(const _Q& a, const _Q& b, const _Q& c) {
        return 4 * a * b - sq(a + b - c);
    }

    /**
     * @brief Cyclic quadrilateral quadrea theorem
     *
     * @tparam _Q
     * @param[in] a
     * @param[in] b
     * @param[in] c
     * @param[in] d
     * @return auto
     */
    template <typename _Q> constexpr auto cqq(const _Q& a, const _Q& b, const _Q& c, const _Q& d)
        -> std::array<_Q, 2> {
        const auto t1 = 4 * a * b;
        const auto t2 = 4 * c * d;
        auto m = (t1 + t2) - sq(a + b - c - d);
        auto p = m * m - 4 * t1 * t2;
        return {std::move(m), std::move(p)};
    }

    /**
     * @brief
     *
     * @tparam _Q
     * @param[in] quad
     * @return auto
     */
    template <typename T> constexpr auto Ptolemy(const T& quad) -> bool {
        const auto& [Q12, Q23, Q34, Q14, Q13, Q24] = quad;
        using _K = decltype(Q12);
        return Ar(Q12 * Q34, Q23 * Q14, Q13 * Q24) == _K(0);
    }

}  // namespace fun
