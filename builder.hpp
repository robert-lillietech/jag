#ifndef BUILDER_HPP_201704162000PDT
#define BUILDER_HPP_201704162000PDT 

#include <string>
#include "toolchain.hpp"
#include "options.hpp"

template<class T>
class Builder {
public:
  using toolchain_t           = T;
  using string_t              = typename Toolchain::string_t;
  using name_list_t           = typename Toolchain::name_list_t;

  Builder(Toolchain& tc);

template<class Head>
  Builder(Head&& head);
template<class Head, class...Tail>
  Builder(Head&& head, Tail&&...tail);

  auto compile(string_t filename) const -> name_list_t;
  void link(const name_list_t& objfiles) const;

private:
  // ctor delegates
  void set_option(const SourcePath& p)          
    { 
      toolchain_.set_source_path(p.value); 
    }
  void set_option(const TargetName& name)         
    { 
      toolchain_.set_target_name(name.value); 
    }
  // members
  toolchain_t       toolchain_;
  string_t          target_;

};

template<class T>
template<class Head>
Builder<T>::Builder(Head&& head) 
{
  set_option(std::forward<Head>(head));
}

template<class T>
template<class Head, class...Tail>
Builder<T>::Builder(Head&& head, Tail&&...tail) : Builder(std::forward<Tail>(tail)...) 
{
  set_option(std::forward<Head>(head));
}

template<class T>
auto Builder<T>::compile(string_t filename) const -> name_list_t
  { return name_list_t({ toolchain_.compile(filename) }); }

template<class T>
void Builder<T>::link(const name_list_t& objfiles) const
  { toolchain_.link(target_, objfiles); }


#endif//BUILDER_HPP_201704162000PDT
