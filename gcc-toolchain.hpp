#ifndef GCC_TOOLCHAIN_HPP_201704161947PDT
#define GCC_TOOLCHAIN_HPP_201704161947PDT 

#include "toolchain.hpp"

class Gcc final : public Toolchain {
public:
  using string_t              = std::string;
  using name_list_t           = std::list<string_t>;

  auto source_to_objname(const string_t& src) const -> string_t;

  virtual auto opt_compiler_executable() const -> const string_t& override;
  virtual auto opt_build_output_file() const -> const string_t& override;
  virtual auto opt_build_no_link() const -> const string_t& override;
  virtual auto opt_source_path() const -> const string_t& override;
  virtual auto opt_target_name() const -> const string_t& override;

  virtual void set_source_path(const string_t& path) override;
  virtual void set_target_name(const string_t& name) override;

private:
  virtual auto do_compile(const string_t& source) const -> string_t override;
  virtual auto do_exec(const string_t& cmd) const -> bool override;
  virtual void do_link(const name_list_t& objfiles) const override;

  string_t          opt_source_path_;
  string_t          opt_target_name_;
};

void Gcc::set_source_path(const string_t& p) 
{
  opt_source_path_ = p;
}

void Gcc::set_target_name(const string_t& n) 
{
  opt_target_name_ =  "-o ";
  opt_target_name_ += n;
}

auto Gcc::opt_target_name() const -> const string_t&
{
  return opt_target_name_;
}

auto Gcc::opt_source_path() const -> const string_t&
{
  return opt_source_path_;
}

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

void Gcc::do_link(const name_list_t& objfiles) const
{
  using namespace std;
  stringstream cmdline;
  cmdline << "g++"
          << " " << opt_target_name();
  for(const auto& o : objfiles) {
    cmdline << " " << o;
  }
  if(!this->do_exec(cmdline.str())) {
    string msg =  "Link failed for command:\n"; 
           msg += cmdline.str();
           msg += "\n";
    throw std::runtime_error(msg);
  }
}

auto Gcc::do_compile(const string_t& source) const -> string_t
{
  using namespace std;
  stringstream cmdline;
  auto objfile = source_to_objname(source);
  cmdline << opt_compiler_executable() 
    << " " << opt_build_no_link()
    << " " << opt_build_output_file()
    << " " << objfile
    << " " << opt_source_path() << source;

  if(!this->do_exec(cmdline.str())) {
    string msg =  "Compilation failed for command:\n"; 
           msg += cmdline.str();
           msg += "\n";
    throw std::runtime_error(msg);
  }
  return objfile;
}

auto Gcc::source_to_objname(const string_t& src) const -> string_t
{
  return src + ".o";
}

auto Gcc::do_exec(const string_t& cmd) const -> bool
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


#endif//GCC_TOOLCHAIN_HPP_201704161947PDT
