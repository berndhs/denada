#ifndef DELIBERATE_H
#define DELIBERATE_H

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
 
#include <qapplication.h>
#include <stdio.h>
#include <QTextStream>
#include <QSettings>

#define DELIBERATE_QT_NUM (((DELIBERATE_QTM1)*10000)+((DELIBERATE_QTM2)*100)+(DELIBERATE_QTP))
#if DELIBERATE_QT_NUM > 40600
#define DELIBERATE_HAVE_WEBELT 1
#else
#define DELIBERATE_HAVE_WEBELT 0
#endif

namespace deliberate {


QTextStream  & StdOut();

void SetSettings (QSettings & settings);

void InitSettings ();

QSettings & Settings ();

bool IsMaemo ();

bool IsFingerInterface ();

bool IsIp6Address (QString addr);
bool IsIp4Address (QString addr);

void Rot1 (QByteArray & data, const QByteArray & key);
void Rot2 (QByteArray & data, const QByteArray & key);
}


#endif
