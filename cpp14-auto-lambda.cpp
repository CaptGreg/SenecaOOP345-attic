// https://solarianprogrammer.com/2015/01/13/raspberry-pi-raspbian-install-gcc-compile-cpp-14-programs/

// 2106-03-18-raspbian-jessie.zip uses g++ (Raspbian 4.9.2-10) 4.9.2

// 2016-03-18-raspbian-jessie.zip, https://www.raspberrypi.org/downloads/raspbian/
//   RASPBIAN JESSIE
//   Full desktop image based on Debian Jessie
//   Version:March 2016
//   Release date:2016-03-18
//   Kernel version:4.1

// uname -a: Linux raspberrypi 4.1.19-v7+ #858 SMP Tue Mar 15 15:56:00 GMT 2016 armv7l GNU/Linux
// g++ --version: g++ (Raspbian 4.9.2-10) 4.9.2

// g++ -std=c++14 -Wall -pedantic cpp14.cpp -o cpp14 && ./cpp14

// lambda C++14 test

#include<iostream>
#include<complex>

int main() 
{
  // Store a generalized lambda, that squares a number, in a variable
  auto func = [](auto input) { return input * input; };

  // Usage examples:
  // square of an int
  std::cout << func(10) << std::endl;

  // square of a double
  std::cout << func(2.345) << std::endl;

  // square of a complex number
  std::cout << func(std::complex<double>(3, -2)) << std::endl;
  // GB (3,-2) * (3,-2) = (3 * 3 - -2 * -2, 2 * 3 * -2) = (9-4,-12) = (5,-12)

  return 0;
}
/*
output:

100
5.49903
(5,-12)
*/
