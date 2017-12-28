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

#include <QApplication>
#include <pokerengine/Game.h>
#include "Window.h"
#include "PokerTable.h"
#include "pixmaps.h"

static void deinit (void);
static void init (void);

int main (int argc, char** args, char** env) {
  QApplication app (argc, args);
  init ();

  Window window;
  window.show ();

  Game game;
  PokerTable* table = new PokerTable (&game);
  window.setScreen (table);

  int exitCode = app.exec ();
  deinit ();
  return exitCode;
}

static void deinit (void) {
  CardDeck::deinit ();
  pixmaps::deinit ();
}

static void init (void) {
  pixmaps::init ();
  CardDeck::init ();
}
