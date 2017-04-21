#ifndef PROJECT_HPP_201704192246PDT
#define PROJECT_HPP_201704192246PDT 

#include "configurable.hpp"
#include "timestamp.hpp"

template<class T>
class Project : public Configurable<Project<T>> {
public:
  struct ObjectInfo;
  using toolchain_t           = T;
  using string_t              = std::string;
  using object_info_t         = ObjectInfo;
  using object_list_t         = std::list<object_info_t>;
  using dependency_list_t     = std::list<string_t>;

  enum build_type_t
  {
    target_executable 
  };

  template<class...Args>
  Project(Args&&...opts);

  void set_option(const Depends& depends);
  void set_option(const Sources& sourcelist);
  void set_option(const SourcePath& p);
  void set_option(const TargetExecutable& name);         

  auto opt_source_path() const -> const string_t&;
  auto opt_target_name() const -> const string_t&;
  auto opt_build_type()  const -> build_type_t;

private:
  build_type_t      opt_build_type_     = target_executable;
  dependency_list_t opt_dependency_list_;
  object_list_t     opt_object_list_;
  string_t          opt_source_path_;
  string_t          opt_target_name_;
};

template<class T>  
struct Project<T>::ObjectInfo {
  const string_t&   source_filename;
  string_t          object_filename;
};

template<class T>
template<class...Args>
Project<T>::Project(Args&&...opts)  
{
  this->set_options(std::forward<Args>(opts)...);
}

template<class T>
void Project<T>::set_option(const Depends& depends) 
{
  opt_dependency_list_ = depends.value;
}

template<class T>
void Project<T>::set_option(const Sources& sourcelist)
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

template<class T>
void Project<T>::set_option(const SourcePath& p)          
{ 
  opt_source_path_ = p.value;
}

template<class T>
void Project<T>::set_option(const TargetExecutable& name)         
{ 
  opt_build_type_  = target_executable;
  opt_target_name_ = name.value;
}

template<class T>
auto Project<T>::opt_target_name() const -> const string_t&
{
  return opt_target_name_;
}

template<class T>
auto Project<T>::opt_source_path() const -> const string_t&
{
  return opt_source_path_;
}


#endif//PROJECT_HPP_201704192246PDT
