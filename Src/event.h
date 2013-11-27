#pragma once

#include <list>
#include <functional>

#ifdef DEBUG
#include <iostream>
#endif

template<typename T>
class Event
{
  std::list< std::function<void(T)> > m_handlers;
  
  void init()
  {
    this->m_handlers = std::list< std::function<void(T)> >();
  }
  
public:
  
  Event()
  {
    this->init();
  }
  
  int add_handler(const std::function<void(T)>& handler)
  {
    this->m_handlers.push_back(handler);
#ifdef DEBUG
    std::cout << "Handler added" << std::endl;
#endif
    return this->m_handlers.size() - 1;
  }
  
  void remove_handler(const int handler_id)
  {
    this->m_handlers.earase(this->m_handlers.begin() + handler_id);
  }
  
  void trigger(T value) const
  {
    for (auto it = this->m_handlers.begin(); it != this->m_handlers.end(); ++it)
    {
      (*it)(value);
    }
  }
};
