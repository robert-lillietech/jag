#ifndef BUILDER_HPP_201704162000PDT
#define BUILDER_HPP_201704162000PDT 

template<class T>
class Builder {
public:
  using toolset_t             = T;
  using string_t              = typename toolset_t::string_t;
  using name_list_t           = typename toolset_t::name_list_t;

  Builder(const string_t& t);

  auto compile(string_t filename) -> name_list_t
    { return name_list_t({toolset_t::compile(filename)}); }
  auto link(const name_list_t& objfiles) -> string_t
    { return toolset_t::link(target_, objfiles); }
private:
  string_t target_;
};

template<class T>
Builder<T>::Builder(const string_t& t) : target_(t)
{
}

#endif//BUILDER_HPP_201704162000PDT
