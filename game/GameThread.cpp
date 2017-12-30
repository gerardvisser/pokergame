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

#include <pokerengine/BetIterator.h>
#include <pokerengine/HumanPlayer.h>
#include "GameThread.h"

#define RAISE_COUNT_MAX 3

GameThread::GameThread (PokerTable* pokertable) : wm_pokertable (pokertable) {
  m_cond = new QWaitCondition ();
  m_mutex = new QMutex ();
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
      /* TODO: Clear action text of player.  */
      waitForHumanPlayer (callAmount > 0, raiseCount < RAISE_COUNT_MAX);
    }

    int bet = player->getBet (callAmount, raiseCount < RAISE_COUNT_MAX);
    if (bet < callAmount) {
      player->setActive (false);
      /* TODO: action text: FOLDS.  */
    } else if (bet == 0) {
      if (game->maxBet () > 0) {
        /* TODO: action text: CHECKS.  */
      } else {
        /* TODO: action text: PASSES.  */
      }
    } else {
      if (game->maxBet () == 0) {
        /* callAmount must be 0 here.  */
        /* TODO: action text: BETS.  */
        game->addToMaxBet (bet);
      } else if (bet > callAmount) {
        /* TODO: action text: RAISES.  */
        game->addToMaxBet (bet - callAmount);
        ++raiseCount;
      } else {
        /* TODO: action text: CALLS.  */
      }
//      player->removeMoney (CHIP_VALUE * bet);
      /* TODO: Add (CHIP_VALUE * bet) to pot. */
      /* TODO: Update money of player and pot in screen.  */
    }

    /* TODO: Wait for a second and then dehighlight player...  */
  }
}

QMutex* GameThread::mutex (void) {
  return m_mutex;
}

void GameThread::run (void) {
}

QWaitCondition* GameThread::waitCondition (void) {
  return m_cond;
}

void GameThread::waitForHumanPlayer (bool canCall, bool canRaise) {
  m_mutex->lock ();
  wm_pokertable->doneButton ()->setEnabled (true);
  if (canCall)
    wm_pokertable->callButton ()->setEnabled (true);
  if (canRaise)
    wm_pokertable->betButton ()->setEnabled (true);
  m_cond->wait (m_mutex);
  m_mutex->unlock ();
}
