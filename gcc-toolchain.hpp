#ifndef GCC_TOOLCHAIN_HPP_201704161947PDT
#define GCC_TOOLCHAIN_HPP_201704161947PDT 

#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

#include "options.hpp"

class Gcc final {
public:
  struct ObjectInfo;

  using object_info_t         = ObjectInfo;
  using string_t              = std::string;
  using object_list_t         = std::list<object_info_t>;
  
  enum build_type_t
  {
    target_executable 
  };

  void build() const;

  void set_option(const Sources& sourcelist);
  void set_option(const SourcePath& p);
  void set_option(const TargetExecutable& name);         

  build_type_t      opt_build_type_     = target_executable;
  object_list_t     opt_object_list_;
  string_t          opt_source_path_;
  string_t          opt_target_name_;
private:
  void compile() const;
  auto exec(const string_t& cmd) const -> bool;
  void link() const;
  auto timestamp(const string_t& filepath) const -> time_t;

  auto opt_compiler_executable() const -> const string_t&;
  auto opt_build_output_file() const -> const string_t&;
  auto opt_build_no_link() const -> const string_t&;
  auto opt_source_path() const -> const string_t&;
  auto opt_target_name() const -> const string_t&;


};

auto Gcc::timestamp(const string_t& filepath) const -> time_t
{
  struct stat info;
  auto t = stat(filepath.c_str(), &info);
  if(t < 0) {
    std::stringstream msg;
    msg << "Could not stat file: " << filepath;
    throw std::runtime_error(msg.str());
  }
  
  return info.st_mtime;

}

struct Gcc::ObjectInfo {
  const string_t&   source_filename;
  string_t          object_filename;
};

void Gcc::build() const
{
  switch(opt_build_type_) {
  case target_executable:
    compile();
    link();
    break;
  };
}

void Gcc::set_option(const Sources& sourcelist)
{
  auto source_to_objname = [](const string_t& s) -> string_t
  {
    return s + ".o";
  };

  for(const auto& source : sourcelist.value) {
    object_info_t info { source, source_to_objname(source) };
    opt_object_list_.push_back(std::move(info));
  }
}

void Gcc::set_option(const SourcePath& p)          
{ 
  opt_source_path_ = p.value;
}

void Gcc::set_option(const TargetExecutable& name)         
{ 
  opt_build_type_  = target_executable;
  opt_target_name_ = name.value;
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

void Gcc::link() const
{
  using namespace std;
  cout << "[[ LINKING ]]\n";
  stringstream cmdline;
  auto& tfile = opt_target_name();
  cmdline << "g++"
          << " -o " << tfile;
  bool target_is_stale = false;
  for(const auto& obj_info : opt_object_list_) {
    auto& ofile = obj_info.object_filename;
    if(timestamp(tfile) < timestamp(ofile)) {
      target_is_stale = true;
    }
    cmdline << " " << obj_info.object_filename;
  }
  if(target_is_stale) {
    if(!this->exec(cmdline.str())) {
      throw std::runtime_error("Linking stage failed.");
    }
  }
  else {
    cout << tfile << " up-to-date\n";
  }
}

void Gcc::compile() const
{
  using namespace std;
  cout << "[[ COMPILING ]]\n";
  for(const auto& obj_info : opt_object_list_) {
    stringstream cmdline;
    auto& ofile = obj_info.object_filename;
    auto sfile = opt_source_path() + obj_info.source_filename;
    bool object_is_stale = timestamp(ofile) < timestamp(sfile);
    if(object_is_stale) {
      cmdline << opt_compiler_executable() 
        << " " << opt_build_no_link()
        << " " << opt_build_output_file()
        << " " << ofile
        << " " << sfile;
    }
    else {
      cout << ofile << " up-to-date\n";
    }
    if(!this->exec(cmdline.str())) {
      throw std::runtime_error("Compilation stage failed.");
    }
  }
}

auto Gcc::exec(const string_t& cmd) const -> bool
{
  auto retval = system(cmd.c_str());
  std::cout << cmd << "\n";
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
