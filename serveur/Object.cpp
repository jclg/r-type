#include <stdlib.h>
#include <iostream>
#include "Object.h"
#include "Session.h"

Object::Object()
{

}

Object::Object(const char id, const char x, const char y, const char type)
{
	m_id = id;
	m_x = x;
	m_y = y;
	m_type = type;
	m_lim = 0;
}

Object::~Object()
{

}

void Object::move(Session *s)
{
  Object *obj;


  if (m_type == 12)
    {
      m_x--;
     if (m_lim < 5)
       m_y++;
     else
       m_y--;
     if (m_lim == 10)
       m_lim = 0;
     else
       m_lim++;
   }

  else if (m_type == 11)
    {
      static int k = 0;
      
      if (k % 3 == 0)
	{
	  obj = new Object(s->mob_id++, m_x - 3, m_y, 6);
	  s->_listObj.push_back(obj);
	}
      if (s->mob_id > 127)
	s->mob_id = 11;
      m_x--;
      if (k == 10)
	k = 0;
      k++;
    }
  else if (m_type == 5)
    m_x++;
  else if (m_type == 6 || m_type == 11 || m_type == 9)
    m_x--;
}

char Object::getId()
{
  return (m_id);
}

char Object::getX()
{
  return (m_x);
}

char Object::getY()
{
  return (m_y);
}

char Object::getType()
{
  return (m_type);
}
