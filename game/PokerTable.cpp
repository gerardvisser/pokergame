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

#include "GameThread.h"
#include "PokerTable.h"
#include <pokerengine/CardDeck.h>

#include <stdio.h>

#define BUTTON_HEIGHT    30
#define BUTTON_WIDTH     100
#define PADDING_LEFT     20
#define PADDING_TOP      20

#define RAISE_AMOUNT_MAX 3

PokerTable::PokerTable (Game* game, QWidget* parent) : QWidget (parent), wm_game (game), m_cardClickEnabled (false) {
  const int playerViewHeight = 106;
  QFont font ("FreeSans", 12);

  Player* const * players = game->players ();
  for (int i = 0; i < 4; ++i) {
    PlayerView* playerView = new PlayerView (players[i], font, this);
    playerView->move (PADDING_LEFT, PADDING_TOP + playerViewHeight * i);
    m_playerviews[players[i]] = playerView;
    if (players[i]->isHuman ())
      wm_humanPlayer = dynamic_cast<HumanPlayer*> (players[i]);
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

void PokerTable::continueGame (void) {
  wm_bet->setEnabled (false);
  wm_call->setEnabled (false);
  wm_deal->setEnabled (false);
  wm_done->setEnabled (false);
  m_gameThread->humanPlayerDone ();
}

Game* PokerTable::game (void) const {
  return wm_game;
}

void PokerTable::onBetClicked (bool checked) {
  printf ("Bet/Raise clicked.\n");

  int r = wm_humanPlayer->raise ();
  if (r >= RAISE_AMOUNT_MAX) {
    continueGame ();
  } else {
    if (wm_call->isEnabled ())
      wm_call->setEnabled (false);
    QString action;
    if (wm_game->maxBet () == 0)
      action.sprintf ("bets \342\202\254 %d", r * wm_game->chipValue ());
    else
      action.sprintf ("raises \342\202\254 %d", r * wm_game->chipValue ());
    updatePlayerAction (wm_humanPlayer, action);
  }
}

void PokerTable::onCallClicked (bool checked) {
  printf ("Call clicked.\n");

  wm_humanPlayer->call ();
  continueGame ();
}

void PokerTable::onCardClicked (int index) {
  if (m_cardClickEnabled) {
    printf ("Card clicked: %d.\n", index);
  }
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

  continueGame ();
}

void PokerTable::onEnableClickables (int mask) {
  if ((mask & ENABLE_BET) != 0)
    wm_bet->setEnabled (true);
  if ((mask & ENABLE_CALL) != 0)
    wm_call->setEnabled (true);
  if ((mask & ENABLE_CARDS) != 0)
    m_cardClickEnabled = true;
  if ((mask & ENABLE_DEAL) != 0)
    wm_deal->setEnabled (true);
  if ((mask & ENABLE_DONE) != 0)
    wm_done->setEnabled (true);
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
