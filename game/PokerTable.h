/*
   Author:  Gerard Visser
   e-mail:  visser.gerard(at)gmail.com

   Copyright (C) 2017 Gerard Visser.

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

#include <QWidget>
#include <QPushButton>
#include <pokerengine/Game.h>
#include "PlayerView.h"

class PokerTable : public QWidget {
  Q_OBJECT

private:
  PlayerView* wm_playerView[4];
  QPushButton* wm_deal;
  QPushButton* wm_bet;
  QPushButton* wm_call;
  QPushButton* wm_done;
  Game* wm_game;

public:
  explicit PokerTable (Game* game, QWidget* parent = 0);
  virtual ~PokerTable (void);

signals:

private slots:
  void onBetClicked (bool checked = false);
  void onCallClicked (bool checked = false);
  void onDealClicked (bool checked = false);
  void onDoneClicked (bool checked = false);
};

#endif