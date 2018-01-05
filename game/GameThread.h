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

#ifndef GAME_THREAD_INCLUDED
#define GAME_THREAD_INCLUDED

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include "PokerTable.h"

class GameThread : public QThread {
  Q_OBJECT

private:
  QMutex* m_mutex;
  QWaitCondition* m_cond;
  PokerTable* wm_pokertable;
  bool m_terminationRequested;

public:
  explicit GameThread (PokerTable* pokertable);
  ~GameThread (void) override;

  void humanPlayerDone (void);
  void requestTermination (void);
  void run (void) override;

signals:
  void enableClickables (int mask);
  void playerAction (const Player* player, QString str);
  void playerMoneyUpdated (const Player* player);
  void updateCardViews (const Player* player);

private:
  void checkForTerminationRequest (void);
  void doBetting (void);
  void doCardReplacement (void);
  void enableButtonsForBetting (bool canCall, bool canRaise);
  void waitForHumanPlayer (void);
};

#endif
