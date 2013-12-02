#pragma once

#include <map>

//#ifdef DEBUG
#include <iostream>
//#endif

#include "event.h"

class EventRepository
{
public:
  enum Types
  {
    LP_STAR_COUNT,
    LP_STAR_TIME_OF_LIVE,
    LP_PERCENT_BIG,
    LP_PERCENT_MIDDLE,
    LP_PERCENT_SMALL,
    
    WN_KEY_LEFT,
    WN_KEY_RIGHT,
    WN_KEY_UP,
    WN_KEY_DOWN,
    WN_KEY_PAGE_UP,
    WN_KEY_PAGE_DOWN,
    
    SS_FPS_CHANGED,
    SS_TIMER
  };

private:
  static EventRepository* m_instance;
  EventRepository() : m_events(std::map<Types, void*>()) { ; }
  virtual ~EventRepository() { ; }
  
  std::map<Types, void*> m_events;
  
public:
  static EventRepository* get_instance()
  {
    if (!m_instance)
    {
      m_instance = new EventRepository();
    }
    return m_instance;
  }
  
  template<typename T>
  static Event<T>* get_event(Types type)
  {
    EventRepository* er = EventRepository::get_instance();
    if (!er->m_events[type])
    {
#ifdef DEBUG
      std::cout << "Creating event: " << type << std::endl;
#endif
      er->m_events[type] = new Event<T>();
    }
    
    return static_cast< Event<T>* >(er->m_events[type]);
  }
};

typedef EventRepository::Types EventTypes;
