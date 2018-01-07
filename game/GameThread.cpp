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
#include <pokerengine/CardDrawIterator.h>
#include "GameThread.h"

#define PLAYER_COUNT    4
#define RAISE_COUNT_MAX 3

#define STATE_DEAL             1
#define STATE_BETTING_1        2
#define STATE_CARD_REPLACEMENT 3
#define STATE_BETTING_2        4

class TerminationRequestException {};

GameThread::GameThread (PokerTable* pokertable) : wm_pokertable (pokertable), m_terminationRequested (false) {
  m_cond = new QWaitCondition ();
  m_mutex = new QMutex ();

  connect (this, SIGNAL (playerAction (const Player*, QString)), pokertable, SLOT (updatePlayerAction (const Player*, QString)));
  connect (this, SIGNAL (playerMoneyUpdated (const Player*)), pokertable, SLOT (updatePlayerMoney (const Player*)));
  connect (this, SIGNAL (enableClickables (int)), pokertable, SLOT (onEnableClickables (int)));
  connect (this, SIGNAL (updateCardViews (const Player*)), pokertable, SLOT (onUpdateCardViews (const Player*)));
  connect (this, SIGNAL (determineWinners (int)), pokertable, SLOT (onDetermineWinners (int)));
  connect (this, SIGNAL (dehighlight (const Player*)), pokertable, SLOT (onDehighlight (const Player*)));
  connect (this, SIGNAL (highlight (const Player*)), pokertable, SLOT (onHighlight (const Player*)));
}

GameThread::~GameThread (void) {
  delete m_mutex;
  delete m_cond;
}

void GameThread::checkForTerminationRequest (void) {
  m_mutex->lock ();
  bool terminationRequested = m_terminationRequested;
  m_mutex->unlock ();
  if (terminationRequested)
    throw TerminationRequestException ();
}

int GameThread::doBetting(int activePlayers) {
  Game* game = wm_pokertable->game ();
  BetIterator iter (game);
  int raiseCount = 0;

  while (iter.hasNext ()) {
    Player* player = iter.next ();
    emit highlight (player);
    const int callAmount = game->maxBet () - player->totalBet ();

    if (player->isHuman ()) {
      HumanPlayer* humanPlayer = dynamic_cast<HumanPlayer*> (player);
      humanPlayer->resetRaise ();
      emit playerAction (player, "");
      enableButtonsForBetting (callAmount > 0, raiseCount < RAISE_COUNT_MAX);
      waitForHumanPlayer ();
    }

    QString action;
    int bet = player->getBet (callAmount, raiseCount < RAISE_COUNT_MAX);
    if (bet < callAmount) {
      player->setActive (false);
      action = "folds";
      --activePlayers;
    } else if (bet == 0) {
      if (game->maxBet () > 0) {
        action = "checks";
      } else {
        action = "passes";
      }
    } else {
      if (game->maxBet () == 0) {
        /* callAmount must be 0 here.  */
        action.sprintf ("bets %lc %d", 0x20AC, bet * game->chipValue ());
        game->addToMaxBet (bet);
      } else if (bet > callAmount) {
        action.sprintf ("raises %lc %d", 0x20AC, (bet - callAmount) * game->chipValue ());
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

    sleep (1);
    emit dehighlight (player);
  }
  return activePlayers;
}

void GameThread::doCardReplacement (void) {
  CardDeck& deck = wm_pokertable->game ()->cardDeck ();
  CardDrawIterator iter (wm_pokertable->game ());

  while (iter.hasNext ()) {
    Player* player = iter.next ();
    emit highlight (player);

    if (player->isHuman ()) {
      HumanPlayer* humanPlayer = dynamic_cast<HumanPlayer*> (player);
      humanPlayer->clearCardsToReplace ();
      emit playerAction (player, "");
      emit enableClickables (ENABLE_DONE | ENABLE_CARDS);
      waitForHumanPlayer ();
    }

    const std::vector<int>& cardsToReplace = player->cardsToReplace ();
    int numberOfCardsToReplace = cardsToReplace.size ();
    for (int i = 0; i < numberOfCardsToReplace; ++i) {
      player->setCard (cardsToReplace[i], deck.drawCard ());
    }
    QString action;
    if (numberOfCardsToReplace == 1)
      action = "replaces 1 card";
    else
      action.sprintf ("replaces %d cards", numberOfCardsToReplace);
    emit playerAction (player, action);
    if (player->isHuman () && numberOfCardsToReplace > 0) {
      emit updateCardViews (player);
    }

    sleep (1);
    emit dehighlight (player);
  }
}

void GameThread::enableButtonsForBetting (bool canCall, bool canRaise) {
  int mask = ENABLE_DONE;
  if (canCall)
    mask |= ENABLE_CALL;
  if (canRaise)
    mask |= ENABLE_BET;
  emit enableClickables (mask);
}

void GameThread::humanPlayerDone (void) {
  m_mutex->lock ();
  m_cond->wakeOne ();
  m_mutex->unlock ();
}

void GameThread::requestTermination (void) {
  m_mutex->lock ();
  m_terminationRequested = true;
  m_cond->wakeOne ();
  m_mutex->unlock ();
}

void GameThread::run (void) {
  try {

    int activePlayers;
    int state = STATE_DEAL;

    while (true) {
      switch (state) {
      case STATE_DEAL:
        state = STATE_BETTING_1;
        emit enableClickables (ENABLE_DEAL);
        waitForHumanPlayer ();
        break;

      case STATE_BETTING_1:
        activePlayers = doBetting (PLAYER_COUNT);
        if (activePlayers == 1) {
          emit determineWinners (activePlayers);
          state = STATE_DEAL;
        } else if (wm_pokertable->game ()->maxBet () == 0) {
          state = STATE_DEAL;
        } else {
          state = STATE_CARD_REPLACEMENT;
        }
        break;

      case STATE_CARD_REPLACEMENT:
        doCardReplacement ();
        state = STATE_BETTING_2;
        break;

      case STATE_BETTING_2:
        activePlayers = doBetting (activePlayers);
        emit determineWinners (activePlayers);
        state = STATE_DEAL;
        break;
      }
    }

  } catch (TerminationRequestException ignore) {
  }
}

void GameThread::waitForHumanPlayer (void) {
  m_mutex->lock ();
  m_cond->wait (m_mutex);
  m_mutex->unlock ();
  checkForTerminationRequest ();
}
