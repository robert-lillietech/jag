#include <cstdlib>
#include <string>
#include <iostream>
#include <list>
#include <utility>
#include <sstream>

class Gcc {
public:
  using string_t              = std::string;
  static auto compile(const string_t& file) -> bool;
  static auto exec(const string_t& cmd) -> bool;
  static auto source_to_objname(const string_t& src) -> string_t;
};

auto Gcc::source_to_objname(const string_t& src) -> string_t
{
  return src + ".o";
}

auto Gcc::compile(const string_t& f) -> bool
{
  using namespace std;
  stringstream cmdline;
  cmdline << "g++ -c -o "
          << source_to_objname(f) << " "
          << f;

  Gcc::exec(cmdline.str());
}

auto Gcc::exec(const string_t& cmd) -> bool
{
  auto retval = system(cmd.c_str());
  return WIFEXITED(retval); 
}

template<class T>
class Builder {
public:
  using toolset_t             = T;
  using string_t              = typename toolset_t::string_t;

  auto compile(string_t filename) -> bool;
  
};

template<class T>
auto Builder<T>::compile(string_t filename) -> bool
{
  return toolset_t::compile(filename);
}


int main() 
{
  return 0;
}
