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

#define PLAYER_COUNT     4
#define RAISE_AMOUNT_MAX 3

PokerTable::PokerTable (Game* game, QWidget* parent) : QWidget (parent), wm_game (game), m_cardClickEnabled (false) {
  const int playerViewHeight = 106;
  QFont font ("FreeSans", 12);

  Player* const * players = game->players ();
  for (int i = 0; i < PLAYER_COUNT; ++i) {
    PlayerView* playerView = new PlayerView (players[i], font, this);
    playerView->move (PADDING_LEFT, PADDING_TOP + playerViewHeight * i);
    m_playerviews[players[i]] = playerView;
    if (players[i]->isHuman ())
      wm_humanPlayer = dynamic_cast<HumanPlayer*> (players[i]);
  }

  QLabel* label = new QLabel ("Pot", this);
  label->move (PADDING_LEFT + 387, PADDING_TOP + playerViewHeight * 4 + 2);
  label->setFont (font);

  wm_pot = new QLabel (this);
  wm_pot->setFixedWidth (61);
  wm_pot->move (PADDING_LEFT + 587, PADDING_TOP + playerViewHeight * 4 + 2);
  wm_pot->setAlignment (Qt::AlignRight);
  wm_pot->setFont (font);
  updatePotView ();

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
  m_gameThread->requestTermination ();
  m_gameThread->wait ();
  delete m_gameThread;
}

void PokerTable::continueGame (void) {
  m_cardClickEnabled = false;
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
  int r = wm_humanPlayer->raise ();
  if (r >= RAISE_AMOUNT_MAX) {
    continueGame ();
  } else {
    if (wm_call->isEnabled ())
      wm_call->setEnabled (false);
    QString action;
    if (wm_game->maxBet () == 0)
      action.sprintf ("bets %lc %d", 0x20AC, r * wm_game->chipValue ());
    else
      action.sprintf ("raises %lc %d", 0x20AC, r * wm_game->chipValue ());
    updatePlayerAction (wm_humanPlayer, action);
  }
}

void PokerTable::onCallClicked (bool checked) {
  wm_humanPlayer->call ();
  continueGame ();
}

void PokerTable::onCardClicked (int index) {
  if (m_cardClickEnabled) {
    if (wm_humanPlayer->replaceCard (index)) {
      m_playerviews.find (wm_humanPlayer)->second->updateCardView (index);
    }
  }
}

void PokerTable::onDealClicked (bool checked) {
  wm_game->deal ();
  std::map<const Player*, PlayerView*>::iterator iter;
  for (iter = m_playerviews.begin (); iter != m_playerviews.end (); ++iter) {
    iter->second->updateCardViews (iter->first->isHuman ());
    iter->second->updateMoney ();
    iter->second->updateAction ("");
  }
  updatePotView ();
  updatePlayerAction (wm_game->dealer (), "deals");
  continueGame ();
}

void PokerTable::onDehighlight (const Player* player) {
  m_playerviews.find (player)->second->dehighlight ();
}

void PokerTable::onDetermineWinners (int activePlayers) {
  if (activePlayers > 1) {
    for (const std::pair<const Player*, PlayerView*>& entry : m_playerviews) {
      if (entry.first->isActive ()) {
        entry.second->updateCardViews (true);
        entry.second->updateAction ("");
      }
    }
  }
  std::vector<const Player*>* winners = wm_game->determineWinners ();
  for (const Player* player : *winners) {
    PlayerView* playerView = m_playerviews.find (player)->second;
    playerView->updateAction ("wins");
    playerView->updateMoney ();
  }
  delete winners;
  updatePotView ();
}

void PokerTable::onDoneClicked (bool checked) {
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

void PokerTable::onHighlight (const Player* player) {
  m_playerviews.find (player)->second->highlight ();
}

void PokerTable::onUpdateCardViews (const Player* player) {
  m_playerviews.find (player)->second->updateCardViews (true);
}

void PokerTable::updatePlayerAction (const Player* player, QString str) {
  m_playerviews.find (player)->second->updateAction (str);
}

void PokerTable::updatePlayerMoney (const Player* player) {
  m_playerviews.find (player)->second->updateMoney ();
  updatePotView ();
}

void PokerTable::updatePotView (void) {
  QString str;
  str.sprintf ("%lc %d", 0x20AC, wm_game->pot ());
  wm_pot->setText (str);
}
