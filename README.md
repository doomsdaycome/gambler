![](https://img.shields.io/badge/Code-C-informational?style=flat&logo=c&logoColor=white&color=brightgreen)
![](https://img.shields.io/badge/Code-C++-informational?style=flat&logo=cplusplus&logoColor=white&color=brightgreen)

# Gambler Random Generator Library
Gambler is a library built on top of PCG32.

Despite being built on PCG32, it does not aim to resemble PCG32 in any meaningful way. It was created out of pure boredom, with the goal of implementing a minimal, fast random number generator for an Object-Oriented Programming class while fully supporting easy copy-pasting in future projects.

1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Execution](#execution)
4. [Troubleshoot](#troubleshoot)
5. [Contribution](#contribution)
2. [Acknowledgement](#acknowledgement)

## Introduction
Gambler is a high-speed random number generator built on top of the famous PCG32 algorithm by Melissa O'Neill. While traditional C/C++ random engines can be slow and bloated, Gambler leverages PCG's smart bit-shifting and permutation math to deliver raw, lightweight performance. It takes a world-class mathematical foundation and packages it into a clean, modern design built for maximum execution speed.

## Installation

```bash
cd libgambler/build
cmake .. && cmake --build .
```

## Execution

```bash
cd build
cmake .. && cmake --build .
./test
```

## Troubleshoot
It works! The program has been tested and runs correctly on the provided datasets. If you run into issues, make sure your compiler supports C++20 and that you implement the `CMakeLists.txt` yourself.
## Contribution

## Acknowledgement
This project leverages the following excellent open-source resources and mathematical foundations:

* **[PCG-Random](https://github.com/imneme)** by Melissa O'Neill – Used for the core `pcg32` basic random number generation code.
* **[Magicalbat](https://github.com/Magicalbat)** – Inspiration and reference for the distribution implementations.
* **[Box–Muller Transform](https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform)** – The mathematical algorithm utilized to generate normal distributions.