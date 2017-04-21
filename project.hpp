#ifndef PROJECT_HPP_201704192246PDT
#define PROJECT_HPP_201704192246PDT 

#include "configurable.hpp"
#include "fileops.hpp"

class Project : public Configurable<Project> {
public:
  using string_t              = std::string;
  using dependency_list_t     = std::list<string_t>;
  using source_list_t         = std::list<string_t>;
  using object_list_t         = std::list<string_t>;

  enum build_type_t
  {
    Executable,
    BUILD_TYPE_MAX
  };

  template<class...Args>
  Project(Args&&...opts);

  void set_option(const Name& name);
  void set_option(const Depends& depends);
  void set_option(const Sources& sourcelist);
  void set_option(const SourcePath& p);
  void set_option(const TargetExecutable& name);         

  auto opt_build_type()   const -> build_type_t;
  auto opt_project_name() const -> const string_t&;
  auto opt_source_path()  const -> const string_t&;
  auto opt_target_name()  const -> const string_t&;
  auto opt_source_list()  const -> const source_list_t&;

private:
  build_type_t      opt_build_type_     = Executable;
  dependency_list_t opt_dependency_list_;
  object_list_t     opt_object_list_;
  string_t          opt_project_name_;
  source_list_t     opt_source_list_;
  string_t          opt_source_path_;
  string_t          opt_target_name_;
  const char*       build_type_names_[BUILD_TYPE_MAX];
};


template<class...Args>
Project::Project(Args&&...opts)  
{
  this->set_options(std::forward<Args>(opts)...);
}

void Project::set_option(const Depends& opt) 
{
  opt_dependency_list_ = opt.value;
}

void Project::set_option(const Name& opt) 
{
  opt_project_name_ = opt.value;
}

void Project::set_option(const Sources& opt)
{
  opt_source_list_ = opt.value; 
}

void Project::set_option(const SourcePath& opt)          
{ 
  opt_source_path_ = opt.value;
}

void Project::set_option(const TargetExecutable& value)         
{ 
  opt_build_type_  = Executable;
  opt_target_name_ = value.value;
}

auto Project::opt_build_type() const -> build_type_t
{
  return opt_build_type_;
}

auto Project::opt_project_name() const -> const string_t&
{
  return opt_project_name_;
}

auto Project::opt_source_list() const -> const source_list_t&
{
  return opt_source_list_;
}

auto Project::opt_source_path() const -> const string_t&
{
  return opt_source_path_;
}

auto Project::opt_target_name() const -> const string_t&
{
  return opt_target_name_;
}


#endif//PROJECT_HPP_201704192246PDT
