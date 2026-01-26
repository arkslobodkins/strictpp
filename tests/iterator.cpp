#include "test.hpp"

#include <cstdlib>
#include <iterator>
#include <utility>


using namespace spp;
using namespace spp::place;


////////////////////////////////////////////////////////////////////////////////////////////////////
void iterator_ops() {
   const Array1D<int> A = sequence<int>(10);
   auto it = A.begin();

   ASSERT(*it == 0_si);
   ASSERT(*(++it) == 1_si);
   ASSERT(*(--it) == 0_si);

   ++it;
   ASSERT(*it == 1_si);
   --it;
   ASSERT(*it == 0_si);

   it += 2;
   ASSERT(*it == 2_si);
   it -= 2;
   ASSERT(*it == 0_si);

   ASSERT(*(it + 2) == 2_si);

   it += 2;
   ASSERT(*(it - 2) == 0_si);
   it -= 2;
   ASSERT(((it + 2) - it) == 2);

   ASSERT(it[2] == 2_si);
   ASSERT(it->val() == 0);

   ASSERT(it == A.begin());
   ASSERT(it != A.end());
   ASSERT(it < A.end());
   ASSERT(it <= A.end());
   ASSERT(it + 1 > A.begin());
   ASSERT(it + 1 >= A.begin());

   static_assert(SameAs<decltype(it)::difference_type, long int>);
   static_assert(SameAs<decltype(it)::value_type, StrictInt>);
   static_assert(std::contiguous_iterator<decltype(it)>);
   static_assert(!std::contiguous_iterator<decltype(A.view1D().lval().begin())>);
   static_assert(std::random_access_iterator<decltype(A.view1D().lval().begin())>);
}


void row_iterator() {
   Array2D<int> A = sequence<int>(6).view2D(3, 2);
   auto rit = RowIt{A};
   ASSERT(equal(*rit.begin(), {0_si, 1_si}));
   ASSERT(equal(*rit.cbegin(), {0_si, 1_si}));
   ASSERT(equal(*rit.rbegin(), {4_si, 5_si}));
   ASSERT(equal(*rit.crbegin(), {4_si, 5_si}));

   ASSERT(equal(*(rit.begin() + 1), {2_si, 3_si}));

   auto even_rows = A(place::even, place::all);
   auto even_rit = RowIt{even_rows};
   ASSERT(equal(*(even_rit.begin()), {0_si, 1_si}));
   ASSERT(equal(*(even_rit.begin() + 1), {4_si, 5_si}));

   static_assert(SameAs<decltype(rit.begin())::value_type, decltype(A.row(0))>);
   static_assert(SameAs<decltype(rit.begin())::difference_type, long int>);
}


void col_iterator() {
   Array2D<int> A = sequence<int>(6).view2D(3, 2);
   auto cit = ColIt{A};
   ASSERT(equal(*cit.begin(), {0_si, 2_si, 4_si}));
   ASSERT(equal(*cit.cbegin(), {0_si, 2_si, 4_si}));
   ASSERT(equal(*cit.rbegin(), {1_si, 3_si, 5_si}));
   ASSERT(equal(*cit.crbegin(), {1_si, 3_si, 5_si}));

   ASSERT(equal(*(cit.begin() + 1), {1_si, 3_si, 5_si}));

   static_assert(SameAs<decltype(cit.begin())::value_type, decltype(A.col(0))>);
   static_assert(SameAs<decltype(cit.begin())::difference_type, long int>);
}


void index_range_iterator() {
   IndexRange2D range{3, 2};
   ASSERT((*range.begin()).first == 0_sl);
   ASSERT((*range.begin()).second == 0_sl);

   auto it = range.begin();
   ASSERT((*it == std::pair{0_sl, 0_sl}));
   ASSERT((*++it == std::pair{0_sl, 1_sl}));
   ASSERT((*++it == std::pair{1_sl, 0_sl}));
   ASSERT((*++it == std::pair{1_sl, 1_sl}));
   ASSERT((*++it == std::pair{2_sl, 0_sl}));
   ASSERT((*++it == std::pair{2_sl, 1_sl}));
   ASSERT((++it == range.end()));
}


void iterator_fail() {
   Array2D<int> A1 = sequence<int>(4).view2D(2, 2);
   auto rit1 = RowIt{A1};
   REQUIRE_THROW(*(rit1.begin() + 3));
   REQUIRE_THROW(rit1.begin()[3]);

   auto A2 = A1;
   auto rit2 = RowIt{A2};
   REQUIRE_THROW(void(rit1.begin() == rit2.begin()));
   REQUIRE_THROW(void(rit1.begin() != rit2.begin()));
   REQUIRE_THROW(void(rit1.begin() < rit2.begin()));
   REQUIRE_THROW(void(rit1.begin() <= rit2.begin()));
   REQUIRE_THROW(void(rit1.begin() > rit2.begin()));
   REQUIRE_THROW(void(rit1.begin() >= rit2.begin()));

   decltype(rit1.begin()) it3{};
   REQUIRE_THROW(++it3);
   REQUIRE_THROW(--it3);
   REQUIRE_THROW(it3++);
   REQUIRE_THROW(it3--);
   REQUIRE_THROW(it3 - it3);
   REQUIRE_THROW(it3[0]);
   REQUIRE_THROW(*it3);

   REQUIRE_THROW((it3 == it3));
   REQUIRE_THROW((it3 != it3));
   REQUIRE_THROW((it3 < it3));
   REQUIRE_THROW((it3 <= it3));
   REQUIRE_THROW((it3 > it3));
   REQUIRE_THROW((it3 >= it3));

   IndexRange2D range1{3, 2};
   IndexRange2D range2{3, 3};
   IndexRange2D range3{0, 0};
   REQUIRE_THROW(void(range1.begin() == range2.begin()));
   REQUIRE_THROW(void(range1.begin() != range2.begin()));
   REQUIRE_THROW(*range3.begin());
   ASSERT(range3.begin() == range3.end());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
   TEST_NON_TYPE(iterator_ops);
   TEST_NON_TYPE(row_iterator);
   TEST_NON_TYPE(col_iterator);
   TEST_NON_TYPE(index_range_iterator);
   TEST_NON_TYPE(iterator_fail);
   return EXIT_SUCCESS;
}
