// Arkadijs Slobodkins, 2023


#pragma once


namespace spp {


enum AlignmentFlag { Aligned, Unaligned };


namespace detail {


template <typename T, AlignmentFlag AF>
consteval int alignment_of() {
   constexpr int avx512_bytes = 64;
   return (AF == Aligned) ? avx512_bytes : 0;
}


} // namespace detail


} // namespace spp
