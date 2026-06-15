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

#pragma once

#include <cstdint>
#include <limits>

namespace gambler {
class Random {
private:
  uint64_t state_ = 0x853c49e6748fea9bULL;
  uint64_t increment_ = 0xda3e39cb94b95bdbULL;

  float normal_float_32_ = std::numeric_limits<float>::quiet_NaN();
  float normal_float_64_ = std::numeric_limits<double>::quiet_NaN();

public:
  Random() = default;
  Random(const Random &other) = default;
  Random &operator=(const Random &other) = default;
  Random(uint64_t initial_state, uint64_t initial_sequence);

  ~Random() = default;

  Random(Random &&other) noexcept = default;
  Random &operator=(Random &&other) noexcept = default;

  void setSeed(uint64_t initial_state, uint64_t initial_sequence);

  template <typename T> T getUniform();

  uint32_t getUniformUnsignedInt32();
  uint64_t getUniformUnsignedInt64();
  float getUniformFloat32();
  double getUniformFloat64();

  template <typename T> T getBounded(T bound);

  uint32_t getBoundedUnsignedInt32(uint32_t bound);
  uint64_t getBoundedUnsignedInt64(uint64_t bound);

  template <typename T> T getNormal(T mu, T sigma);

  float getNormalFloat32(float mu, float sigma);
  double getNormalFloat64(double mu, double sigma);
};

extern Random GLOBAL_P32_RANDOM;
} // namespace gambler
