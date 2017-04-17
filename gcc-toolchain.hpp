#ifndef GCC_TOOLCHAIN_HPP_201704161947PDT
#define GCC_TOOLCHAIN_HPP_201704161947PDT 


class Gcc {
public:
  using string_t              = std::string;
  using name_list_t           = std::list<string_t>;

  template<class...Args>
  static auto compile(Args&&...sources) -> name_list_t;

  static auto link(const string_t& target, const name_list_t& objfiles) 
    -> string_t;
  static auto source_to_objname(const string_t& src) -> string_t;

  static auto opt_compiler_executable() -> const string_t&;
  static auto opt_build_output_file() -> const string_t&;
  static auto opt_build_no_link() -> const string_t&;

private:
template<class Head>
  static void compile_list(name_list_t* objlist, Head&& head);
template<class Head, class...Tail>
  static void compile_list(name_list_t* objlist, Head&& head, Tail&&...tail);
  static auto exec(const string_t& cmd) -> bool;
};

auto Gcc::opt_build_output_file() -> const string_t&
{
  static string_t value = "-o";
  return value;
}

auto Gcc::opt_build_no_link() -> const string_t&
{
  static string_t value = "-c";
  return value;
}

auto Gcc::opt_compiler_executable() -> const string_t&
{
  static string_t value = "gcc";
  return value;
}

template<class Head>
void Gcc::compile_list(name_list_t* objlist, Head&& source)
{
  using namespace std;
  stringstream cmdline;
  auto objfile = source_to_objname(source);
  cmdline << opt_compiler_executable() 
    << " " << opt_build_no_link()
    << " " << opt_build_output_file()
    << " " << objfile
    << " " << source;

  if(Gcc::exec(cmdline.str())) {
    objlist->push_back(objfile);
  }
}

template<class Head, class...Tail>
void Gcc::compile_list(name_list_t* objlist, Head&& head, Tail&&...sources)
{
  Gcc::compile_list(objlist, head.c_str());
  return Gcc::compile_list(objlist, std::forward<Tail>(sources)...);
}

auto Gcc::link(const string_t& target, const name_list_t& objfiles) 
  -> string_t
{
  using namespace std;
  stringstream cmdline;
  cmdline << "g++ -o"
          << " " << target;
  for(const auto& o : objfiles) {
    cmdline << " " << o;
  }
  return Gcc::exec(cmdline.str()) ? target : "";
}

auto Gcc::source_to_objname(const string_t& src) -> string_t
{
  return src + ".o";
}

template<class...Args>
auto Gcc::compile(Args&&...sources) -> name_list_t
{
  name_list_t objlist;
  Gcc::compile_list(&objlist, std::forward<Args>(sources)...);
  return objlist;
}

auto Gcc::exec(const string_t& cmd) -> bool
{
  auto retval = system(cmd.c_str());
  return WIFEXITED(retval); 
}


#endif//GCC_TOOLCHAIN_HPP_201704161947PDT
