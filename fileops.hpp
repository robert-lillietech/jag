#ifndef TIMESTAMP_HPP_201704210018PDT
#define TIMESTAMP_HPP_201704210018PDT 
#include <sys/stat.h>
#include <string>

//auto timestamp(const std::string& filepath) -> time_t
//{
//  struct stat info;
//  auto t = stat(filepath.c_str(), &info);
//  if(t < 0) {
//    std::string msg("Could not execute stat() for file: ");
//    msg += filepath;
//    throw std::runtime_error(msg);
//  }
//  return info.st_mtime;
//
//}

struct file_info_t {
  bool              error;
  time_t            timestamp;
};

inline
auto file_info(const std::string& path) -> file_info_t
{
  struct stat info;
  auto t = stat(path.c_str(), &info);
  if(t < 0) {
    return file_info_t { 
      true,                   // error occurred
      -1                      // invalid timestam (infinite future?)
    };
  }

  return file_info_t { 
    false,                    // no error occurred
    info.st_mtime             // file timestamp
  };

}

#endif//TIMESTAMP_HPP_201704210018PDT
