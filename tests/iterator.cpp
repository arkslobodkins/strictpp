#include <cstdlib>
#include <vector>

#include "test.hpp"


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


////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
   TEST_NON_TYPE(iterator_ops);
   return EXIT_SUCCESS;
}

