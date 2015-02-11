#ifndef YESS_RANDOM
#define YESS_RANDOM

#include <random>
#include <type_traits>
#include <functional>
#include <limits>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <chrono>

namespace yess {

  std::random_device random_device;

  using DefaultGenerator = std::mt19937_64;

  template <class Arithmetic>
  using DistributionWrapper = std::function<Arithmetic()>;

  template<
    class Arithmetic,
    class Distribution,
    class Generator = DefaultGenerator
  >
  DistributionWrapper<Arithmetic>
  initialize_distribution(Arithmetic min, Arithmetic max) {

    /**
    * Solution which works both Windows and *nix
    * return std::bind(Distribution(min, max),
    *   Generator(std::chrono::system_clock::now().time_since_epoch().count())
    * );
    */


    /**
    * This solution does not work on Windows
    */
    std::array<typename Generator::result_type, Generator::state_size> seed_data;
    std::generate(seed_data.begin(), seed_data.end(), std::ref(random_device));
    std::seed_seq seq(seed_data.begin(), seed_data.end());
    return std::bind(Distribution(min, max), Generator( seq ));
  }

  template<
    class Arithmetic,
    class Distribution,
    class Generator = DefaultGenerator
  >
  Arithmetic generate_number(Arithmetic min, Arithmetic max) {
    return initialize_distribution<
      Arithmetic, Distribution, Generator
    >(min, max)();
  }

  template<
    class Arithmetic,
    class Distribution,
    class Generator = DefaultGenerator
  >
  Arithmetic generate_number() {
    return initialize_distribution<
        Arithmetic, Distribution, Generator
    >(
        std::numeric_limits<Arithmetic>::lowest(),
        std::numeric_limits<Arithmetic>::max()
    )();
  }

  template<class Integral>
  Integral generate_integer(Integral min, Integral max) {
    return generate_number<Integral, std::uniform_int_distribution<Integral>>(min, max);
  }

  template<class Integral>
  Integral generate_integer() {
    return generate_number<Integral, std::uniform_int_distribution<Integral>>(
      std::numeric_limits<Integral>::lowest(),
      std::numeric_limits<Integral>::max()
    );
  }

  template<class Real>
  Real generate_real(Real min, Real max) {
    return generate_number<Real, std::uniform_real_distribution<Real>>(min, max);
  }

  template<class Real>
  Real generate_real() {
    return generate_number<Real, std::uniform_real_distribution<Real>>(
      std::numeric_limits<Real>::lowest(),
      std::numeric_limits<Real>::max()
    );
  }

  template<class Arithmetic>
  typename std::enable_if<std::is_integral<Arithmetic>::value, Arithmetic>::type
  rand(Arithmetic min, Arithmetic max) noexcept {
    return generate_integer<Arithmetic>(min, max);
  }

  template<class Arithmetic>
  typename std::enable_if<std::is_floating_point<Arithmetic>::value, Arithmetic>::type
  rand(Arithmetic min, Arithmetic max) noexcept {
    return generate_real<Arithmetic>(min, max);
  }

  template<class Arithmetic>
  typename std::enable_if<std::is_integral<Arithmetic>::value, Arithmetic>::type
  rand() noexcept {
    return generate_integer<Arithmetic>();
  }

  template<class Arithmetic>
  typename std::enable_if<std::is_floating_point<Arithmetic>::value, Arithmetic>::type
  rand() noexcept {
    return generate_real<Arithmetic>();
  }

}

#endif
