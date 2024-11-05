// Arkadijs Slobodkins, 2023


#pragma once


#include <chrono>
#include <iomanip>
#include <iostream>

#include "../StrictCommon/strict_common.hpp"


// Removed std::format since Intel and clang compilers do not support it(at this time).
#define STRICT_TIME(a)                                                          \
   do {                                                                         \
      spp::util::timer t;                                                       \
      a;                                                                        \
      std::cout << (#a) << " took: " << std::setprecision(4) << std::scientific \
                << t.wall_time().val() << " seconds\n";                         \
   } while(0)


namespace spp::util {


struct timer {
   explicit timer() : start{std::chrono::high_resolution_clock::now()} {
   }

   void restart() {
      start = std::chrono::high_resolution_clock::now();
   }

   Strict64 wall_time() {
      return Strict64{double(std::chrono::duration_cast<std::chrono::nanoseconds>(
                                 std::chrono::high_resolution_clock::now() - start)
                                 .count())
                      / 1.e9};
   }

private:
   std::chrono::system_clock::time_point start;
};


}  // namespace spp::util
