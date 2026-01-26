
#include "test.hpp"

#include <cstdlib>
#include <vector>


using namespace spp;
using namespace spp::place;


template <bool is_array>
struct ArrayOrView1D {
   using type = Array1D<int>;
};


template <>
struct ArrayOrView1D<false> {
   using type = decltype(sequence<int>(1));
};


template <bool is_array>
struct ArrayOrView2D {
   using type = Array2D<int>;
};


template <>
struct ArrayOrView2D<false> {
   using type = decltype(sequence<int>(1).view2D(1, 1));
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <bool is_array>
void run_seqn1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(seqN{10, 0}), {}));

   ASSERT(equal(A(seqN{0, 3}), {0_si, 1_si, 2_si}));
   ASSERT(equal(A(seqN{0, 3, 1}), {0_si, 1_si, 2_si}));
   ASSERT(equal(A(seqN{2, 3, -1}), {2_si, 1_si, 0_si}));

   ASSERT(equal(A(seqN{0, 3, 2}), {0_si, 2_si, 4_si}));
   ASSERT(equal(A(seqN{4, 3, -2}), {4_si, 2_si, 0_si}));
   ASSERT(equal(A(seqN{1, 3, 2}), {1_si, 3_si, 5_si}));
}


template <bool is_array>
void run_seq1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(seq{0, 2}), {0_si, 1_si, 2_si}));
   ASSERT(equal(A(seq{0, 2, 1}), {0_si, 1_si, 2_si}));
   ASSERT(equal(A(seq{2, 0, -1}), {2_si, 1_si, 0_si}));

   ASSERT(equal(A(seq{0, 4, 2}), {0_si, 2_si, 4_si}));
   ASSERT(equal(A(seq{4, 0, -2}), {4_si, 2_si, 0_si}));
   ASSERT(equal(A(seq{1, 5, 2}), {1_si, 3_si, 5_si}));
}


template <bool is_array>
void run_skip1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(skipN{3}), {0_si, 3_si, 6_si, 9_si}));
   ASSERT(equal(A(skipN{4}), {0_si, 4_si, 8_si}));
   ASSERT(equal(A(skipN{20}), {0_si}));
   ASSERT(equal(A(skipN{1}), A));
}


template <bool is_array>
void run_first1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(firstN{5}), {0_si, 1_si, 2_si, 3_si, 4_si}));
   ASSERT(equal(A(firstN{0}), {}));
}


template <bool is_array>
void run_last1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(lastN{5}), {5_si, 6_si, 7_si, 8_si, 9_si}));
   ASSERT(equal(A(lastN{0}), {}));
}


template <bool is_array>
void run_even1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(even), {0_si, 2_si, 4_si, 6_si, 8_si}));
}


template <bool is_array>
void run_odd1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(odd), {1_si, 3_si, 5_si, 7_si, 9_si}));
}


template <bool is_array>
void run_reverse1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(5);
   ASSERT(equal(A(reverse), {4_si, 3_si, 2_si, 1_si, 0_si}));
}


template <bool is_array>
void run_vector1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(std::vector<ImplicitInt>{}), {}));
   ASSERT(equal(A(std::vector<ImplicitInt>{0, 1, 2}), {0_si, 1_si, 2_si}));
   ASSERT(equal(A(std::vector<ImplicitInt>{0, 2, 4}), {0_si, 2_si, 4_si}));
   ASSERT(equal(A(std::vector<ImplicitInt>{4, 2, 0}), {4_si, 2_si, 0_si}));
   ASSERT(
      equal(A(std::vector<ImplicitInt>{4, 4, 3, 3, 5, 5}), {4_si, 4_si, 3_si, 3_si, 5_si, 5_si}));
}


template <bool is_array>
void run_list1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A({}), {}));
   ASSERT(equal(A({0, 1, 2}), {0_si, 1_si, 2_si}));
   ASSERT(equal(A({0, 2, 4}), {0_si, 2_si, 4_si}));
   ASSERT(equal(A({4, 2, 0}), {4_si, 2_si, 0_si}));
   ASSERT(equal(A({4, 4, 3, 3, 5, 5}), {4_si, 4_si, 3_si, 3_si, 5_si, 5_si}));
}


template <bool is_array>
void run_complement1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(place::complement{1, 2, 3, 4, 5}), {0_si, 6_si, 7_si, 8_si, 9_si}));
   ASSERT(equal(A(place::complement{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), {}));
}


template <bool is_array>
void run_empty1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(0);
   ASSERT(equal(A(seqN{0, 0}), {}));
   ASSERT(equal(A(skipN{1}), {}));
   ASSERT(equal(A(firstN{0}), {}));
   ASSERT(equal(A(lastN{0}), {}));
   ASSERT(equal(A(even), {}));
   ASSERT(equal(A(odd), {}));
   ASSERT(equal(A(reverse), {}));
   ASSERT(equal(A({}), {}));
}


template <bool is_array>
void run_fail1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   REQUIRE_THROW(A(seqN{First{-1}, Size{1}, Stride{0}}));
   REQUIRE_THROW(A(seqN{First{1}, Size{10}, Stride{1}}));
   REQUIRE_THROW(A(seqN{First{0}, Size{2}, Stride{10}}));

   REQUIRE_THROW(A(seq{First{0}, Endmost{10}, Stride{1}}));
   REQUIRE_THROW(A(seq{First{-1}, Endmost{1}, Stride{1}}));
   REQUIRE_THROW(A(seq{First{0}, Endmost{1}, Stride{-1}}));

   REQUIRE_THROW(A(skipN{0}));
   REQUIRE_THROW(A(skipN{-1}));
   REQUIRE_THROW(A(firstN{11}));
   REQUIRE_THROW(A(lastN{11}));

   REQUIRE_THROW(A({-1}));
   REQUIRE_THROW(A(complement{-1}));

   REQUIRE_NOT_THROW(A(seqN{First{0}, Size{0}, Stride{100}}));
   REQUIRE_NOT_THROW(A(seqN{First{100}, Size{0}, Stride{100}}));

   REQUIRE_NOT_THROW(A(seq{First{0}, Endmost{0}, Stride{0}}));
   REQUIRE_NOT_THROW(A(seq{First{0}, Endmost{0}, Stride{100}}));

   REQUIRE_NOT_THROW(A(skipN{100}));
}


template <bool is_array>
void run_compound1D() {
   typename ArrayOrView1D<is_array>::type A = sequence<int>(10);
   ASSERT(equal(A(seqN{0, 3, 1})(reverse), {2_si, 1_si, 0_si}));
   ASSERT(equal(A(even)(even), {0_si, 4_si, 8_si}));
   ASSERT(equal(A(even)({0, 1, 3}), {0_si, 2_si, 6_si}));
   ASSERT(equal(A(seqN{0, 3, 1})(reverse)(odd), {1_si}));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <bool is_array>
void run_seqn2D() {
   typename ArrayOrView2D<is_array>::type A = sequence<int>(9).view2D(3, 3);
   ASSERT(equal(A(
                   seqN{
                      0, 3, 1
   },
                   seqN{0, 2, 1}),
                {{0_si, 1_si}, {3_si, 4_si}, {6_si, 7_si}}));
}


template <bool is_array>
void run_seq2D() {
   typename ArrayOrView2D<is_array>::type A = sequence<int>(9).view2D(3, 3);
   ASSERT(equal(A(
                   seq{
                      0, 2, 1
   },
                   seq{0, 1, 1}),
                {{0_si, 1_si}, {3_si, 4_si}, {6_si, 7_si}}));
}


template <bool is_array>
void run_skip2D() {
   typename ArrayOrView2D<is_array>::type A = sequence<int>(9).view2D(3, 3);
   ASSERT(equal(A(
                   skipN{
                      2
   },
                   skipN{2}),
                {{0_si, 2_si}, {6_si, 8_si}}));
}


template <bool is_array>
void run_odd_even2D() {
   typename ArrayOrView2D<is_array>::type A = sequence<int>(9).view2D(3, 3);
   ASSERT(equal(A(even, odd), {{{1_si}}, {{7_si}}}));
}


template <bool is_array>
void run_first_last2D() {
   typename ArrayOrView2D<is_array>::type A = sequence<int>(9).view2D(3, 3);
   ASSERT(equal(A(
                   firstN{
                      2
   },
                   lastN{2}),
                {{1_si, 2_si}, {4_si, 5_si}}));
}


template <bool is_array>
void run_vector2D() {
   typename ArrayOrView2D<is_array>::type A = sequence<int>(9).view2D(3, 3);
   ASSERT(equal(A(
                   std::vector<ImplicitInt>{
                      0, 2
   },
                   std::vector<ImplicitInt>{1, 2}),
                {{1_si, 2_si}, {7_si, 8_si}}));
}


template <bool is_array>
void run_empty2D() {
   typename ArrayOrView2D<is_array>::type A1 = sequence<int>(9).view2D(3, 3);
   ASSERT(equal(A1(std::vector<ImplicitInt>{0, 1}, std::vector<ImplicitInt>{}), {}));
   ASSERT(equal(A1(std::vector<ImplicitInt>{}, std::vector<ImplicitInt>{0, 1}), {}));

   auto A2 = sequence<int>(0).view2D(0, 0);
   ASSERT(equal(A2(even, odd), {}));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void slice1D() {
   run_seqn1D<true>();
   run_seq1D<true>();
   run_skip1D<true>();
   run_first1D<true>();
   run_last1D<true>();
   run_even1D<true>();
   run_odd1D<true>();
   run_reverse1D<true>();
   run_vector1D<true>();
   run_list1D<true>();
   run_complement1D<true>();
   run_empty1D<true>();
   run_fail1D<true>();
   run_compound1D<true>();

   run_seqn1D<false>();
   run_seq1D<false>();
   run_skip1D<false>();
   run_first1D<false>();
   run_last1D<false>();
   run_even1D<false>();
   run_odd1D<false>();
   run_reverse1D<false>();
   run_vector1D<false>();
   run_list1D<false>();
   run_complement1D<false>();
   run_empty1D<false>();
   run_fail1D<false>();
   run_compound1D<true>();
}


void slice2D() {
   run_seqn2D<true>();
   run_seq2D<true>();
   run_skip2D<true>();
   run_odd_even2D<true>();
   run_first_last2D<true>();
   run_vector2D<true>();
   run_empty2D<true>();

   run_seqn2D<false>();
   run_seq2D<false>();
   run_skip2D<false>();
   run_odd_even2D<false>();
   run_first_last2D<false>();
   run_vector2D<false>();
   run_empty2D<false>();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
   TEST_NON_TYPE(slice1D);
   TEST_NON_TYPE(slice2D);
   return EXIT_SUCCESS;
}
