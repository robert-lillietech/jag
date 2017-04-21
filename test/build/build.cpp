#include <builder/gcc-toolchain.hpp>
#include <builder/builder.hpp>
#include <builder/project.hpp>

int main() 
{
  Builder<Gcc> builder(
      Project<Gcc>(
        Sources("test-source.cpp"),
        TargetExecutable("test-executable"),
        SourcePath("../")
      )
  );


  return 0;
}
