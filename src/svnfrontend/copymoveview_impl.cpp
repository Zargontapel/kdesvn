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


#include "copymoveview_impl.h"
#include <ksqueezedtextlabel.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <klineedit.h>
#include <klocale.h>
#include <kdialogbase.h>
#include <qvbox.h>

CopyMoveView_impl::CopyMoveView_impl(const QString&baseName,const QString&sourceName,bool move,QWidget* parent, const char* name, WFlags fl)
: CopyMoveView(parent,name,fl)
{
    m_BaseName = baseName;
    if (!m_BaseName.endsWith("/")) {
        m_BaseName+="/";
    }
    m_PrefixLabel->setText(m_BaseName);
    m_OldNameLabel->setText("<b>"+sourceName+"</b>");
    m_OldName = sourceName;
    QString t = m_OldName.right(m_OldName.length()-m_BaseName.length());
    m_NewNameInput->setText(t);
    if (move) {
        m_HeadOneLabel->setText(i18n("Rename/move"));
    } else {
        m_HeadOneLabel->setText(i18n("Copy"));
    }
}

CopyMoveView_impl::~CopyMoveView_impl()
{
}

/*$SPECIALIZATION$*/


#include "copymoveview_impl.moc"



/*!
    \fn CopyMoveView_impl::newName()
 */
QString CopyMoveView_impl::newName()
{
    return m_BaseName+m_NewNameInput->text();
}


/*!
    \fn CopyMoveView_impl::force()
 */
bool CopyMoveView_impl::force()
{
    return m_ForceBox->isChecked();
}


/*!
    \fn CopyMoveView_impl::getMoveCopyTo(bool*ok,bool*force,const QString&old,const QString&base,QWidget*)
 */
QString CopyMoveView_impl::getMoveCopyTo(bool*ok,bool*force,bool move,
    const QString&old,const QString&base,QWidget*parent,const char*name)
{
    KDialogBase dlg(parent,name,true,(move?i18n("Move/Rename file/dir"):i18n("Copy file/dir")),
            KDialogBase::Ok|KDialogBase::Cancel,
            KDialogBase::NoDefault);
    QWidget* Dialog1Layout = dlg.makeVBoxMainWidget();
    CopyMoveView_impl*ptr=new CopyMoveView_impl(base,old,(move),Dialog1Layout);
    QString nName = QString::null;
    dlg.resize( QSize(500,160).expandedTo(dlg.minimumSizeHint()) );
    if (dlg.exec()!=QDialog::Accepted) {
        if (ok) *ok = false;
    } else {
        if (force) *force = ptr->force();
        nName = ptr->newName();
        if (ok) *ok=true;
    }
    return nName;
}
