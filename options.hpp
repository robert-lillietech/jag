#ifndef OPTIONS_HPP_201704162338PDT
#define OPTIONS_HPP_201704162338PDT 

#include "options_base.hpp"

struct Depends : ListOption {
};

struct AllowLogging : BooleanOption {
  AllowLogging(bool v) : BooleanOption(v) {}
};



struct TargetExecutable: SimpleOption 
{  
  template<class...Args>
  TargetExecutable(Args&&...args) : SimpleOption(std::forward<Args>(args)...) {} 
};

struct SourcePath : SimpleOption 
{  
  template<class...Args>
  SourcePath(Args&&...args) : SimpleOption(std::forward<Args>(args)...) {} 
};

struct Sources : ListOption
{
  template<class...Args>
  Sources(Args&&...args) : ListOption(std::forward<Args>(args)...)  {} 

};


#endif//OPTIONS_HPP_201704162338PDT
