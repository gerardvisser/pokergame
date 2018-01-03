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

#include <pokerengine/BetIterator.h>
#include "GameThread.h"

#define RAISE_COUNT_MAX 3

GameThread::GameThread (PokerTable* pokertable) : wm_pokertable (pokertable) {
  m_cond = new QWaitCondition ();
  m_mutex = new QMutex ();

  connect (this, SIGNAL (playerAction (const Player*, QString)), pokertable, SLOT (updatePlayerAction (const Player*, QString)));
  connect (this, SIGNAL (playerMoneyUpdated (const Player*)), pokertable, SLOT (updatePlayerMoney (const Player*)));
  connect (this, SIGNAL (enableClickables (int)), pokertable, SLOT (onEnableClickables (int)));
}

GameThread::~GameThread (void) {
  /* TODO: Make sure mutex isn't locked.  */
  delete m_mutex;
  delete m_cond;
}

void GameThread::doBetting (void) {
  Game* game = wm_pokertable->game ();
  BetIterator iter (game);
  int raiseCount = 0;

  while (iter.hasNext ()) {
    Player* player = iter.next ();
    /* TODO: Highlight player?  */
    const int callAmount = game->maxBet () - player->totalBet ();

    if (player->isHuman ()) {
      HumanPlayer* humanPlayer = dynamic_cast<HumanPlayer*> (player);
      humanPlayer->resetRaise ();
      emit playerAction (player, "");
      waitForHumanPlayer (callAmount > 0, raiseCount < RAISE_COUNT_MAX);
    }

    QString action;
    int bet = player->getBet (callAmount, raiseCount < RAISE_COUNT_MAX);
    if (bet < callAmount) {
      player->setActive (false);
      action = "folds";
    } else if (bet == 0) {
      if (game->maxBet () > 0) {
        action = "checks";
      } else {
        action = "passes";
      }
    } else {
      if (game->maxBet () == 0) {
        /* callAmount must be 0 here.  */
        action.sprintf ("bets \342\202\254 %d", bet * game->chipValue ());
        game->addToMaxBet (bet);
      } else if (bet > callAmount) {
        action.sprintf ("raises \342\202\254 %d", (bet - callAmount) * game->chipValue ());
        game->addToMaxBet (bet - callAmount);
        ++raiseCount;
      } else {
        action = "calls";
      }
      player->removeMoney (game->chipValue () * bet);
      game->addToPot (game->chipValue () * bet);
      emit playerMoneyUpdated (player);
    }
    emit playerAction (player, action);

    /* TODO: Wait for a second and then dehighlight player...  */
  }
}

void GameThread::humanPlayerDone (void) {
  m_mutex->lock ();
  m_cond->wakeOne ();
  m_mutex->unlock ();
}

void GameThread::run (void) {
}

void GameThread::waitForHumanPlayer (bool canCall, bool canRaise) {
  int mask = ENABLE_DONE;
  if (canCall)
    mask |= ENABLE_CALL;
  if (canRaise)
    mask |= ENABLE_BET;
  emit enableClickables (mask);

  m_mutex->lock ();
  m_cond->wait (m_mutex);
  m_mutex->unlock ();
}
