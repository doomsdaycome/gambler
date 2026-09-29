/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *     http://www.pcg-random.org
 */

/*
 * This code is derived from the full C implementation, which is in turn
 * derived from the canonical C++ PCG implementation. The C++ version
 * has many additional features and is preferable if you can use C++ in
 * your project.
 */

#ifndef GAMBLER_RANDOM_HPP_
#define GAMBLER_RANDOM_HPP_

#include <cstdint>
#include <limits>

#if __has_include(<stdfloat>)
#include <stdfloat>
#endif

namespace gambler {

class PCG32Random {
private:
  uint64_t state_ = 0x853c49e6748fea9bull;
  uint64_t increment_ = 0xda3e39cb94b95bdbull;

#if defined(__STDCPP_FLOAT16_T__)
  std::float32_t normal_float_32_ =
      std::numeric_limits<std::float32_t>::quiet_NaN();
  std::float64_t normal_float_64_ =
      std::numeric_limits<std::float64_t>::quiet_NaN();
#else
  float normal_float_32_ = std::numeric_limits<float>::quiet_NaN();
  double normal_float_64_ = std::numeric_limits<double>::quiet_NaN();
#endif

public:
  PCG32Random() = default;
  PCG32Random(PCG32Random &&other) = default;
  PCG32Random(const PCG32Random &other) = default;
  PCG32Random(uint64_t initial_state, uint64_t initial_sequence);

  ~PCG32Random() = default;

  PCG32Random &operator=(PCG32Random &&other) = default;
  PCG32Random &operator=(const PCG32Random &other) = default;

  void SetSeed(uint64_t initial_state, uint64_t initial_sequence);

  template <typename T> T GetUniform();
  template <typename T> T GetBounded(T bound);
  template <typename T> T GetNormal(T mu, T sigma);

#if defined(__STDCPP_FLOAT16_T__)
  std::float32_t GetUniformFloat32();
  std::float64_t GetUniformFloat64();
#else
  float GetUniformFloat32();
  double GetUniformFloat64();
#endif

  uint32_t GetUniformUnsignedInt32();
  uint64_t GetUniformUnsignedInt64();

  uint32_t GetBoundedUnsignedInt32(uint32_t bound);
  uint64_t GetBoundedUnsignedInt64(uint64_t bound);

#if defined(__STDCPP_FLOAT16_T__)
  std::float32_t GetNormalFloat32(std::float32_t mu, std::float32_t sigma);
  std::float64_t GetNormalFloat64(std::float64_t mu, std::float64_t sigma);
#else
  float GetNormalFloat32(float mu, float sigma);
  double GetNormalFloat64(double mu, double sigma);
#endif
};

extern PCG32Random kGlobalPCG32Random;

} // namespace gambler

#endif // !GAMBLER_RANDOM_HPP_
