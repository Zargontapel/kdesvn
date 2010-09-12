/***************************************************************************
 * DialogStack.cpp                                                            *
 *   Copyright (C) 2005-2010 by Rajko Albrecht  ral@alwins-world.de        *                                                                                
 *   http://kdesvn.alwins-world.de/                                        *                                                                                
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

#include "DialogStack.h"
#include <kdialog.h>

DialogStack::DialogStack(KDialog*aDlg,const KConfigGroup&group)
{
	_dlg = aDlg;
	_k = group;

}

DialogStack::~DialogStack()
{
	if (_dlg) {
        _dlg->saveDialogSize(_k);
        delete _dlg;
	}
}