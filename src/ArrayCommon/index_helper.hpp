//// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>
#include <vector>

#include "../StrictCommon/auxiliary_types.hpp"
#include "../StrictCommon/common_traits.hpp"
#include "../StrictCommon/config.hpp"
#include "../StrictCommon/strict_literals.hpp"
#include "../StrictCommon/strict_traits.hpp"
#include "../StrictCommon/strict_val.hpp"
#include "array_traits.hpp"


namespace spp::detail {


template <typename T>
class IndexWrapper {
public:
   IndexWrapper(T i) = delete;
};


template <typename T>
   requires SignedInteger<T> || SameAs<T, StrictInt> || SameAs<T, StrictLong>
         || SameAs<T, ImplicitInt>
class IndexWrapper<T> {
   ImplicitInt i_;

public:
   STRICT_CONSTEXPR_INLINE explicit IndexWrapper(T i) : i_{i} {
   }

   STRICT_CONSTEXPR_INLINE index_t get([[maybe_unused]] OneDimBaseType auto const& A) const {
      return i_.get();
   }

   STRICT_CONSTEXPR_INLINE index_t get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      return i_.get();
   }

   STRICT_CONSTEXPR_INLINE index_t get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      return i_.get();
   }
};


template <typename T>
   requires SameAs<T, Last>
class IndexWrapper<T> {
   index_t i_;

public:
   STRICT_CONSTEXPR_INLINE explicit IndexWrapper(T lst) : i_{lst.get()} {
   }

   STRICT_CONSTEXPR_INLINE index_t get(OneDimBaseType auto const& A) const {
      return A.size() - i_ - 1_sl;
   }

   STRICT_CONSTEXPR_INLINE index_t get_row(TwoDimBaseType auto const& A) const {
      return A.rows() - i_ - 1_sl;
   }

   STRICT_CONSTEXPR_INLINE index_t get_col(TwoDimBaseType auto const& A) const {
      return A.cols() - i_ - 1_sl;
   }
};


STRICT_CONSTEXPR_INLINE index_t index_helper(OneDimBaseType auto const& A, auto i) {
   return IndexWrapper{i}.get(A);
}


STRICT_CONSTEXPR_INLINE index_t index_row_helper(TwoDimBaseType auto const& A, auto i) {
   return IndexWrapper{i}.get_row(A);
}


STRICT_CONSTEXPR_INLINE index_t index_col_helper(TwoDimBaseType auto const& A, auto j) {
   return IndexWrapper{j}.get_col(A);
}


STRICT_CONSTEXPR_INLINE std::pair<index_t, index_t> index_row_col_helper(
    TwoDimBaseType auto const& A, auto i, auto j) {
   return {IndexWrapper{i}.get_row(A), IndexWrapper{j}.get_col(A)};
}


STRICT_CONSTEXPR_INLINE std::pair<index_t, index_t> index_map_one_to_two_dim(
    TwoDimBaseType auto const& A, auto i) {
   auto r = i.get() / A.cols();
   auto c = i.get() % A.cols();
   return {r, c};
}


}  // namespace spp::detail


namespace spp {


STRICT_CONSTEXPR_INLINE auto implicit_vector_sequence(ImplicitInt size,
                                                      ImplicitInt start = Strict<int>{},
                                                      ImplicitInt incr = Strict<int>{1}) {
   ASSERT_STRICT_DEBUG(size.get() > -1_sl);
   std::vector<ImplicitInt> indexes(to_size_t(size.get()));
   for(auto i = 0_sl; i < size.get(); ++i) {
      indexes[to_size_t(i)] = start.get() + i * incr.get();
   }
   return indexes;
}


template <Integer T>
STRICT_CONSTEXPR_INLINE auto implicit_vector_sequence(Size size, Start<T> start = Start<T>{},
                                                      Incr<T> incr = Incr{T(1)}) {
   return implicit_vector_sequence(size.get(), start.get(), incr.get());
}


}  // namespace spp

