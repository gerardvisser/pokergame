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

#include <stddef.h>
#include "Window.h"

Window::Window(QWidget *parent) : QWidget (parent), m_screen (NULL) {
  setFixedSize (800, 600);
}

void Window::setScreen (QWidget* screen) {
  if (m_screen != NULL) {
    m_screen->setVisible (false);
    m_screen->setParent (NULL);
    delete m_screen;
  }
  m_screen = screen;
  screen->setParent (this);
  screen->setVisible (true);
}
