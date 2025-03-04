// Arkadijs Slobodkins, 2023


#pragma once


#include "../StrictCommon/config.hpp"
#include "../StrictCommon/strict_traits.hpp"
#include "../StrictCommon/strict_val.hpp"
#include "array_auxiliary.hpp"
#include "array_traits.hpp"


namespace spp {


enum Storage { RowMajor, ColMajor };


namespace detail {


template <BaseType Base1, BaseType Base2, BaseType... BArgs>
   requires(same_dimension_b<Base1, Base2, BArgs...>())
STRICT_NODISCARD_CONSTEXPR StrictBool same_storage() {
   // Always true for one-dimensional array types.
   if constexpr(OneDimBaseType<Base1>) {
      return true_sb;
   } else {
      StrictBool b{Base1::storage() == Base2::storage()};
      if constexpr(sizeof...(BArgs) > 0) {
         return b && same_storage<Base2, BArgs...>();
      }
      return b;
   }
}


template <BaseType Base1, BaseType Base2, BaseType... BArgs>
   requires(same_dimension_b<Base1, Base2, BArgs...>())
STRICT_NODISCARD_CONSTEXPR bool same_storage_b() {
   return bool{same_storage<Base1, Base2, BArgs...>()};
}


}  // namespace detail


}  // namespace spp

