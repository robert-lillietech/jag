#ifndef GCC_TOOLCHAIN_HPP_201704161947PDT
#define GCC_TOOLCHAIN_HPP_201704161947PDT 

#include "toolchain.hpp"

#include <sstream>

class Gcc final : public Toolchain {
public:
  using string_t              = std::string;
  using name_list_t           = std::list<string_t>;

  auto link(const string_t& target, const name_list_t& objfiles) 
    -> string_t;
  auto source_to_objname(const string_t& src) const -> string_t;

  virtual auto opt_compiler_executable() const -> const string_t& override;
  virtual auto opt_build_output_file() const -> const string_t& override;
  virtual auto opt_build_no_link() const -> const string_t& override;


private:
  auto exec(const string_t& cmd) const -> bool;
  virtual auto do_compile(const string_t& source) const -> string_t override;
};

auto Gcc::opt_build_output_file() const -> const string_t&
{
  static string_t value = "-o";
  return value;
}

auto Gcc::opt_build_no_link() const -> const string_t&
{
  static string_t value = "-c";
  return value;
}

auto Gcc::opt_compiler_executable() const -> const string_t&
{
  static string_t value = "gcc";
  return value;
}

auto Gcc::do_compile(const string_t& source) const -> string_t
{
  using namespace std;
  stringstream cmdline;
  auto objfile = source_to_objname(source);
  cmdline << opt_compiler_executable() 
    << " " << "-g"
    << " " << opt_build_no_link()
    << " " << opt_build_output_file()
    << " " << objfile
    << " " << source;

  if(!Gcc::exec(cmdline.str())) {
    throw std::runtime_error("Build failed.");
  }
  return objfile;
}

auto Gcc::link(const string_t& target, const name_list_t& objfiles) 
  -> string_t
{
  using namespace std;
  stringstream cmdline;
  cmdline << "g++ -o"
          << " " << target;
  for(const auto& o : objfiles) {
    cmdline << " " << o;
  }
  return Gcc::exec(cmdline.str()) ? target : "";
}

auto Gcc::source_to_objname(const string_t& src) const -> string_t
{
  return src + ".o";
}

auto Gcc::exec(const string_t& cmd) const -> bool
{
  auto retval = system(cmd.c_str());
  return WIFEXITED(retval); 
}


#endif//GCC_TOOLCHAIN_HPP_201704161947PDT
