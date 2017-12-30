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

#ifndef PLAYER_VIEW_INCLUDED
#define PLAYER_VIEW_INCLUDED

#include <QFrame>
#include <QLabel>
#include <pokerengine/Player.h>

class PlayerView : public QFrame {
  Q_OBJECT

private:
  const Player* m_player;
  QLabel* wm_cardViews[5];
  QLabel* wm_name;
  QLabel* wm_money;
  QLabel* wm_action;

public:
  explicit PlayerView (const Player* player, QWidget* parent = 0);

  void updateCardViews (void);
  void updateAction (const char* str);

signals:

public slots:
};

#endif
