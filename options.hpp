#ifndef OPTIONS_HPP_201704162338PDT
#define OPTIONS_HPP_201704162338PDT 

#include <string>

struct SimpleOption {
public:
  using string_t              = std::string;
  SimpleOption(std::string value);
  std::string          value;
};

SimpleOption::SimpleOption(std::string v) : value(v)
{
}

struct TargetName: public SimpleOption 
{  
  TargetName(std::string value) : SimpleOption(value) {} 
};

struct SourcePath : public SimpleOption 
{  
  SourcePath(std::string value) : SimpleOption(value) {} 
};



#endif//OPTIONS_HPP_201704162338PDT
