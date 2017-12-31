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

#define BUTTON_HEIGHT 30
#define BUTTON_WIDTH  100
#define PADDING_LEFT  20
#define PADDING_TOP   20

PokerTable::PokerTable (Game* game, QWidget* parent) : QWidget (parent), wm_game (game) {
  const int playerViewHeight = 106;
  QFont font ("FreeSans", 12);

  const Player* const * players = game->players ();
  for (int i = 0; i < 4; ++i) {
    PlayerView* playerView = new PlayerView (players[i], font, this);
    playerView->move (PADDING_LEFT, PADDING_TOP + playerViewHeight * i);
    m_playerviews[players[i]] = playerView;
  }

  QLabel* label = new QLabel ("Pot", this);
  label->move (PADDING_LEFT + 387, PADDING_TOP + playerViewHeight * 4 + 2);
  label->setFont (font);

  QString str;
  str.sprintf ("\342\202\254 %d", game->pot ());
  wm_pot = new QLabel (this);
  wm_pot->setFixedWidth (61);
  wm_pot->move (PADDING_LEFT + 587, PADDING_TOP + playerViewHeight * 4 + 2);
  wm_pot->setAlignment (Qt::AlignRight);
  wm_pot->setFont (font);
  wm_pot->setText (str);

  wm_deal = new QPushButton ("Deal", this);
  wm_deal->setGeometry (PADDING_LEFT, PADDING_TOP + playerViewHeight * 4 + 28, BUTTON_WIDTH, BUTTON_HEIGHT);
  wm_deal->setEnabled (false);

  wm_bet = new QPushButton ("Bet/Raise", this);
  wm_bet->setGeometry (PADDING_LEFT + BUTTON_WIDTH + 5, PADDING_TOP + playerViewHeight * 4 + 28, BUTTON_WIDTH, BUTTON_HEIGHT);
  wm_bet->setEnabled (false);

  wm_call = new QPushButton ("Call", this);
  wm_call->setGeometry (PADDING_LEFT + 2 * (BUTTON_WIDTH + 5), PADDING_TOP + playerViewHeight * 4 + 28, BUTTON_WIDTH, BUTTON_HEIGHT);
  wm_call->setEnabled (false);

  wm_done = new QPushButton ("Done", this);
  wm_done->setGeometry (PADDING_LEFT + 3 * (BUTTON_WIDTH + 5), PADDING_TOP + playerViewHeight * 4 + 28, BUTTON_WIDTH, BUTTON_HEIGHT);
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

void PokerTable::updatePlayerMoney (const Player* player) {
  QString str;
  str.sprintf ("\342\202\254 %d", wm_game->pot ());
  wm_pot->setText (str);
  m_playerviews.find (player)->second->updateMoney ();
}
