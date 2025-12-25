// Arkadijs Slobodkins, 2023


#pragma once


#include <Eigen/Dense>

#include "../StrictCommon/strict_common.hpp"
#include "../derived1D.hpp"
#include "../derived2D.hpp"


namespace spp { namespace eigen {


namespace detail {


template <typename Base>
consteval Eigen::AlignmentType eigen_align() {
   constexpr int align = Base::alignment();
   using namespace Eigen;
   // AlignmentType is used to avoid ambiguity between enums in Eigen and strictpp.
   if(align == 0) {
      return AlignmentType::Unaligned;
   } else if(align == 8) {
      return AlignmentType::Aligned8;
   } else if(align == 16) {
      return AlignmentType::Aligned16;
   } else if(align == 32) {
      return AlignmentType::Aligned32;
   } else if(align == 64) {
      return AlignmentType::Aligned64;
   } else if(align == 128) {
      return AlignmentType::Aligned128;
   } else {
      return AlignmentType::Unaligned;
   }
}


template <typename T, typename U>
struct CopyConstTo {
   using value_type = U;
};


template <typename T, typename U>
struct CopyConstTo<const T, U> {
   using value_type = const U;
};


}  // namespace detail


template <typename Base>
   requires TwoDimOwnerType<RemoveRef<Base>>
auto map(Base&& A) {
   using namespace Eigen;
   using builtin_type = BuiltinTypeOf<Base>;
   using base = RemoveRef<Base>;

   if constexpr(base::is_fixed()) {
      using matrix_type = detail::CopyConstTo<
          base,
          Matrix<builtin_type, base::rows().val(), base::cols().val(), RowMajor>>::value_type;

      return Map<matrix_type, detail::eigen_align<base>()>{A.blas_data()};
   } else {
      using matrix_type
          = detail::CopyConstTo<base, Matrix<builtin_type, Dynamic, Dynamic, RowMajor>>::value_type;

      return Map<matrix_type, detail::eigen_align<base>()>{
          A.blas_data(), A.rows().val(), A.cols().val()};
   }
}


}}  // namespace spp::eigen

