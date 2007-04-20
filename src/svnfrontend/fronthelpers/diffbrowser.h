/***************************************************************************
 *   Copyright (C) 2007 by Rajko Albrecht                                  *
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
#ifndef _DIFF_BROWSER_H
#define _DIFF_BROWSER_H

#include <qtextbrowser.h>

class DiffSyntax;
class KEdFind;

class DiffBrowser : public QTextBrowser
{
    Q_OBJECT

public:
    DiffBrowser(QWidget*parent=0,const char*name=0);
    virtual ~DiffBrowser();

public slots:
    virtual void setText(const QString&aText);
    virtual void setText(const QByteArray&ex);
    virtual void saveDiff();

protected:
    virtual void keyPressEvent(QKeyEvent*);

    void startSearch();

protected:
    DiffSyntax*m_Syntax;
    QByteArray m_content;
    KEdFind   *srchdialog;
};

#endif
