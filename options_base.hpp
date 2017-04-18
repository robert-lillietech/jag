#ifndef OPTIONS_BASE_HPP_201704181139PDT
#define OPTIONS_BASE_HPP_201704181139PDT 

#include <string>
#include <list>

template<class T>
struct Option {
  using value_t     = T;

  template<class Arg>
  Option(Arg&& v) : value(std::forward<Arg>(v)) {}

  value_t           value;
};

/*! \brief Base class for options which can be enabled/disabled. */
struct BooleanOption : Option<bool> {
  BooleanOption(bool v) : Option(v) {}
};

/*! \brief Base class for options which only use a single value. */
struct SimpleOption : Option<std::string> {
  SimpleOption(value_t v) : Option(v) {}
};

/*! \brief Base class for options which carry a list of values. */
struct ListOption : Option<std::list<std::string>>
{
  template<class...Args>
  ListOption(Args&&...list_items) 
    : Option(value_t({ std::forward<Args>(list_items)... })) {}
};


#endif//OPTIONS_BASE_HPP_201704181139PDT
