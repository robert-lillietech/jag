#include <builder/gcc-toolchain.hpp>
#include <builder/builder.hpp>

int main() 
{
  Builder<Gcc> builder(
      TargetName("test-executable"),
      SourcePath("../")
  );

  builder.link(builder.compile("test-source.cpp"));
  return 0;
}
