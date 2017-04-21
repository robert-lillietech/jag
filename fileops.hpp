#ifndef TIMESTAMP_HPP_201704210018PDT
#define TIMESTAMP_HPP_201704210018PDT 
#include <sys/stat.h>
#include <string>

auto timestamp(const std::string& filepath) -> time_t
{
  struct stat info;
  auto t = stat(filepath.c_str(), &info);
  if(t < 0) {
    std::stringstream msg;
    msg << "Could not stat file: " << filepath;
    throw std::runtime_error(msg.str());
  }
  
  return info.st_mtime;

}



#endif//TIMESTAMP_HPP_201704210018PDT
