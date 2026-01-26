#include "test.hpp"

#include <cstdlib>


int main() {
   spp::set_cyan_output();
   spp::compiler_info();
   spp::print_config_info();
   spp::reset_output();
   return EXIT_SUCCESS;
}
