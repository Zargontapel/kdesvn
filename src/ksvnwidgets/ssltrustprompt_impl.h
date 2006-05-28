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
#ifndef SSLTRUSTPROMPT_IMPL_H
#define SSLTRUSTPROMPT_IMPL_H

#include "src/ksvnwidgets/ssltrustprompt.h"

class SslTrustPrompt_impl: public SslTrustPrompt {
Q_OBJECT
public:
    SslTrustPrompt_impl(const QString&,QWidget *parent = 0, const char *name = 0);
    static bool sslTrust(const QString&host,const QString&fingerprint,const QString&validFrom,const QString&validUntil,const QString&issuerName,const QString&realm,const QStringList&reasons,bool*ok,bool*saveit);
};

#endif