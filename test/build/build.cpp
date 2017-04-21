#include <builder/gcc-toolchain.hpp>
#include <builder/builder.hpp>
#include <builder/project.hpp>

int main() 
{
  Builder<Gcc> builder(
      Project(
        Name("test-project"),
        Sources("test-source.cpp"),
        TargetExecutable("test-executable"),
        SourcePath("../")
      )
  );

  builder.run(); 
  return 0;
}
