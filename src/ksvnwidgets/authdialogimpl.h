/***************************************************************************
 *   Copyright (C) 2005 by Rajko Albrecht                                  *
 *   ral@alwins-world.de                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef AUTHDIALOGIMPL_H
#define AUTHDIALOGIMPL_H

#include "src/ksvnwidgets/authdlg.h"
#include <qstring.h>

class AuthDialogImpl: public AuthDialogData {
Q_OBJECT
public:
    AuthDialogImpl(const QString & realm = "",const QString&user="", QWidget *parent = 0, const char *name = 0);

    const QString Username()const;
    const QString Password();
    bool maySave()const;
protected slots:
    virtual void slotHelp();
protected:
    QString curPass;
};

#endif