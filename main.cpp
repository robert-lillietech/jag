#include <cstdlib>
#include <string>
#include <iostream>

void build_target1()
{
  system("g++ -c -o test1.o test-source.cpp");
  system("g++ -o target1 test1.o");
}

class ToolGccCompiler {
public:
  void input(const char* source);
  void output(const char* target);
  void run();

  void add_option(std::string* cmdline, const char* data);
  void add_option(std::string* cmdline, const std::string& data);

  auto opt_dont_link() -> const char*;
  auto opt_executable() -> const char*;
  auto opt_obj_extension() -> const char*;
  auto opt_output_file() -> const char*;
 
private:
  std::string input_;
  std::string output_;
};

auto ToolGccCompiler::opt_obj_extension() -> const char*
{
  return ".o";
}

void ToolGccCompiler::add_option(std::string* s, const std::string& opt)
{
  *s += " " + opt;
}

void ToolGccCompiler::add_option(std::string* s, const char* opt)
{
  *s += " ";
  *s += opt;
}

auto ToolGccCompiler::opt_executable() -> const char*
{
  return "g++";
}

auto ToolGccCompiler::opt_dont_link() -> const char*
{
  return "-c";
}

auto ToolGccCompiler::opt_output_file() -> const char*
{
  return "-o";
}

void ToolGccCompiler::input(const char* source)
{
  input_ = source;
}

void ToolGccCompiler::output(const char* target)
{
  output_ = target;
}

void ToolGccCompiler::run()
{
  using namespace std;
  string cmd;
  add_option(&cmd, opt_executable());
  add_option(&cmd, opt_dont_link());
  add_option(&cmd, opt_output_file());
  add_option(&cmd, input_ + opt_obj_extension());
  add_option(&cmd, input_);
  cout << cmd << "\n";
  system(cmd.c_str());
}

void build_target2()
{
  ToolGccCompiler compiler;
  compiler.input("test-source.cpp");
  compiler.output("target2");
  compiler.run(); 
}

int main() {
  build_target1();
  build_target2();
  return 0;
};
