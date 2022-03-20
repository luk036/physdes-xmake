#pragma once

#include <type_traits>  // std::is_base_of_v

#include "pg_common.hpp"
#include "proj_plane.hpp"
#include "proj_plane_concepts.h"
#include "proj_plane_measure.hpp"

namespace fun {

    /**
     * @brief
     *
     * @tparam _P
     * @tparam _L
     * @tparam Derived
     */
    template <typename _P, typename _L, template <typename P, typename L> class Derived>
    requires Projective_plane_prim<_P, _L>  // c++20 concept
    struct ck {
        using point_t = _P;
        using line_t = _L;

        using cDer = const Derived<_P, _L>;
        cDer& self = *static_cast<cDer*>(this);

        /**
         * @brief construct ck object
         *
         */
        constexpr ck() noexcept {
            static_assert(std::is_base_of_v<ck<_P, _L, Derived>, Derived<_P, _L>>);
        }

        /**
         * @brief is perpendicular
         *
         * @tparam L
         * @param[in] l
         * @param[in] m
         * @return true
         * @return false
         */
        constexpr auto is_perpendicular(const _L& l, const _L& m) const -> bool {
            return incident(m, self.perp(l));
        }

        /**
         * @brief altitude
         *
         * @param[in] p
         * @param[in] l
         * @return L
         */
        template <typename P, typename L = typename P::dual>
        requires Projective_plane_prim<P, L>  // c++20 concept
        constexpr auto altitude(const P& p, const L& l) const -> L { return p * self.perp(l); }

        /**
         * @brief altitudes of triangle
         *
         * @param[in] tri
         * @return std::tuple
         */
        template <Projective_plane_prim2 P>
        constexpr auto tri_altitude(const Triple<P>& tri) const {
            const auto [l1, l2, l3] = tri_dual(tri);
            const auto& [a1, a2, a3] = tri;

            return std::array{this->altitude(a1, l1), this->altitude(a2, l2),
                              this->altitude(a3, l3)};
        }

        /**
         * @brief ortho-center
         *
         * @param[in] tri
         * @return P
         */
        template <Projective_plane_prim2 P> constexpr auto orthocenter(const Triple<P>& tri) const
            -> P {
            const auto& [a1, a2, a3] = tri;

            const auto t1 = this->altitude(a1, a2 * a3);
            const auto t2 = this->altitude(a2, a1 * a3);
            return t1 * t2;
        }

        /**
         * @brief reflect
         *
         * @param[in] m
         * @return auto
         */
        template <Projective_plane P> auto reflect(const P& m) const {
            return involution{self.perp(m), P{m}};
        }

        /**
         * @brief measure of triangle
         *
         * @param[in] tri
         * @return constexpr auto
         */
        template <Projective_plane P> constexpr auto tri_measure(const Triple<P>& tri) const {
            const auto& [a1, a2, a3] = tri;

            return std::array{self.measure(a2, a3), self.measure(a1, a3), self.measure(a1, a2)};
        }

        /**
         * @brief quadrance between two points
         *
         * @param[in] p
         * @param[in] q
         * @return constexpr auto
         */
        constexpr auto quadrance(const _P& p, const _P& q) const { return self.measure(p, q); }

        /**
         * @brief spread between two lines
         *
         * @param[in] l
         * @param[in] m
         * @return constexpr auto
         */
        constexpr auto spread(const _L& l, const _L& m) const { return self.measure(l, m); }

        /**
         * @brief quadrances of triangle
         *
         * @param[in] triangle
         * @return constexpr auto
         */
        constexpr auto tri_quadrance(const Triple<_P>& triangle) const {
            return this->tri_measure(triangle);
        }

        /**
         * @brief spreads of triangle
         *
         * @param[in] trilateral
         * @return constexpr auto
         */
        constexpr auto tri_spread(const Triple<_L>& trilateral) const {
            return this->tri_measure(trilateral);
        }
    };

    /**
     * @brief check sine law
     *
     * @tparam Q_t
     * @tparam S_t
     * @param[in] Q
     * @param[in] S
     * @return true
     * @return false
     */
    template <ordered_ring Q_t>
    constexpr auto check_sine_law(const Triple<Q_t>& Q, const Triple<Q_t>& S) -> bool {
        const auto& [q1, q2, q3] = Q;
        const auto& [s1, s2, s3] = S;
        return (s1 * q2 == s2 * q1) && (s2 * q3 == s3 * q2);
    }

    /**
     * @brief Elliptic Plane
     *
     * @tparam P
     * @tparam P::dual
     */
    template <typename P, typename L = typename P::dual>
    requires Projective_plane_prim<P, L>  // c++20 concept
    struct ellck : ck<P, L, ellck> {
        /**
         * @brief perp (polar) of point
         *
         * @param[in] v
         * @return L
         */
        constexpr auto perp(const P& v) const -> L { return L(v); }

        /**
         * @brief perp (pole) of line
         *
         * @param[in] v
         * @return P
         */
        constexpr auto perp(const L& v) const -> P { return P(v); }

        /**
         * @brief measure between two objects
         *
         * @tparam _P
         * @param[in] a1
         * @param[in] a2
         * @return constexpr auto
         */
        template <Projective_plane2 _P> constexpr auto measure(const _P& a1, const _P& a2) const {
            return 1 - x_ratio(a1, a2, this->perp(a2), this->perp(a1));
        }
    };

    /**
     * @brief Hyperbolic Plane
     *
     * @tparam P
     * @tparam P::dual
     */
    template <typename P, typename L = typename P::dual>
    requires Projective_plane_prim<P, L>  // c++20 concept
    struct hyck : ck<P, L, hyck> {
        /**
         * @brief perp (polar) of point
         *
         * @param[in] v
         * @return L
         */
        constexpr auto perp(const P& v) const -> L { return L(v[0], v[1], -v[2]); }

        /**
         * @brief perp (pole) of line
         *
         * @param[in] v
         * @return P
         */
        constexpr auto perp(const L& v) const -> P { return P(v[0], v[1], -v[2]); }

        /**
         * @brief measure between two objects
         *
         * @tparam _P
         * @param[in] a1
         * @param[in] a2
         * @return constexpr auto
         */
        template <Projective_plane2 _P> constexpr auto measure(const _P& a1, const _P& a2) const {
            return 1 - x_ratio(a1, a2, this->perp(a2), this->perp(a1));
        }
    };

    /**
     * @brief
     *
     * @tparam K
     * @param[in] Q
     * @return constexpr auto
     */
    template <ordered_ring K> constexpr auto check_cross_TQF(const Triple<K>& Q) {
        const auto& [q1, q2, q3] = Q;
        return sq(q1 + q2 + q3) - 2 * (q1 * q1 + q2 * q2 + q3 * q3) - 4 * q1 * q2 * q3;
    }

    /**
     * @brief
     *
     * @tparam K
     * @param[in] S
     * @param[in] q3
     * @return constexpr auto
     */
    template <ordered_ring K> constexpr auto check_cross_law(const Triple<K>& S, const K& q3) {
        const auto& [s1, s2, s3] = S;
        return sq(s1 * s2 * q3 - (s1 + s2 + s3) + 2) - 4 * (1 - s1) * (1 - s2) * (1 - s3);
    }

}  // namespace fun
