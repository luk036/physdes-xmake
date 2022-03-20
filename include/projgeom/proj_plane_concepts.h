/** @file include/proj_plane_concepts.h
 *  This is a C++ Library header.
 */

#pragma once

#include "common_concepts.h"

/**
 * @todo: projectivity >=
 */

namespace fun {

    /**
     * @brief Projective plane Concept (half)
     *
     * @tparam P Point
     * @tparam L Line
     */
    template <class P, class L>
    concept Projective_plane_prim_h
        = STD_ALT::equality_comparable<P> && requires(const P& p, const P& q, const L& l) {
        { incident(p, l) } -> STD_ALT::convertible_to<bool>;  // incidence
        { p* q } -> STD_ALT::convertible_to<L>;               // join or meet
        // { p.aux() } -> STD_ALT::convertible_to<L>; // line not incident with p
        // { p.aux2(q) } -> STD_ALT::convertible_to<P>; // point r on p * q, r != p and r != q
    };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam P Point
     * @tparam L Line
     */
    template <class P, class L = typename P::dual>
    concept Projective_plane_prim = Projective_plane_prim_h<P, L> && Projective_plane_prim_h<L, P>;

    /**
     * @brief Shorthand Notation of Projective_plane
     *
     * @tparam P Point
     */
    template <class P>
    concept Projective_plane_prim2
        = Projective_plane_prim<std::remove_reference_t<P>>;  // Make the compiler
                                                              // happy

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam P Point
     * @tparam L Line
     */
    template <class P, class L = typename P::dual>
    concept Projective_plane_generic_h
        = Projective_plane_prim_h<P, L> && requires(const P& p, const P& q) {
        { p.aux() } -> STD_ALT::convertible_to<L>;    // line not incident with p
        { p.aux2(q) } -> STD_ALT::convertible_to<P>;  // point r on p * q, r != p and r != q
    };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam P Point
     * @tparam L Line
     */
    template <class P, class L = typename P::dual>
    concept Projective_plane_generic
        = Projective_plane_generic_h<P, L> && Projective_plane_generic_h<L, P>;

    /**
     * @brief Shorthand Notation of Projective_plane
     *
     * @tparam P Point
     */
    template <class P>
    concept Projective_plane_generic2
        = Projective_plane_generic<std::remove_reference_t<P>>;  // Make the compiler
                                                                 // happy

    /**
     * @brief Projective plane Concept (half)
     *
     * @tparam P Point
     * @tparam L Line
     */
    template <class P, class L>
    concept Projective_plane_h
        = STD_ALT::equality_comparable<P> && requires(const P& p, const P& q, const L& l,
                                                      const Value_type<P>& a) {
        typename Value_type<P>;
        // { P(p) } -> P; // copyable
        // { incident(p, l) } -> bool; // incidence
        { p* q } -> STD_ALT::convertible_to<L>;                  // join or meet
        { p.dot(l) } -> STD_ALT::convertible_to<Value_type<P>>;  // for measurement
        { p.aux() } -> STD_ALT::convertible_to<L>;               // line not incident with p
        { plucker(a, p, a, q) } -> STD_ALT::convertible_to<P>;   // module computation
    };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam P Point
     * @tparam L Line
     */
    template <class P, class L = typename P::dual>
    concept Projective_plane = Projective_plane_h<P, L> && Projective_plane_h<L, P>;

    /*
    axiom(P p, P q, P r, L l) {
      l == L{p, q} => I(p, l) and I(q, l);
    }
    */

    /**
     * @brief Shorthand Notation of Projective_plane
     *
     * @tparam P Point
     */
    template <class P>
    concept Projective_plane2
        = Projective_plane<std::remove_reference_t<P>>;  // Make the compiler happy

    /**
     * @brief Projective plane Concept (half)
     *
     * @tparam P Point
     * @tparam L Line
     */
    template <class P, class L>
    concept Projective_plane_coord_h
        = Projective_plane_h<P, L> && requires(const P& p, size_t idx) {
        typename Value_type<P>;

        { p[idx] } -> STD_ALT::convertible_to<Value_type<P>>;  // for coordinate acess
    };

    /**
     * @brief Projective plane Concept (full)
     *
     * @tparam P Point
     * @tparam L Line
     */
    template <class P, class L = typename P::dual>
    concept Projective_plane_coord
        = Projective_plane_coord_h<P, L> && Projective_plane_coord_h<L, P>;

    /**
     * @brief Shorthand Notation of Projective_plane
     *
     * @tparam P Point
     */
    template <class P>
    concept Projective_plane_coord2
        = Projective_plane_coord<std::remove_reference_t<P>>;  // Make the compiler
                                                               // happy

}  // namespace fun
