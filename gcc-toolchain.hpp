#ifndef GCC_TOOLCHAIN_HPP_201704161947PDT
#define GCC_TOOLCHAIN_HPP_201704161947PDT 

#include <memory>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include "options.hpp"

class Gcc final {
public:

  using string_t              = std::string;
  void set_option(const AllowLogging& state);

private:
  bool              opt_logging_enabled_ = true;
  
  auto exec(const string_t& cmd) const -> bool;

  auto opt_compiler_executable() const -> const string_t&;
  auto opt_build_output_file() const -> const string_t&;
  auto opt_build_no_link() const -> const string_t&;


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

void Gcc::set_option(const AllowLogging& opt) 
{
  opt_logging_enabled_ = opt.value;
}


#endif//GCC_TOOLCHAIN_HPP_201704161947PDT
