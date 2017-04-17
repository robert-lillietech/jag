#include <cstdlib>
#include <string>
#include <iostream>
#include <list>
#include <utility>
#include <sstream>

#include "build-tools/gcc-toolchain.hpp"
#include "build-tools/builder.hpp"

int main() 
{
  Builder<Gcc> builder("test-target");
  builder.link(builder.compile("test-source.cpp"));
  return 0;
}
