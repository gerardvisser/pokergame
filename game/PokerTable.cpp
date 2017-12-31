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

#include "GameThread.h"
#include "PokerTable.h"
#include <pokerengine/CardDeck.h>

#include <stdio.h>

PokerTable::PokerTable (Game* game, QWidget* parent) : QWidget (parent), wm_game (game) {
  const int playerViewHeight = 106;

  const Player* const * players = game->players ();
  for (int i = 0; i < 4; ++i) {
    PlayerView* playerView = new PlayerView (players[i], this);
    playerView->move (20, playerViewHeight * i + 20);
    m_playerviews[players[i]] = playerView;
  }

  wm_deal = new QPushButton ("Deal", this);
  wm_deal->setGeometry (20, playerViewHeight * 4 + 25, 100, 30);
  wm_deal->setEnabled (false);

  wm_bet = new QPushButton ("Bet/Raise", this);
  wm_bet->setGeometry (20 + 100 + 5, playerViewHeight * 4 + 25, 100, 30);
  wm_bet->setEnabled (false);

  wm_call = new QPushButton ("Call", this);
  wm_call->setGeometry (20 + 2 * (100 + 5), playerViewHeight * 4 + 25, 100, 30);
  wm_call->setEnabled (false);

  wm_done = new QPushButton ("Done", this);
  wm_done->setGeometry (20 + 3 * (100 + 5), playerViewHeight * 4 + 25, 100, 30);
  wm_done->setEnabled (false);

  connect (wm_deal, SIGNAL (clicked (bool)), this, SLOT (onDealClicked (bool)));
  connect (wm_bet, SIGNAL (clicked (bool)), this, SLOT (onBetClicked (bool)));
  connect (wm_call, SIGNAL (clicked (bool)), this, SLOT (onCallClicked (bool)));
  connect (wm_done, SIGNAL (clicked (bool)), this, SLOT (onDoneClicked (bool)));

  m_gameThread = new GameThread (this);
  m_gameThread->start ();


  QSize qsize = this->size ();
  printf ("WIDTH=%d, HEIGHT=%d\n", qsize.width (), qsize.height ());
}

PokerTable::~PokerTable (void) {
  /* TODO: How do we make sure thread has ended??  */
  delete m_gameThread;
}

QPushButton* PokerTable::betButton (void) {
  return wm_bet;
}

QPushButton* PokerTable::callButton (void) {
  return wm_call;
}

QPushButton* PokerTable::dealButton (void) {
  return wm_deal;
}

QPushButton* PokerTable::doneButton (void) {
  return wm_done;
}

Game* PokerTable::game (void) const {
  return wm_game;
}

void PokerTable::onBetClicked (bool checked) {
  printf ("Bet/Raise clicked.\n");
}

void PokerTable::onCallClicked (bool checked) {
  printf ("Call clicked.\n");
}

void PokerTable::onDealClicked (bool checked) {
  printf ("Deal clicked.\n");

  wm_game->deal ();
  std::map<const Player*, PlayerView*>::iterator iter;
  for (iter = m_playerviews.begin (); iter != m_playerviews.end (); ++iter) {
    iter->second->updateCardViews ();
  }
}

void PokerTable::onDoneClicked (bool checked) {
  printf ("Done clicked.\n");
}

void PokerTable::updatePlayerAction (const Player* player, QString str) {
  m_playerviews.find (player)->second->updateAction (str);
}
