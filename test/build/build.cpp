#include <builder/gcc-toolchain.hpp>
#include <builder/builder.hpp>

int main() 
{
  Builder<Gcc> builder(
      AllowLogging(false),
      Sources("test-source.cpp"),
      TargetExecutable("test-executable"),
      SourcePath("../")
  );

  builder.run();

  return 0;
}
