# jag

Jag is a simple build system written as a bunch of C++ headers.  It really 
should be called "Yamr" for "Yet another makefile replacement", but I like that
"jag" occupies the home row and it sounds cool--like a sports car.

All of the work is done in the Builder class constructor.  You place one or
more Project objects in the constructor argument list, which are constructed
in-place; the options for the project are passed in the Project class 
constructor.

After everything is constructed you just call the run() method, and the
project(s) should build.

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

