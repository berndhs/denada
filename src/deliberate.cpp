#include "deliberate.h"


/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2010, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/
 
#include <QStringList>
#include <iostream>
#include <stdlib.h>

namespace deliberate {

QTextStream & StdOut ()
{
  static QTextStream *out(0);
  
  if (out == 0) {
    out = new QTextStream (stdout);
  }
  return *out;
}


static QSettings * mySettings(0);
  
void
InitSettings ()
{
  mySettings = 0;
}

void
SetSettings (QSettings & settings)
{
  if (mySettings) {
    delete mySettings;
  }
  mySettings = &settings;
}

QSettings &
Settings ()
{
  if (mySettings == 0) {
    mySettings = new QSettings;
  }
  if (mySettings == 0) {
    std::cerr << "Cannot allocate Settings, have to quit" << std::endl;
    abort ();
  }
  return *mySettings;
}

bool
IsMaemo ()
{
  #ifdef Q_WS_MAEMO_5
  return true;
  #else
  return false;
  #endif
}

bool
IsFingerInterface ()
{
  return IsMaemo ();
}

void
Rot1 (QByteArray &data, const QByteArray &key)
{
  int len (key.size());
  for (int c = 0; c<data.size(); c++) {
    int cc = data[c];
    int k = c % len;
    data[c] = (cc ^ key[k]);
  }
  data = data.toBase64();
}

void
Rot2 (QByteArray &data, const QByteArray &key)
{
  data = QByteArray::fromBase64(data);
  int len (key.size());
  for (int c = 0; c<data.size(); c++) {
    int cc = data[c];
    int k = c % len;
    data[c] = (cc ^ key[k]);
  }
}

bool
IsIp6Address (QString addr)
{
  bool isnum (false);
  QStringList parts = addr.split (':',QString::SkipEmptyParts);
  for (int p = 0; p < parts.size(); p++) {
    parts.at(p).toUInt (&isnum, 16);
    if (!isnum) {
      return false;
    }
  }
  return true;
}


bool
IsIp4Address (QString addr)
{
  bool isnum (false);
  QStringList parts = addr.split ('.',QString::SkipEmptyParts);
  if (parts.size() != 4) {
    return false;
  }
  for (int p = 0; p < parts.size(); p++) {
    parts.at(p).toUInt (&isnum, 10);
    if (!isnum) {
      return false;
    }
  }
  return true;
}


}
