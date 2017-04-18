#ifndef BUILDER_HPP_201704162000PDT
#define BUILDER_HPP_201704162000PDT 

#include <string>
#include "options.hpp"

template<class T>
class Builder {
public:
  using toolchain_t           = T;
  using string_t              = typename T::string_t;

  template<class...Args>
  Builder(Args&&...options);

  void run() const;

  template<class Arg>
  void set_options(Arg&& arg);

  template<class Arg1, class...Args>
  void set_options(Arg1&& head, Args&&...tail);

private:
  // members
  toolchain_t       toolchain_;
  string_t          target_;

};

template<class T>
template<class Arg1, class...Args>
void Builder<T>::set_options(Arg1&& head, Args&&...tail)
{
  set_options(std::forward<Arg1>(head));
  set_options(std::forward<Args>(tail)...);
}

template<class T>
template<class Arg>
void Builder<T>::set_options(Arg&& opt)  
{
  toolchain_.set_option(std::forward<Arg>(opt));
}

template<class T>
template<class...Args>
Builder<T>::Builder(Args&&...opts)  
{
  set_options(std::forward<Args>(opts)...);
}

template<class T>
void Builder<T>::run() const
{ 
  toolchain_.build();
}



#endif//BUILDER_HPP_201704162000PDT
