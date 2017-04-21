#ifndef OPTIONS_HPP_201704162338PDT
#define OPTIONS_HPP_201704162338PDT 

#include "options_base.hpp"

struct Depends : ListOption {
};

struct AllowLogging : BooleanOption {
  AllowLogging(bool v) : BooleanOption(v) {}
};

struct Name : SimpleOption {
  Name(const std::string& value) : SimpleOption(value) {}
};

struct TargetExecutable: SimpleOption 
{  
  TargetExecutable(const std::string& value) : SimpleOption(value) {} 
};

struct SourcePath : SimpleOption 
{  
  template<class...Args>
  SourcePath(const std::string& value) : SimpleOption(value) {} 
};

struct Sources : ListOption
{
  template<class...Args>
  Sources(Args&&...args) : ListOption(std::forward<Args>(args)...)  {} 

};


#endif//OPTIONS_HPP_201704162338PDT
