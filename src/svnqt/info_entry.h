/***************************************************************************
 *   Copyright (C) 2006-2009 by Rajko Albrecht                             *
 *   ral@alwins-world.de                                                   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or           *
 * modify it under the terms of the GNU Lesser General Public              *
 * License as published by the Free Software Foundation; either            *
 * version 2.1 of the License, or (at your option) any later version.      *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * Lesser General Public License for more details.                         *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this program (in the file LGPL.txt); if not,         *
 * write to the Free Software Foundation, Inc., 51 Franklin St,            *
 * Fifth Floor, Boston, MA  02110-1301  USA                                *
 *                                                                         *
 * This software consists of voluntary contributions made by many          *
 * individuals.  For exact contribution history, see the revision          *
 * history and logs, available at http://kdesvn.alwins-world.de.           *
 ***************************************************************************/
#ifndef SVNQT_INFO_ENTRY_H
#define SVNQT_INFO_ENTRY_H

#include <svnqt/helper.h>
#include <svnqt/lock_entry.h>
#include <svnqt/datetime.h>
#include <svnqt/revision.h>
#include <svnqt/svnqttypes.h>
#include <svnqt/conflictdescription.h>

#include <QString>

#include <svn_version.h>

#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,7,0)
#undef SVN_INFO_SIMPLE_CONFLICT_TYPE
struct svn_client_info2_t;
#else
#define SVN_INFO_SIMPLE_CONFLICT_TYPE 1
struct svn_info_t;
#endif

namespace svn
{
class SVNQT_EXPORT InfoEntry
{
public:
    static const qlonglong SVNQT_SIZE_UNKNOWN = -1;
    InfoEntry();
#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,7,0)
    InfoEntry(const svn_client_info2_t *, const char *path);
    InfoEntry(const svn_client_info2_t *, const QString &path);
#else
    InfoEntry(const svn_info_t *, const char *path);
    InfoEntry(const svn_info_t *, const QString &path);
#endif
    InfoEntry(const InfoEntry &);
    ~InfoEntry();

#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,7,0)
    void init(const svn_client_info2_t *, const char *path);
    void init(const svn_client_info2_t *, const QString &path);
#else
    void init(const svn_info_t *, const char *path);
    void init(const svn_info_t *, const QString &path);
#endif

    DateTime cmtDate()const;
    DateTime textTime()const;
    DateTime propTime()const;
    bool hasWc()const;
    /**
     * @return lock for that entry
     * @since subversion 1.2
     */
    const LockEntry &lockEntry()const;
    /**
     * @return last commit author of this file
     */
    const QString &cmtAuthor() const;
    const QString &Name()const;

    const QString &checksum()const;

#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,7,0)
    const ConflictDescriptionList &conflicts()const;
#else
    const QString &conflictNew()const;
    const QString &conflictOld()const;
    const QString &conflictWrk()const;
#endif

    const QString &copyfromUrl()const;
    const QString &prejfile()const;
    const QString &reposRoot()const;
    const QString &url()const;
    const QString &uuid()const;
    svn_node_kind_t kind()const;
    const Revision &cmtRev()const;
    const Revision &copyfromRev()const;
    const Revision &revision()const;
    svn_wc_schedule_t Schedule()const;

    qlonglong size()const;
    qlonglong working_size()const;
    const QByteArray &changeList()const;
    svn::Depth depth()const;

    const QString &prettyUrl()const;

    bool isDir()const;
    QString prettyUrl(const char *)const;

protected:
    DateTime m_last_changed_date;
    DateTime m_text_time;
    DateTime m_prop_time;
    bool m_hasWc;
    LockEntry m_Lock;
    QString m_name;
    QString m_checksum;
#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,7,0)
    ConflictDescriptionList m_conflicts;
#else
    QString m_conflict_new;
    QString m_conflict_old;
    QString m_conflict_wrk;
#endif
    QString m_copyfrom_url;
    QString m_last_author;
    QString m_prejfile;
    QString m_repos_root;
    QString m_url;
    QString m_pUrl;
    QString m_UUID;
    svn_node_kind_t m_kind;
    Revision m_copy_from_rev;
    Revision m_last_changed_rev;
    Revision m_revision;
    svn_wc_schedule_t m_schedule;

    qlonglong m_size;
    qlonglong m_working_size;
    QByteArray m_changeList;
    svn::Depth m_depth;

protected:
    void init();
};
}
#endif
