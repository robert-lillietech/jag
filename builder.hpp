#ifndef BUILDER_HPP_201704162000PDT
#define BUILDER_HPP_201704162000PDT 

#include <string>
#include "options.hpp"
#include "configurable.hpp"
#include "project.hpp"

template<class T>
class Builder : public Configurable<Builder<T>> {
public:
  using toolchain_t           = T;
  using string_t              = typename T::string_t;
  using project_t             = Project<T>;
  using project_list_t        = std::list<project_t>;

  template<class...Args>
  Builder(Args&&...options);

  void set_option(const project_t& project);

  void run() const;
  void build() const;
  void compile(const project_t&) const;
  void link(const project_t&) const;

template<class Arg>
  void write_log(Arg&& data) const;

template<class Arg1, class...Args>
  void write_log(Arg1&& head, Args&&...tail) const;

private:
  // members
  toolchain_t       toolchain_;
  string_t          target_;
  project_list_t    project_list_;
  bool              opt_logging_enabled_ = true;
};

template<class T>
void Builder<T>::set_option(const project_t& project)
{
}

template<class T>
template<class...Args>
Builder<T>::Builder(Args&&...opts)  
{
  this->set_options(std::forward<Args>(opts)...);
}

template<class T>
void Builder<T>::build() const
{
  for(const auto& project : project_list_) {
    switch(project.opt_build_type()) {
    case project_t::target_executable:
      compile(project);
      link(project);
      break;
    };
  }
}

template<class T>
void Builder<T>::compile(const project_t& project) const
{
  using namespace std;
  write_log("[[ COMPILING ]]\n");
  for(const auto& obj_info : project.opt_object_list()) {
    stringstream cmdline;
    auto& ofile = obj_info.object_filename;
    auto sfile = project.opt_source_path() + obj_info.source_filename;
    bool object_is_stale = timestamp(ofile) < timestamp(sfile);
    write_log(ofile, ":");
    if(object_is_stale) {
      cmdline <<        toolchain_t::opt_compiler_executable() 
              << " " << toolchain_t::opt_build_no_link()
              << " " << toolchain_t::opt_build_output_file()
              << " " << ofile
              << " " << sfile;
    }
    write_log(cmdline.str(), "\n");
    if(!toolchain_t::exec(cmdline.str())) {
      throw std::runtime_error("Compilation stage failed.");
    }
  }
}

template<class T>
void Builder<T>::link(const project_t& project) const
{
  using namespace std;
  write_log("[[ LINKING ]]\n");
  stringstream cmdline;
  auto& tfile = project.opt_target_name();
  cmdline << "g++"
          << " -o " << tfile;
  bool target_is_stale = false;
  write_log(tfile, ": ");
  for(const auto& obj_info : project.opt_object_list()) {
    auto& ofile = obj_info.object_filename;
    if(timestamp(tfile) < timestamp(ofile)) {
      target_is_stale = true;
    }
    cmdline << " " << obj_info.object_filename;
  }
  if(target_is_stale) {
    write_log(cmdline.str(), "\n");
    if(!toolchain_t::exec(cmdline.str())) {
      throw std::runtime_error("Linking stage failed.");
    }
  }
}

template<class T>
void Builder<T>::run() const
{ 
  toolchain_.build();
}

template<class T>
template<class Arg>
void Builder<T>::write_log(Arg&& data) const {
  if(opt_logging_enabled_) {
    std::cout << data;
  }
}

template<class T>
template<class Arg1, class...Args>
void Builder<T>::write_log(Arg1&& head, Args&&...tail) const {
  this->write_log(std::forward<Arg1>(head));
  this->write_log(std::forward<Args>(tail)...);
}



#endif//BUILDER_HPP_201704162000PDT
