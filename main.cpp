#include <cstdlib>
#include <string>
#include <iostream>
#include <list>
#include <utility>

class ToolGccCompiler {
public:
  using string_t              = std::string;
  using input_list_t          = std::list<string_t>;
  using output_list_t         = std::list<string_t>;

  void input(string_t source);
template<class Arg1, class...Args>
  void input(Arg1&& first, Args&&...sources);
  void output(string_t target);
  void run() { return do_run(); }

  auto opt_dont_link() -> string_t;
  auto opt_executable() -> string_t;
  auto opt_obj_extension() -> string_t;
  auto opt_output_file() -> string_t;

private:
  virtual void do_add_option(std::string* cmdline, string_t data);
  virtual void do_add_option(std::string* cmdline, const std::string& data);
  virtual void do_run();
  virtual auto do_filename_to_objname(string_t filename) -> std::string;

  input_list_t      input_list_;
  output_list_t     output_list_;
};

class ToolGccLinker : public ToolGccCompiler {
public:
  void run();
};

template<class Arg1, class...Args>
void ToolGccCompiler::input(Arg1&& head, Args&&...tail) 
{
  this->input(head);
  this->input(std::forward<Args>(tail)...);
}

auto ToolGccCompiler::opt_obj_extension() -> string_t
{
  return ".o";
}

void ToolGccCompiler::do_add_option(std::string* s, const std::string& opt)
{
  *s += " " + opt;
}

void ToolGccCompiler::do_add_option(std::string* s, string_t opt)
{
  *s += " ";
  *s += opt;
}

auto ToolGccCompiler::opt_executable() -> string_t
{
  return "g++";
}

auto ToolGccCompiler::opt_dont_link() -> string_t
{
  return "-c";
}

auto ToolGccCompiler::opt_output_file() -> string_t
{
  return "-o";
}

auto ToolGccCompiler::do_filename_to_objname(string_t f) -> std::string
{

}

void ToolGccCompiler::input(string_t source)
{
  input_list_.push_back(source);
  output_list_.push_back(do_filename_to_objname(source));
}

void ToolGccCompiler::output(string_t target)
{
  //output_ = target;
}

void ToolGccCompiler::do_run()
{
  using namespace std;
  string cmd;
  for(const auto& in : input_list_) {
    do_add_option(&cmd, opt_executable());
    do_add_option(&cmd, opt_dont_link());
    do_add_option(&cmd, opt_output_file());
    do_add_option(&cmd, in + opt_obj_extension());
    do_add_option(&cmd, in);
  }
  cout << cmd << "\n";
  system(cmd.c_str());
}

void build_target1()
{
  system("g++ -c -o test1.o test-source.cpp");
  system("g++ -o target1 test1.o");
}

void build_target2()
{
  ToolGccCompiler compiler;
  compiler.input("test-source.cpp");
  compiler.output("target2");
  compiler.run(); 
  ToolGccLinker linker;
  linker.input(compiler.output());
}

void build_target3()
{
  ToolGccCompiler compiler;
//  compiler.input("test-source.cpp", "test-source2.cpp");
//  compiler.output("target3");
//  compiler.run(); 
}

int main() {
  build_target1();
  build_target2();
  build_target3();
  return 0;
};

