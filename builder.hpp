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
  using obj_list_t            = std::list<string_t>;
  using project_list_t        = std::list<Project>;

  template<class...Args>
  Builder(Args&&...options);

  void set_option(Project&& project);
  void set_option(const AllowLogging& state);

  void run() const;


template<class Arg>
  void write_log(Arg&& data) const;

template<class Arg1, class...Args>
  void write_log(Arg1&& head, Args&&...tail) const;

private:
  // members
  string_t          target_;
  toolchain_t       toolchain_;
  bool              opt_logging_enabled_ = true;
  project_list_t    project_list_;

  void build(const Project&) const;
  auto compile(const Project&) const -> obj_list_t;
  void error_build_type_not_available(const Project&) const;
  void link(const Project&, const obj_list_t&) const;

};

template<class T>
void Builder<T>::set_option(Project&& project)
{
  project_list_.push_back(std::move(project));
}

template<class T>
template<class...Args>
Builder<T>::Builder(Args&&...opts)  
{
  this->set_options(std::forward<Args>(opts)...);
}

template<class T>
void Builder<T>::build(const Project& project) const
{
  auto build_type = project.opt_build_type();
  switch(build_type) {
  case Project::Executable:
    {
      auto objlist = this->compile(project);
      this->link(project, objlist);
    }
    break;
  default:
    this->error_build_type_not_available(project);
  }
}

template<class T>
auto Builder<T>::compile(const Project& project) const -> obj_list_t
{
  using namespace std;
  write_log("[[ COMPILING ]]\n");
  obj_list_t objlist;
  for(const auto& source : project.opt_source_list()) {
    stringstream cmdline;
    auto sfile = project.opt_source_path() + source;
    auto sfile_info = file_info(sfile);
    if(sfile_info.error) {
      stringstream msg;
      msg << "Could not open source file: " << sfile;
      throw std::runtime_error(msg.str());
    }
    auto ofile = toolchain_.source_name_to_obj_name(source);
    auto ofile_info = file_info(ofile);
    bool object_is_stale = (ofile_info.timestamp < sfile_info.timestamp)
                         | ofile_info.error;

    write_log(ofile, ": ");
    if(object_is_stale) {
      cmdline <<        toolchain_.opt_compiler_executable() 
              << " " << toolchain_.opt_build_no_link()
              << " " << toolchain_.opt_build_output_file()
              << " " << ofile
              << " " << sfile;
      write_log(cmdline.str(), "\n");
    } else {
      write_log("[no change]\n");
    }
    if(!toolchain_.exec(cmdline.str())) {
      throw std::runtime_error("Compilation stage failed.");
    }
    objlist.push_back(ofile);
  }
  return objlist;
}

template<class T>
void Builder<T>::error_build_type_not_available(const Project& project) const
{
  std::stringstream msg;
  msg << "Build type \"" << project.opt_build_type() 
      << "\" not available for project \"" << project.opt_project_name() << "\"";
  throw std::runtime_error(msg.str());
}

template<class T>
void Builder<T>::link(const Project& project, const obj_list_t& objlist) const
{
  using namespace std;
  write_log("[[ LINKING ]]\n");
  stringstream cmdline;
  auto& tfile = project.opt_target_name();
  cmdline << "g++"
          << " -o " << tfile;
  bool target_is_stale = false;
  write_log(tfile, ": ");
  for(const auto& obj : objlist) {
    auto ofile_info = file_info(obj);
    if(ofile_info.error) {
      stringstream msg;
      msg << "Could not open object file: " << obj;
      throw std::runtime_error(msg.str());
    }
    auto tfile_info = file_info(tfile);
    if(!target_is_stale) { 
      target_is_stale = (tfile_info.timestamp < ofile_info.timestamp)
                      | tfile_info.error;
    }
    cmdline << " " << obj;
  }
  if(target_is_stale) {
    write_log(cmdline.str(), "\n");
    if(!toolchain_.exec(cmdline.str())) {
      throw std::runtime_error("Linking stage failed.");
    }
  } else {
    write_log("[no change]\n");
  }
}

template<class T>
void Builder<T>::run() const
{ 
  // iterate over projects
  for(const auto& project : project_list_) {
    this->build(project); 
  }
}

template<class T>
void Builder<T>::set_option(const AllowLogging& opt) 
{
  opt_logging_enabled_ = opt.value;
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
