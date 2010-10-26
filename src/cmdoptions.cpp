#include "cmdoptions.h"
#include <iostream>
#include <QObject>

//
//  Copyright (C) 2009 - Bernd H Stramm 
//

/****************************************************************
 * This file is distributed under the following license:
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
using namespace std;

namespace deliberate {

QString CmdOptions::trHelp (QObject::tr("help"));
QString CmdOptions::trH    (QObject::tr("h"));
QString CmdOptions::trVersion (QObject::tr("version"));
QString CmdOptions::trV       (QObject::tr("v"));

CmdOptions::CmdOptions (QString pgmname)
   :mPgm("")
{
  bad = false;
  mPgm = pgmname;
  AddSoloOption (trHelp,
                 trH,
                 QObject::tr("Give help message and exit"));
  AddSoloOption (trVersion,
                 trV,
                 QObject::tr("Give version and exit"));
	;
}

QStringList
CmdOptions::Arguments ()
{
  return mArgs;
}

void
CmdOptions::Usage ()
{
  cout << mPgm.toStdString() << QObject::tr(":").toStdString() << endl;
  OptValuesType::iterator it;
  string tab = QObject::tr("\t\t").toStdString();
  for (it = mOptValues.begin(); it != mOptValues.end(); it++) {
    Option *opt = it->second;
    cout << QObject::tr("--").toStdString() << opt->longName.toStdString()
         << QObject::tr(" ( -").toStdString() << opt->shortName.toStdString() 
         << QObject::tr(") ").toStdString(); 
    if (opt->theType != Opt_Solo) {
      cout << QObject::tr(" value  ").toStdString();
    }
    cout << tab << opt->theMsg.toStdString()    
         << endl;
  }
}

bool
CmdOptions::Parse (int argc, char *argv[])
{
  try {
    int a(1);
    bool keepParsing (true);
    QString word;
    while (a < argc) {
      word = QString (argv[a]);
      if (word == "--") {
        keepParsing = false;
      } else if (keepParsing && word.startsWith("--")) {
        ParseLong (word, argv, a, argc+1);
      } else if (keepParsing && word.startsWith("-")) {
        ParseShort (word, argv, a, argc+1);
      } else {
        mArgs += word;
      }
      a++;
    }
  } catch (...) {
    bad = true;
    return false;
  }
  bad = false;
  return true;  
}

void
CmdOptions::ParseLong (QString wd, char *argv[], int & pos, int posmax)
{
  int len = wd.length();
  if (len < 3) {
    throw -1;
  }
  QString name = wd.right(len-2);
  if (mOptValues.count(name) < 1) { // invalid option
    throw -1;
  }
  Option * opt = mOptValues[name];
  if (opt == 0) { // corrupt option map
    throw -2;
  }
  opt->seenIt = true;
  if (opt->theType == Opt_Solo) {
    opt->theValue.append (QVariant(true));
    return;
  } else {
    pos++;
    if (pos > posmax) {
      throw -1;
    }
    QString value(argv[pos]);
    switch (opt->theType) {
    case Opt_Int:
      opt->theValue.append (QVariant (value.toInt()));
      break;
    case Opt_QStr:
      opt->theValue.append (QVariant (value));
      break;
    default: // corrupt option map
      throw -2;
    }
  }
}


void
CmdOptions::ParseShort (QString wd, char* argv[], int & pos, int posmax)
{
  int len = wd.length();
  if (len < 2) {
    throw -1;
  }
  QString name = wd.right(len-1);
  if (mLongNameOf.count(name) < 1) {
    throw -1;
  }
  QString lngWd ("--");
  lngWd.append(mLongNameOf[name]);
  ParseLong (lngWd, argv, pos, posmax);
}

CmdOptions::Option *
CmdOptions::AddOption (QString longName, 
                      QString shortName, 
                      QString msg)
{
  if (shortName.length() > 1) {
    throw -3;
  }
  Option * thisopt = new Option;
  if (thisopt == 0) {
    throw -2;
  }
  thisopt->seenIt = false;
  thisopt->theMsg = msg;
  thisopt->longName = longName;
  thisopt->shortName = shortName;
  mLongNameOf[shortName] = longName;
  mOptValues[longName] = thisopt;
  return thisopt;
}

void
CmdOptions::AddStrOption (QString longName,
                          QString shortName,
                          QString msg)
{
  Option * thisopt = AddOption (longName, shortName, msg);
  thisopt->theType = Opt_QStr;
}

void
CmdOptions::AddIntOption (QString longName,
                          QString shortName,
                          QString msg)
{
  Option * thisopt = AddOption (longName, shortName, msg);
  thisopt->theType = Opt_Int;
}

void
CmdOptions::AddSoloOption (QString longName,
                          QString shortName,
                          QString msg)
{
  Option * thisopt = AddOption (longName, shortName, msg);
  thisopt->theType = Opt_Solo;
}

int
CmdOptions::SeenOpt (QString longName)
{
  if (mOptValues.count(longName) > 0) {
    if (mOptValues[longName]->seenIt) {
      return mOptValues[longName]->theValue.size();
    } else {
      return 0;
    }
  } 
  return false;
}

QList<QVariant> 
CmdOptions::ValueList (QString longName)
{
  QList <QVariant> results;
  if (SeenOpt (longName)) {
    results = mOptValues[longName]->theValue;
  }
  return results;
}

bool
CmdOptions::SetStringOpt (QString longName, QString & opt)
{
  bool seenOpt = SeenOpt (longName);
  if (seenOpt) {
    opt = mOptValues[longName]->theValue.last().toString();
    return true;
  }
  return false;
}

bool
CmdOptions::SetIntOpt (QString longName, int & opt)
{
  bool seenOpt = SeenOpt(longName);
  if (seenOpt) {
    opt = mOptValues[longName]->theValue.last().toInt();
    return true;
  }
  return false;
}

bool
CmdOptions::SetSoloOpt (QString longName, bool & seenIt)
{
  seenIt = SeenOpt(longName);
  return seenIt;
}

bool
CmdOptions::WantHelp ()
{
  return SeenOpt(trHelp);
}

bool
CmdOptions::WantVersion ()
{
  return SeenOpt(trVersion);
}

} // namespace
