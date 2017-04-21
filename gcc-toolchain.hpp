#ifndef GCC_TOOLCHAIN_HPP_201704161947PDT
#define GCC_TOOLCHAIN_HPP_201704161947PDT 

#include <memory>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include "options.hpp"

class Toolchain {
};

class Gcc final {
public:

  using string_t              = std::string;

  auto exec(const string_t& cmd) const -> bool;

  static auto source_name_to_obj_name(const string_t& source) -> string_t;

  auto opt_compiler_executable() const -> const string_t&;
  auto opt_build_output_file() const -> const string_t&;
  auto opt_build_no_link() const -> const string_t&;
private:
  bool              opt_logging_enabled_ = true;
  



};

auto Gcc::exec(const string_t& cmd) const -> bool
{
  auto retval = system(cmd.c_str());
  if(WIFEXITED(retval)) {
    auto status = WEXITSTATUS(retval);
    return status == 0;
  } else {
    std::string msg =  "system() call failed: ";
                msg += cmd;
                msg += "\n";
    throw std::runtime_error(msg);
  }
}

auto Gcc::source_name_to_obj_name(const string_t& source) -> string_t
{
  return source + ".o";
}
auto Gcc::opt_compiler_executable() const -> const string_t&
{
  static string_t value = "gcc";
  return value;
}

auto Gcc::opt_build_no_link() const -> const string_t&
{
  static string_t value = "-c";
  return value;
}

auto Gcc::opt_build_output_file() const -> const string_t&
{
  static string_t value = "-o";
  return value;
}


#endif//GCC_TOOLCHAIN_HPP_201704161947PDT
