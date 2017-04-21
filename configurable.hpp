#ifndef CONFIGURABLE_HPP_201704202135PDT
#define CONFIGURABLE_HPP_201704202135PDT 

template<class T>
class Configurable {
public:
  template<class Arg>
  void set_options(Arg&& head);

  template<class Arg1, class...Args>
  void set_options(Arg1&& head, Args&&...tail);

};

template<class T>
template<class Arg>
void Configurable<T>::set_options(Arg&& head)
{
  static_cast<T*>(this)->set_option(std::forward<Arg>(head));
}

template<class T>
template<class Arg1, class...Args>
void Configurable<T>::set_options(Arg1&& head, Args&&...tail)
{
  static_cast<T*>(this)->set_option(std::forward<Arg1>(head));
  set_options(std::forward<Args>(tail)...);
}

#endif//CONFIGURABLE_HPP_201704202135PDT
