#ifndef OPTIONS_HPP_201704162338PDT
#define OPTIONS_HPP_201704162338PDT 

#include <string>
#include <list>

struct SimpleOption {
  using string_t              = std::string;
  SimpleOption(string_t value);
  string_t          value;
};

SimpleOption::SimpleOption(string_t v) : value(v)
{
}

struct TargetExecutable: public SimpleOption 
{  
  template<class...Args>
  TargetExecutable(Args&&...args) : SimpleOption(std::forward<Args>(args)...) {} 
};

struct SourcePath : public SimpleOption 
{  
  template<class...Args>
  SourcePath(Args&&...args) : SimpleOption(std::forward<Args>(args)...) {} 
};

struct ListOption 
{
  using string_t              = std::string;
  using list_t                = std::list<string_t>;

  template<class...Args>
  ListOption(Args&&...list_items) 
    : value({ std::forward<Args>(list_items)... }) {}

  list_t value;
};

struct Sources : public ListOption
{
  template<class...Args>
  Sources(Args&&...args) : ListOption(std::forward<Args>(args)...)  {} 

};

#endif//OPTIONS_HPP_201704162338PDT
