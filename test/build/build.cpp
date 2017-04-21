#include <jag/gcc-toolchain.hpp>
#include <jag/builder.hpp>
#include <jag/project.hpp>

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
