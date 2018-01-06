/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2017, 2018 Gerard Visser.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef POKER_TABLE_INCLUDED
#define POKER_TABLE_INCLUDED

#include <map>
#include <QWidget>
#include <QPushButton>
#include <pokerengine/Game.h>
#include <pokerengine/HumanPlayer.h>
#include "PlayerView.h"

#define ENABLE_BET   1
#define ENABLE_CALL  2
#define ENABLE_CARDS 4
#define ENABLE_DEAL  8
#define ENABLE_DONE  16

class GameThread;

class PokerTable : public QWidget {
  Q_OBJECT

private:
  std::map<const Player*, PlayerView*> m_playerviews;
  GameThread* m_gameThread;
  HumanPlayer* wm_humanPlayer;
  QPushButton* wm_deal;
  QPushButton* wm_bet;
  QPushButton* wm_call;
  QPushButton* wm_done;
  QLabel* wm_pot;
  Game* wm_game;
  bool m_cardClickEnabled;

public:
  explicit PokerTable (Game* game, QWidget* parent = 0);
  virtual ~PokerTable (void);

  Game* game (void) const;

signals:

private slots:
  void onBetClicked (bool checked = false);
  void onCallClicked (bool checked = false);
  void onCardClicked (int index);
  void onDealClicked (bool checked = false);
  void onDehighlight (const Player* player);
  void onDetermineWinners (int activePlayers);
  void onDoneClicked (bool checked = false);
  void onEnableClickables (int mask);
  void onHighlight (const Player* player);
  void onUpdateCardViews (const Player* player);
  void updatePlayerAction (const Player* player, QString str);
  void updatePlayerMoney (const Player* player);

private:
  void continueGame (void);
  void updatePotView (void);
};

#endif
