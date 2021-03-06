#include "LoadLib.h"
#include "Session.h"
#include <stdlib.h>
#include "Elem.h"

Session::Session(AbsUDPNetwork *p, AbsThread *th, AbsMutex *mt, int nbGame)
{
  std::vector<Player *>     tabPlayer(4);
  _tabPlayer = tabPlayer;
  _p = p;
  std::cout << (int)_p->_socket << std::endl;
  _th = th;
  _mt = mt;
  _pingTime[0] = 0;
  _pingTime[1] = 0;
  _pingTime[2] = 0;
  _pingTime[3] = 0;
  _game_n = nbGame;
  _score = 0;
  mob_id = MIN_MOB_ID;
  _flagLoad = 0;
  a = 0;
  b = 0;
  t = 0;
  e = 0;
  boss1 = 0; //TEST
  boss2 = 0;
  boss3 = 0;
  m = 0;
  b1 = 0;
  b2 = 0;
  b3 = 0;
}


Session::~Session()
{
}


void  Session::sessionthread()
{
  int session = _session_n;
  Command           cmd(_game_n);

  _pingTime[session] = MAX_PING_TIME;
  _tabPlayer[session] = new Player(session +1);
  std::cout << "New player assign on slot " << session << "... Partie " << _game_n << std::endl;
  spawnPlayer(_tabPlayer[session]);
  cmd.sendConnect(_tabPlayer[session], _p);
  cmd.sendLife(_tabPlayer[session], _p);
  cmd.sendScore(_score, _p);
  //  cmd.sendMove(_tabPlayer[session], _p);
  //  std::cout << "En attente des nouvelles positions du client..." << std::endl;
  while (_pingTime[session] > 0)
    {
      //Pas de recv dans les threads
      //Le recv est gerer dans la boucle principal en dehors des threads

      //Ici c' est la boucle du thread
      //Lis dans le _tabPlayer et autre et envoit les infos avec _cmd.send****(***)
      //A 20 tours de boucles On fait un ping avec _cmd.sendPing()
      //Si la boucle recv principal ne recoit rien, on deco et on kill la thread
      
   if (_pingTime[session] % 100 == 0)
	cmd.sendMove(_tabPlayer[session], _p);

   if (_pingTime[session] == MID_PING_TIME)
	{
	  cmd.sendPing(_tabPlayer[session], _p);
	  std::cout << "Sending ping to client (player " << session + 1 << ")..." << std::endl;
	}
      if (_pingTime[session] == WARN_PING_TIME)
	{
	  cmd.sendPing(_tabPlayer[session], _p);
	  std::cout << "Sending ping to client (player " << session + 1 << ") (WARNING LAST PING BEFORE DISCONNECT)..." << std::endl;
	}
      _pingTime[session] -= 1;
      _th->ASleep(500);
    }
  cmd.sendDestroy(_tabPlayer[session]->getId(), 0,  _p);
  cmd.sendDisconnect(_tabPlayer[session], _p);
  std::cout << "Disconnecting client (player " << session + 1 << ") on slot " << session << "." << std::endl;
  std::cout << "The slot " << session << " is now free." << std::endl;
  _th->AExitThread(handles[session]);
}

void	Session::Create_Mob(int i)
{
  Object	*obj;

  if (_flagLoad == 0)
    {
      std::cout << "LOAD LIB" << std::endl;
      lib = new LoadLib();
      lib->initTabMonster();
      lib->checkLib();
      _flagLoad = 1;
    }

  //verifier le nombre de joeur
  int				k;
  int				count;
  std::list<Object *>::iterator it;

  k = 0;
  count = 0;
  while (k <= 3)
    {
      if (_pingTime[k] != 0)
	count++;
      k++;
    }
  
  if (count == 0)
    {
      a = 0;
      b = 0;
      t = 0;
      e = 0;
      boss1 = 0; //TEST                                                                                                                                                                                       
      boss2 = 0;
      boss3 = 0;
      m = 0;
      b1 = 0;
      b2 = 0;
      b3 = 0;
      _score = 0;
      it = _listObj.begin();
      while (it != _listObj.end())
	{
	  _listObj.erase(it);
	  it = _listObj.begin();
	}
    }
  
   //Generation d'un mob
  if (i% 9999)
    {
      //generation mob 12 BOULE
      int r;

      if (lib == NULL)
	{
	  std::cout << "LOAD LIB FAILED" << std::endl;
	  exit(0);
	}

      if (a % 1500 == 0 && (_score < LEVEL2 || _score >= LEVEL3))
	{
	  if (lib->getMaillon(1) != NULL)
	    {
	      r = rand();
	      if (r > 20)
		{
		  obj = lib->getInstance(1, mob_id++, 55, r%MAXRAND);
		  _listObj.push_back(obj);
		}
	      if (mob_id > 127)
		mob_id = MIN_MOB_ID;
	    }
	}

      //generation mob_14 AVION
      if (a % 1500 == 0)
	{
	  if (lib->getMaillon(3) != NULL)
	    {
	      
	      obj = lib->getInstance(3, mob_id++, 55, rand()%MAXRAND);
	      _listObj.push_back(obj);
	      
	      if (mob_id > 127)
		mob_id = MIN_MOB_ID;
	    }
	}
      
      //generation mob_11

	if (a % 4500 == 0 && _score >= LEVEL1)
	{
	  int p = rand()%MAXRAND;
	  if (lib->getMaillon(0) != NULL)
	    {
	      obj = lib->getInstance(0, mob_id++, 55, p);
	      _listObj.push_back(obj);
	      if (mob_id > 127)
		mob_id = MIN_MOB_ID;
	      obj = new Elem(mob_id++, 55, p + 4, 9);
	      _listObj.push_back(obj);
	      if (mob_id > 127)
		mob_id = MIN_MOB_ID;
	    }
	}
	
      //generation mob_13 vers le bas meduse et mob_15

      r = rand();
      if (a % 400 == 0 && t < 3 && _score >= LEVEL2 && r%5 == 0)
	{
	  if (lib->getMaillon(2) != NULL)
	    {
	      obj = lib->getInstance(2, mob_id++, 29, 3);
	      _listObj.push_back(obj);
	      if (mob_id > 127)
		mob_id = MIN_MOB_ID;
	    }
	  t++;
	  }
	
      // mob_15 du bas
      r = rand();
      if (a % 400 == 0 && e < 3 && _score >= LEVEL2 && r%10 == 0)
	  {
	    if (lib->getMaillon(4) != NULL)
	      {
		obj = lib->getInstance(4, mob_id++, 32, 40);
		_listObj.push_back(obj);
		if (mob_id > 127)
		  mob_id = MIN_MOB_ID;
	      }
	    e++;
	  }
      if (a % 9000 == 0)
	{
	  t = 0;
	  e = 0;
	}
      //generation mob_30 MUR

      int	p = 0;
      int	id = 11;
      if (a %10000 == 0 && _score >= LEVEL2)
	{
	  while (p < 8)
	    {
	      obj = new Elem(id++, 0 + p * 4, 0, 31);
	      _listObj.push_back(obj);
	      if (id > MIN_MOB_ID)
		id = 11;
	      p++;
	    }
	  p = 0;
	  while (p < 2)
	    {
	      obj = new Elem(id++, 15 + p * 4, 40, 30);
	      _listObj.push_back(obj);
	      if (id > MIN_MOB_ID)
		id = 11;
	      p++;
	    }
	  obj = new Elem(id++, 30, 1, 29);
	  _listObj.push_back(obj);
	  if (id > MIN_MOB_ID)
		id = 11;
	  obj = new Elem(id++, 30, 37, 28);
	  _listObj.push_back(obj);
	  if (id > MIN_MOB_ID)
	    id = 11;
	}

      //generation du cosmonaute
      
      if (_score > 100 && ((_score / 100) % 10) == 0 && a % 18000 == 0)
	{
	  std::cout << "Astronaute !" << std::endl;
	  obj = new Elem(mob_id++, 55, 20, 25);
	  _listObj.push_back(obj);
	  if (mob_id > 127)
	    mob_id = MIN_MOB_ID;
	}
      a++;
      if (a == 10000)
	a = 0;
    }
}

void	Session::Create_Boss(int i)
{
  Object	*obj;
  Command	cmd(_game_n);


  //Generation du boss 1
  if (_score >= LEVEL_BOSS1 && _score <= LEVEL2 && boss1 == 0)
    {
      boss1 = 1;
      cmd.sendScore(_score, _p);
      obj = new Elem(mob_id++, 90, 0, 21);
      _listObj.push_back(obj);
      if (mob_id > 127)
	mob_id = MIN_MOB_ID;
    }

  //Generation du boss 2
  if (_score >= LEVEL_BOSS2 && _score <= LEVEL3 && boss2 == 0)
    {
      boss2 = 1;
      cmd.sendScore(_score, _p);
      obj = new Elem(mob_id++, 90, 0, 22);
      _listObj.push_back(obj);
      if (mob_id > 127)
	mob_id = MIN_MOB_ID;
    }

  //Generation du boss 3
  if (_score >= LEVEL_BOSS3 && boss3 == 0)
    {
      boss3 = 1;
      cmd.sendScore(_score, _p);
      obj = new Elem(mob_id++, 90, 0, 24);
      _listObj.push_back(obj);
      if (mob_id > 127)
	mob_id = MIN_MOB_ID;
    }
}


void  Session::collision_playermissile_mob()
{
  Command       cmd(_game_n);
  Object	*obj;
  Object	*obj2;
  Object	*obj3;
  std::list<Object *>::iterator it;
  std::list<Object *>::iterator it2;

  it = _listObj.begin();
  while (it != _listObj.end())
    {
      it2 = _listObj.begin();
      while (it2 != _listObj.end())
	{
	  obj = *it;
	  obj2 = *it2;
	  if ((obj != obj2) && (obj->getType() == 5 || obj2->getType() == 5) &&
	      obj->getType() != 9 && obj2->getType() != 9 &&
	      (obj->getType() != 21 && obj2->getType() != 21) && //BOSS1
	      (obj->getType() != 22 && obj2->getType() != 22) && //BOSS2
	      (obj->getType() != 24 && obj2->getType() != 24) && //BOSS3
	      (obj->getType() != 26 && obj2->getType() != 26) && // COEUR
	      (!(obj->getType() == 5 && obj2->getType() == 5)) &&
	      obj->getX() < obj2->getX() + 3 && obj->getX() > obj2->getX() - 3 &&
	      obj->getY() < obj2->getY() + 3 && obj->getY() > obj2->getY() - 3)
	    {
	      if (
		  (obj->getType() == 5 && obj2->getType() == 6) ||
		  (obj->getType() == 6 && obj2->getType() == 5) ||
		    
		  (obj->getType() == 5 && obj2->getType() == 8) ||
		  (obj->getType() == 8 && obj2->getType() == 5) ||

		  (obj->getType() == 6 && obj2->getType() == 8) ||
		  (obj->getType() == 8 && obj2->getType() == 6) ||
		    
		  (obj->getType() == 5 && obj2->getType() == 7) ||
		  (obj->getType() == 7 && obj2->getType() == 5)
		  )
		{
		  
		}
	      else
		{
		  cmd.sendDestroy(obj->getId() , obj2->getId(), _p);
		  if (obj->getType() == 25 || obj2->getType() == 25)
		    {
		      if (obj->getType() == 25)
			obj3 = new Elem(mob_id++, obj->getX(), obj->getY(), 26);
		      else
			obj3 = new Elem(mob_id++, obj2->getX(), obj2->getY(), 26);
		      _listObj.push_back(obj3);
		      if (mob_id > 127)
			mob_id = MIN_MOB_ID;
		    }
		  _listObj.erase(it);
		  _listObj.erase(it2);
		  if (obj->getType() != 25 && obj2->getType() != 25)
		    {
		      _score += 10;
		      cmd.sendScore(_score, _p);
		    }
		  it = _listObj.begin();
		  it2 = _listObj.begin();
		}
	    }
	  it2++;
	}
      it++;
    }
}


void  Session::collision_playermissile_boss(unsigned char boss_type, short level_boss, short level, 
					    int limit_x_plus, int limit_x_minus, int limit_y_plus, int limit_y_minus)
{
  Command       cmd(_game_n);
  Object	*obj;
  Object	*obj2;
  std::list<Object *>::iterator it;
  std::list<Object *>::iterator it2;

  if (_score >= level_boss && _score <= level)
    {
      it = _listObj.begin();
      while (it != _listObj.end())
	{
	  it2 = _listObj.begin();
	  while (it2 != _listObj.end())
	    {
	      obj = *it;
	      obj2 = *it2;
	      if ((obj != obj2) && (obj->getType() == 5 || obj2->getType() == 5) &&
		  (obj->getType() == boss_type || obj2->getType() == boss_type) &&
		  obj->getType() != 9 && obj2->getType() != 9 &&
		  (!(obj->getType() == 5 && obj2->getType() == 5)) &&
		  obj->getX() < obj2->getX() + limit_x_plus && obj->getX() > obj2->getX() - limit_x_minus &&
		  obj->getY() < obj2->getY() + limit_y_plus && obj->getY() > obj2->getY() - limit_y_minus)
		{
		  if (obj->getType() == boss_type && obj2->getType() != boss_type && _score < level)
		    {
		      cmd.sendDestroy(obj2->getId() , 0, _p);
		      _score += 10;
		      cmd.sendScore(_score, _p);
		      _listObj.erase(it2);
		      it2 = _listObj.begin();
		    }
		  else if (obj->getType() != boss_type && obj2->getType() == boss_type && _score < level)
		    {
		      cmd.sendDestroy(obj->getId() , 0, _p); 
		      _score += 10;
		      cmd.sendScore(_score, _p);
		      _listObj.erase(it);
		      it = _listObj.begin();
		    }
		  else
		    {
		      //if (obj->getType() == boss_type || obj2->getType() == boss_type)//Update score boss
			_score = level;

		      cmd.sendDestroy(obj->getId() , obj2->getId(), _p); 
		      _listObj.erase(it);
		      _listObj.erase(it2);
		      cmd.sendScore(_score, _p);
		      it = _listObj.begin();
		      it2 = _listObj.begin();
		    }
		}
	      it2++;
	    }
	  it++;
	}
    }
}

void  Session::move_missile()
{
  Command       cmd(_game_n);
  Object	*obj;
  Object	*obj2;
  std::list<Object *>::iterator it;
  std::list<Object *>::iterator it2;

  it = _listObj.begin();
  while (it != _listObj.end())
    {
      obj = *it;
      if (obj->getType() == 5 || obj->getType() == 6)
	{
	  obj->move(); //Mouvement des missiles
	  cmd.sendObjMove(obj, _p);
	  if (obj->getX() > 50) //Missile depassant la fenetre
	    {
	      cmd.sendDestroy(obj->getId() , 0, _p);
	      _listObj.erase(it);
	      it = _listObj.begin();
	    }
	}
      it++;
    }
}

void Session::collision_player_mob()
{
  Command       cmd(_game_n);
  Object	*obj;
  Object	*obj2;
  std::list<Object *>::iterator it;
  std::list<Object *>::iterator it2;
  int   j;

  it = _listObj.begin();
  while (it != _listObj.end())
    {
      j = 0;
      obj = *it;
      while (j <= 3)
	{
	  if (_tabPlayer[j] != NULL && obj->getType() != 5 && 
	      obj->getX() < _tabPlayer[j]->getPosx() + 2 && obj->getX() > _tabPlayer[j]->getPosx() - 2 &&
	      obj->getY() < _tabPlayer[j]->getPosy() + 2 && obj->getY() > _tabPlayer[j]->getPosy() - 2)
	    {
	      if (obj->getType() == 26)
		{
		  std::cout << "Collison avec coeur !!!" << std::endl;
		  _tabPlayer[j]->setLife(_tabPlayer[j]->getLife() + 1);
		  cmd.sendLife(_tabPlayer[j], _p);
		  cmd.sendDestroy(obj->getId() , 0, _p);
		  _listObj.erase(it);
		  it = _listObj.begin();
		}
	      else if ((_tabPlayer[j]->getLife() - 1) > 0)
		{
		  std::cout << "ca pas Collison avec coeur !!!" << std::endl;
		  _tabPlayer[j]->setLife(_tabPlayer[j]->getLife() - 1);
		  cmd.sendLife(_tabPlayer[j], _p);
		  if (obj->getType() != 9 && obj->getType() != 21 && obj->getType() != 22 && obj->getType() != 24) // Les murs et les boss ne se detruisent pas
		    {
		      cmd.sendDestroy(obj->getId() , 0, _p); 
		      _listObj.erase(it);
		      it = _listObj.begin();
		    }
		  spawnPlayer(_tabPlayer[j]);
		}
	      else
		_pingTime[j] = 0;
	    }
	  j++;
	}
      it++;
    }
}



void  Session::sessionthreadElems()
{
  int i = 0;

  Command       cmd(_game_n);
  Object	*obj;
  Object	*obj2;
  std::list<Object *>::iterator it;
  std::list<Object *>::iterator it2;


  std::cout << "Success for threadElems" << std::endl;


   LoadLib	*lib;



   //_score = 10000; // TEST

  while (1) // On envoie des elements à l'infini
    {
      if (i == 10000)
	i = 0; 
      //std::cout << "ID : " << (int)mob_id << std::endl;
      if ((_score < LEVEL_BOSS1 || _score >= LEVEL1) &&
	  (_score < LEVEL_BOSS2 || _score >= LEVEL2) &&
	  (_score < LEVEL_BOSS3 || _score >= LEVEL3))
	Create_Mob(i);
      Create_Boss(i);
      if (i % 100 == 0)
	{
	  //Detection des collisions entre missiles joueur et mobs
	  collision_playermissile_mob();

	  //Detection des collisions entre missiles joueur et boss
	  collision_playermissile_boss(21, LEVEL_BOSS1, LEVEL1, 3, 3, 20, 10);
	  collision_playermissile_boss(22, LEVEL_BOSS2, LEVEL2, 3, 3, 3, 3);
	  collision_playermissile_boss(24, LEVEL_BOSS3, LEVEL3, 3, 3, 20, 10);

	  move_missile();

	  // Debut collision player et mob
	  collision_player_mob();
	}

      if (i % 200 == 0)
	{
	  std::cout << "Sending each objs position" << std::endl;
	  it = _listObj.begin();
	  while (it != _listObj.end())
	    {
	      obj = *it;
	      obj->move();
	      launchMissile(obj);
	      cmd.sendObjMove(obj, _p);
	      if (obj->getX() == 0) // depassant la fenetre
		{
		  cmd.sendDestroy(obj->getId() , 0, _p);
		  _listObj.erase(it);
		  it = _listObj.begin();
		}
	      it++;
	    }
	}
      _th->ASleep(500);
      i++;
    }
}

void	*Session::launchMissile(Object *obj)
{
  Object	*newObj;

  if (obj->getType() == 11)
    {
      if (m % 30 == 0)
	{
	  newObj = new Elem(mob_id++, obj->getX() - 3, obj->getY(), 6);
	  _listObj.push_back(newObj);
	  if (mob_id > 127)                                        
	    mob_id = MIN_MOB_ID;
	  newObj = new Elem(mob_id++, obj->getX() - 3, obj->getY(), 8);
	  _listObj.push_back(newObj);
	  if (mob_id > 127)                                        
	    mob_id = MIN_MOB_ID;
	}
      m++;
      if (m > 10000)
	m = 0;
    }
  if (obj->getType() == 21) //boss1
    {
      if (b1 % 8 == 0)
        {
          newObj = new Elem(mob_id++, obj->getX(), obj->getY() + rand() % 30, 7);
          _listObj.push_back(newObj);
	  if (mob_id > 127)                                        
	    mob_id = MIN_MOB_ID;
        }
      if (b1 == 50000)
        b1 = 0;
      b1++;
    }

  if (obj->getType() == 22) //boss2
    {
      if (b2 % 5 == 0)
        {
          newObj = new Elem(mob_id++, obj->getX(), obj->getY(), 7);
          _listObj.push_back(newObj);
	  if (mob_id > 127)                                        
	    mob_id = MIN_MOB_ID;
        }
      if (b2 == 50000)
        b2 = 0;
      b2++;
    }

  if (obj->getType() == 24) //boss3
    {


      if (b3 % 4 == 0)
        {
          newObj = new Elem(mob_id++, obj->getX(), obj->getY(), 7);
          _listObj.push_back(newObj);
	  if (mob_id > 127)                                        
	    mob_id = MIN_MOB_ID;
        }
      if (b3 == 50000)
        b3 = 0;
      b3++;
    }

  return (NULL);
}

void	*Session::sessionthreadElemsInit(Session *sess)
{
  sess->sessionthreadElems();
  return (NULL);
}

void	*Session::sessionthreadInit(Session *sess)
{
  sess->sessionthread();
  return (NULL);
}


void	Session::spawnPlayer(Player *player)
{
  unsigned char	NewPosX;
  unsigned char	NewPosY;
  int		unblock = 10000;
  Object	*obj;
  std::list<Object *>::iterator it;

  NewPosX = 8 + rand() % 4;
  NewPosY = rand() % 32;
  it = _listObj.begin();
  while (it != _listObj.end())
    {
      obj = *it;
      if (obj->getType() != 5)
	{
	  if (obj->getX() < NewPosX + 6 && obj->getX() > NewPosX - 6 &&
	      obj->getY() < NewPosY + 6 && obj->getY() > NewPosY - 6)
	    {
	      NewPosX = 8 + rand() % 4;
	      NewPosY = rand() % 32;
	      it = _listObj.begin();
	    }
	}
      it++;
      unblock--;
      if (unblock == 0)
	return;
    }
  player->setPosx(NewPosX);
  player->setPosy(NewPosY);
}
