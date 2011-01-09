#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class	MainWindow
{
 private:
  sf::RenderWindow	_mainWindow;
  sf::Font	_menuFont;
  sf::Event	_event;
  sf::Image	_Ibackground;
  sf::Image	_Icredits;
  sf::Image	_gameOverImg;
  sf::Sprite	_gameOver;
  sf::Sprite	_background;
  sf::Sprite	_credit;
  sf::String	_tPlay;
  sf::String	_tCredits;
  sf::String	_tExit;
  sf::String	_tItems;
 public:
  MainWindow();
  ~MainWindow();
  void	InitMenu();
  void	InitCredits();
  void	InitGameList();

  unsigned char	MainMenuLoop();
  unsigned char	GameListLoop();
  int	CreditsLoop();

  void	DrawAll();
  bool	IsOpened();
  bool	GetEvent();
  bool	CloseEvent();
  void	Clear();
  void	Close();
  int	gameOver();
  void	Display();
};

#endif //!MAINWINDOW_H
