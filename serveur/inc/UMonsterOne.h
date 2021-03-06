#ifndef _MONSTERONE_H_
# define _MONSTERONE_H_

#include "../Object.h"

class	MonsterOne : public Object
{
 public:
  MonsterOne(char id, char x, char y, char type = 11);
  void	move();
  ~MonsterOne();
  char getType();
};

typedef MonsterOne *(*maker_monster)(char id, char x, char y);

#endif // _MONSTERONE_H_
