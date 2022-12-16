// for example, compile using recent version of gcc or recent intel compilers from intel-oneAPI
// g++-12.2 -std=c++20 example.cpp
// dpcpp -std=c++20 example.cpp
// icpx -std=c++20 example.cpp

// to use quadruple precision, for example
// g++-12.2 -std=gnu++20 example.cpp -lquadmath

// to enable debugging and range checking add -DSTRICT_DEBUG_ON
// to enable division by 0 checking add -DSTRICT_DIVISION_ON

#include <type_traits>
#include <valarray>
#if __cplusplus < 202002L
#error requires c++20 or higher
#else

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <numeric>

#include "strict_array.hpp"
using namespace strict_array;

template<typename F>
void derivative(Array<float64> & A, F f)
{
   constexpr StrictVal<float64> h = 0.000'001;
   constexpr StrictVal<float64> two_h = 2. * h;
   for(decltype(A.size()) i = 0; i < A.size(); ++i)
      A[i] = ( f(A[i] + h) - f(A[i] - h) ) / two_h;
}

template<typename T>
StrictVal<T> sign(StrictVal<T> x)
{
   return x > T(0) ? T(1) : T(-1);
}

int main()
{
   using std::cout, std::endl;

   // 1. Compute derivative of x * exp(x) and multiply by 2.
   // Expression 2. * A is evaluated on the fly twice(once when
   // printing and once when computing infinity norm).

   Array<float64> A{-1., -2., -3., -4., -5.};
   auto func = [](auto x) { return x * exp(x); };
   derivative(A, func);
   cout << 2. * A;
   cout << "infinity norm = " << norm_inf(2. * A) << endl << endl;

   // 2. find all values in [-0.5, 0.5] and map them to
   // [-1, -0.5] and [0.5, 1], depending on the sign.
   // Note that if any other suffix or no suffix were
   // used instead of "F", the code would not compile,
   // because elements of Array are of type StrictVal<float32>,
   // i.e. StrictVal<float>.

   auto n = 100'000LL;
   Array B = array_random<float32>(n, -1.F, 1.F);
   auto half_range = B.within_range(-0.5F, 0.5F);
   for(auto x_ptr : half_range) *x_ptr += 0.5F * sign(*x_ptr);
   for(auto x : B) assert(abs(x) >= 0.5F && abs(x) <= 1.F); // test mapping

   // 3. fill C with 0, 1, 2, ..., then multiply C by 2
   // (which gives expression template) and
   // then use constant iterator of expression template to
   // sum every second element of 0, 2, 4, .., i.e.
   // sum of 0, 4, 8 ...

   Array<int> C(9);
   std::iota(C.begin(), C.end(), 0);
   StrictVal<int> sum{};
   auto expr = 2 * C;
   for(auto it = expr.begin(); it < expr.end(); it += 2)
      sum += *it;
   cout << "sum = " << sum << endl; // 0 + 4 + 8 + 12 + 16 is hopefully 40

   return EXIT_SUCCESS;
}

#endif
