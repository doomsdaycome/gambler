#include <chrono>
#include <iomanip>
#include <iostream>

#include <gambler/random.hpp>

int main(int argc, char *argv[]) {
  constexpr int n = 10;
  float a[n];

  std::uint64_t seed =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();

  gambler::kGlobalPCG32Random.SetSeed(seed, 0xda3e39cb94b95bdbull);

  for (int i = 0; i < n; i++)
    a[i] = gambler::kGlobalPCG32Random.GetUniformFloat32();

  std::cout << std::fixed << std::setprecision(6);

  for (int i = 0; i < n; i++)
    std::cout << a[i] << (i == n - 1 ? "" : " ");

  return 0;
}
