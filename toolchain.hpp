#ifndef TOOLCHAIN_HPP_201704162203PDT
#define TOOLCHAIN_HPP_201704162203PDT 

#include <string>
#include <list>
#include <sstream>

class Toolchain {
public:
  using string_t              = std::string;
  using name_list_t           = std::list<string_t>;

  virtual ~Toolchain();
  template<class...Args>
  auto compile(Args&&...sources) const -> name_list_t;

  void link(const string_t& target, const name_list_t& objfiles) const;

  virtual auto opt_compiler_executable() const -> const string_t& = 0;
  virtual auto opt_build_output_file() const -> const string_t& = 0;
  virtual auto opt_build_no_link() const -> const string_t& = 0;
  virtual auto opt_source_path() const -> const string_t& = 0;
  virtual auto opt_target_name() const -> const string_t& = 0;

  virtual void set_source_path(const string_t& path) = 0;
  virtual void set_target_name(const string_t& name) = 0;

private:
template<class Head>
  void compile_list(name_list_t* objlist, Head&& head) const;
template<class Head, class...Tail>
  void compile_list(name_list_t* objlist, Head&& head, Tail&&...tail) const;

  virtual auto do_compile(const string_t& source) const -> string_t = 0;
  virtual auto do_exec(const string_t& cmd) const -> bool = 0;
  virtual void do_link(const name_list_t& objlist) const = 0;
};

Toolchain::~Toolchain() 
{
}

void Toolchain::link(const string_t& target, const name_list_t& objfiles) const
{
  this->do_link(objfiles);
}

template<class...Args>
auto Toolchain::compile(Args&&...sources) const -> name_list_t
{
  name_list_t objlist;
  this->compile_list(&objlist, std::forward<Args>(sources)...);
  return objlist;
}

template<class Head>
void Toolchain::compile_list(name_list_t* objlist, Head&& source) const
{
  auto objfile = do_compile(source);
  objlist->push_back(objfile);
}

template<class Head, class...Tail>
void Toolchain::compile_list(name_list_t* objlist, Head&& head, Tail&&...sources) const
{
  this->compile_list(objlist, head.c_str());
  return this->compile_list(objlist, std::forward<Tail>(sources)...);
}

#endif//TOOLCHAIN_HPP_201704162203PDT
