/***************************************************************************
 *   Copyright (C) 2006 by Rajko Albrecht                                  *
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
#include "kiobytestream.h"

KioByteStream::KioByteStream(StreamWrittenCb*aCb,const QString&filename)
    : svn::stream::SvnStream(false,true,0L),
    m_Cb(aCb),m_Written(0),
    m_mimeSend(false),m_Filename(filename)
{
    m_MessageTick.start();
}

KioByteStream::~KioByteStream()
{
}

bool KioByteStream::isOk() const
{
    return m_Cb != 0;
}

long KioByteStream::write(const char* data, const unsigned long max)
{
    bool forceInfo = !m_mimeSend;
    if (m_Cb) {
        if (!m_mimeSend) {
            m_mimeSend = true;
            array.setRawData(data, max);
            KMimeMagicResult * result = KMimeMagic::self()->findBufferFileType(array,m_Filename);
            m_Cb->streamSendMime(result);
            array.resetRawData(data, max);
            m_Cb->streamTotalSizeNull();
        }
        array.setRawData(data, max);
        m_Cb->streamPushData(array);
        array.resetRawData(data, max);

        m_Written+=max;
        if (m_MessageTick.elapsed() >=100 || forceInfo) {
            m_Cb->streamWritten(m_Written);
            m_MessageTick.restart();
        }
        return max;
    }
    return -1;
}