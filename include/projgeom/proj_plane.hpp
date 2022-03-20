#pragma once

#include <array>
#include <cassert>
#include <tuple>

#include "proj_plane_concepts.h"

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
     * @param[in] p
     * @param[in] l
     * @return true
     * @return false
     */
    template <typename P, typename L>
    requires Projective_plane<P, L>
    constexpr auto incident(const P& p, const L& l) -> bool { return p.dot(l) == Value_type<P>(0); }

    /**
     * @brief Coincident
     *
     * @tparam[in] L Line
     * @tparam[in] Args points
     * @return true if points are conincident (on a line l)
     * @return false otherwise
     */
    template <typename L, typename... Args>
    requires(Projective_plane_prim<L, Args>&&...) constexpr auto coincident(const L& l,
                                                                            const Args&... r)
        -> bool {
        return (incident(r, l) && ...);
    }

    template <typename P> using Triple = std::array<P, 3>;

    /**
     * @brief
     *
     * @param[in] tri
     * @return auto
     */
    template <Projective_plane_prim2 P> constexpr auto tri_dual(const Triple<P>& tri)

    {
        const auto& [a1, a2, a3] = tri;
        assert(!coincident(a2 * a3, a1));
        return std::array{a2 * a3, a1 * a3, a1 * a2};
    }

    /**
     * @brief
     *
     * @param[in] func
     * @param[in] tri
     * @return auto
     */
    template <Projective_plane_prim2 P, typename Fn>
    constexpr auto tri_func(Fn&& func, const Triple<P>& tri)

    {
        const auto& [a1, a2, a3] = tri;
        return std::array{func(a2, a3), func(a1, a3), func(a1, a2)};
    }

    /**
     * @brief return whether two triangles are perspective
     *
     * @param[in] tri1
     * @param[in] tri2
     * @return true
     * @return false
     */
    template <Projective_plane_prim2 P>
    constexpr auto persp(const Triple<P>& tri1, const Triple<P>& tri2) -> bool {
        const auto& [A, B, C] = tri1;
        const auto& [D, E, F] = tri2;
        const auto O = (A * D) * (B * E);
        return incident(O, C * F);
    }

    /**
     * @brief
     *
     * @tparam P
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @return constexpr P
     */
    template <Projective_plane2 P> constexpr auto harm_conj(const P& A, const P& B, const P& C)
        -> P {
        assert(incident(A * B, C));
        const auto lC = C * (A * B).aux();
        return plucker(B.dot(lC), A, A.dot(lC), B);
    }

    /**
     * @brief
     *
     * @tparam P
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @return constexpr P
     */
    template <Projective_plane_generic2 _P>
    constexpr auto harm_conj(const _P& A, const _P& B, const _P& C) -> _P {
        assert(incident(A * B, C));
        const auto AB = A * B;
        const auto P = AB.aux();
        const auto R = P.aux2(C);
        const auto S = (A * R) * (B * P);
        const auto Q = (B * R) * (A * P);
        return (Q * S) * AB;
    }

    /**
     * @brief
     *
     * @param[in] A
     * @param[in] B
     * @param[in] C
     * @param[in] D
     * @return constexpr auto
     *
     */
    template <Projective_plane2 P>
    constexpr auto is_harmonic(const P& A, const P& B, const P& C, const P& D) -> bool {
        return harm_conj(A, B, C) == D;
    }

    /**
     * @brief
     *
     * @tparam P
     * @tparam L
     */
    template <typename P, typename L>
    requires Projective_plane<P, L>
    class involution {
        using K = Value_type<P>;

      private:
        L _m;
        P _o;
        K _c;

      public:
        /**
         * @brief Construct a new involution object
         *
         * @param[in] m
         * @param[in] o
         */
        constexpr involution(L m, P o)  // input mirror and center
            : _m{std::move(m)}, _o{std::move(o)}, _c{_m.dot(_o)} {}

        /**
         * @brief
         *
         * @param[in] p
         * @return P
         */
        constexpr auto operator()(const P& p) const -> P {
            return plucker(this->_c, p, K(-2 * p.dot(this->_m)), this->_o);
        }

        /**
         * @brief
         *
         * @param[in] p
         * @return P
         */
        constexpr auto operator()(const L& l) const -> L {
            return plucker(this->_c, l, K(-2 * l.dot(this->_o)), this->_m);
        }
    };

    /**
     * @brief
     *
     * @tparam P
     * @tparam L
     */
    template <typename P, typename L>
    requires Projective_plane_generic<P, L>
    class involution_generic {
      private:
        L _m;
        P _o;

      public:
        /**
         * @brief Construct a new involution object
         *
         * @param[in] m
         * @param[in] o
         */
        constexpr involution_generic(L m, P o)  // input mirror and center
            : _m{std::move(m)}, _o{std::move(o)} {}

        /**
         * @brief
         *
         * @param[in] p
         * @return P
         */
        constexpr auto operator()(const P& p) const -> P {
            auto po = p * this->_o;
            auto B = po * this->_m;
            return harm_conj(this->_o, B, p);
        }
    };

    /**
     * @brief Check Pappus Theorem
     *
     * @tparam P
     * @tparam L
     * @param[in] co1
     * @param[in] co2
     */
    template <Projective_plane_prim2 P>
    void check_pappus(const Triple<P>& co1, const Triple<P>& co2)

    {
        const auto& [A, B, C] = co1;
        const auto& [D, E, F] = co2;

        const auto G = (A * E) * (B * D);
        const auto H = (A * F) * (C * D);
        const auto I = (B * F) * (C * E);
        assert(coincident(G, H, I));
    }

    /**
     * @brief
     *
     * @param[in] tri1
     * @param[in] tri2
     */
    template <Projective_plane_prim2 P>
    void check_desargue(const Triple<P>& tri1, const Triple<P>& tri2) {
        const auto trid1 = tri_dual(tri1);
        const auto trid2 = tri_dual(tri2);
        const auto b1 = persp(tri1, tri2);
        const auto b2 = persp(trid1, trid2);
        assert((b1 && b2) || (!b1 && !b2));
    }

}  // namespace fun
