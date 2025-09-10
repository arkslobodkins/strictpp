// Arkadijs Slobodkins, 2023


#pragma once


namespace spp {


enum AlignmentFlag { Aligned, Unaligned };


namespace detail {


// Currently aligns to 512 bits, regardless of the type T.
template <typename T, AlignmentFlag AF>
consteval int alignment_of() {
   // Align to 512 bit boundary for AVX-512.
   return (AF == Aligned) ? 64 : 0;
}


}  // namespace detail


}  // namespace spp

