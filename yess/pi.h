#ifndef YESS_PI
#define YESS_PI

#include <cmath>

namespace yess {

    // about 13 digits
    const uint64_t __pi_number_of_iterations = 30000;

    constexpr double __pi_impl(uint64_t i, double sign) {
        return i > __pi_number_of_iterations ?
                0.0 : sign / (i * (i + 1.0) * (i + 2.0)) + __pi_impl(i + 2, -sign);
    }

    constexpr double pi() {
        return 3.0 + __pi_impl(2, 4.0);
    }

}

#endif // YESS_PI
