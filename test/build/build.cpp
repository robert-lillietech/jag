#include <builder/gcc-toolchain.hpp>
#include <builder/builder.hpp>

int main() 
{
  Builder<Gcc> builder(
      TargetName("test-executable"),
      SourcePath("../")
  );

  return 0;
}
