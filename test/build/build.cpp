#include <builder/gcc-toolchain.hpp>
#include <builder/builder.hpp>

int main() 
{
  Builder<Gcc> builder(
      Sources("test-source.cpp"),
      TargetExecutable("test-executable"),
      SourcePath("../")
  );

  builder.run();

  return 0;
}
