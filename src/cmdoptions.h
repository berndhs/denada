#ifndef CMDOPTIONS_H
#define CMDOPTIONS_H

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

#include <QString>
#include <QVariant>
#include <QStringList>
#include <QList>
#include <map>

using namespace std;


namespace deliberate {


class CmdOptions {

public:

enum OptType { Opt_None,
               Opt_Int,
               Opt_QStr,
               Opt_Solo,
               Opt_Bad
             };

CmdOptions (QString pgmname);

bool Parse (int argc, char*argv[]);
bool Bad ();

QStringList Arguments ();

void Usage ();

bool WantHelp ();
bool WantVersion ();

void AddStrOption  (QString longName, 
                    QString shortName, 
                    QString msg);
void AddStringOption (QString longName, 
                    QString shortName, 
                    QString msg)
                    { AddStrOption (longName, shortName, msg); }
void AddIntOption  (QString longName, 
                    QString shortname, 
                    QString msg);
void AddSoloOption (QString longName, 
                    QString shortname, 
                    QString msg);

/** @brief these SetSomething functions set the option variable
  * if it was specified on the command line, otherwise they leave
  * the parameter alone. The bool return says whether or not the 
  * parameter was changed
  */
  
int  SeenOpt (QString longName);
bool SetStringOpt (QString longName, QString & opt);
bool SetIntOpt    (QString longName, int & opt);
bool SetSoloOpt   (QString longName, bool & seenIt);

QList<QVariant> ValueList (QString longName);

private:

   class Option {
    
    public:
    
       OptType           theType;
       QList<QVariant>   theValue;
       QString           theMsg;
       QString           longName;
       QString           shortName;
       bool              seenIt;
   };

   
   void PrintAll ();
   
   void ParseLong (QString wd, char* argv[], int & pos, int posmax);
   void ParseShort (QString wd, char* argv[], int & pos, int posmax);
   
   typedef map <QString, Option*> OptValuesType;
   typedef map <QString, QString> ShortToLongType;
   
   OptValuesType   mOptValues;
   ShortToLongType mLongNameOf;
   
   QStringList   mArgs;
   
   
   QString mPgm;
   
   bool    bad;
   
   bool Seen (QString name);
   Option * AddOption  (QString longName, 
                        QString shortname, 
                        QString msg);
   
   static QString  trHelp;
   static QString  trH;
   static QString  trVersion;
   static QString  trV;
};


}


#endif
