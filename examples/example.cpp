#include <iostream>

#include <gambler/random.hpp>

int main(int argc, char *argv[]) {
  int n = 10;
  float a[n];

  for (int i = 0; i < n; i++)
    a[i] = gambler::kGlobalPCG32Random.GetUniformFloat32();

  for (int i = 0; i < n; i++)
    std::cout << a[i] << " ";

  return 0;
}
