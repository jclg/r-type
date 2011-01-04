#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#define FILESERVER "fileserver.txt"
#include "udpnetwork.h"
#include "Element.h"
#include "AbstractWindow.h"
#include "AbstractMusic.h"
#include "ElementFactory.h"
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <list>

class gameClient
{
 private:
  int			_idPlayer;
  int			_score;
  AbstractWindow	_window;
  AbstractMusic		_music;
  ElementFactory	_factory;
 protected:
  udpNetwork	*_network;
 public:
  std::list<Element*>	_object;
  gameClient();
  ~gameClient();
  void		cleanexplosion();
  void		readServer(void *userData);
  void		loopClient();
  int		mainClient();
  void		fillnetwork(udpNetwork*);
  void		requestConnect();
  void		requestPing();
  void		requestDisconnect();
  void		requestMove(int nb);
  void		requestShoot();
  void		findCommand(char buffer[NBOCTETS]);
  bool		replyConnect(char buffer[NBOCTETS]);
  void		replyDisconnect(char buffer[NBOCTETS]);
  void		replyPing(char buffer[NBOCTETS]);
  void		replyMove(char buffer[NBOCTETS]);
  void		replyLife(char buffer[NBOCTETS]);
  void		replyScore(char buffer[NBOCTETS]);
  void		replyDestroy(char buffer[NBOCTETS]);
};


#endif
