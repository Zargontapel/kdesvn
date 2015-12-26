/*
 * Port for usage with qt-framework and development for kdesvn
 * Copyright (C) 2005-2009 by Rajko Albrecht (ral@alwins-world.de)
 * http://kdesvn.alwins-world.de
 */
/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 * dev@rapidsvn.tigris.org
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library (in the file LGPL.txt); if not,
 * write to the Free Software Foundation, Inc., 51 Franklin St,
 * Fifth Floor, Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */


// svncpp
#include "client_impl.h"
#include "helper.h"

// Subversion api
#include <svn_client.h>
#include <svn_sorts.h>
#include <svn_path.h>

#include "dirent.h"
#include "exception.h"
#include "pool.h"
#include "status.h"
#include "targets.h"
#include "info_entry.h"
#include "url.h"
#include "svnqt_defines.h"
#include "context_listener.h"
#include "client_parameter.h"
#include <QCoreApplication>

namespace svn
{

static svn_error_t *
logMapReceiver2(
    void *baton,
    svn_log_entry_t *log_entry,
    apr_pool_t *pool
)
{
    Q_UNUSED(pool);
    Client_impl::sBaton *l_baton = (Client_impl::sBaton *)baton;
    LogEntriesMap *entries =
        (LogEntriesMap *) l_baton->m_data;
    ContextP l_context = l_baton->m_context;
    if (l_context.isNull()) {
        return SVN_NO_ERROR;
    }
    QList<qlonglong> *rstack =
        (QList<qlonglong> *)l_baton->m_revstack;
    svn_client_ctx_t *ctx = l_context->ctx();
    if (ctx && ctx->cancel_func) {
        SVN_ERR(ctx->cancel_func(ctx->cancel_baton));
    }
    if (! SVN_IS_VALID_REVNUM(log_entry->revision)) {
        if (rstack && !rstack->isEmpty()) {
            rstack->pop_front();
        }
        return SVN_NO_ERROR;
    }
    (*entries)[log_entry->revision] = LogEntry(log_entry, (l_baton->excludeList ? * (l_baton->excludeList) : svn::StringArray()));
    /// @TODO insert it into last logentry
    if (rstack) {
        (*entries)[log_entry->revision].m_MergedInRevisions = (*rstack);
        if (log_entry->has_children) {
            rstack->push_front(log_entry->revision);
        }
    }
    return SVN_NO_ERROR;
}

struct StatusEntriesBaton {
    StatusEntries entries;
    apr_pool_t *pool;
    ContextWP m_Context;
    StatusEntriesBaton(): entries(), pool(0)
    {}
};

struct InfoEntriesBaton {
    InfoEntries entries;
    apr_pool_t *pool;
    ContextWP m_Context;
    InfoEntriesBaton(): entries(), pool(0)
    {}
};

static svn_error_t *InfoEntryFunc(void *baton,
                                  const char *path,
#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,7,0)
                                  const svn_client_info2_t *info,
#else
                                  const svn_info_t *info,
#endif
                                  apr_pool_t *)
{
    InfoEntriesBaton *seb = static_cast<InfoEntriesBaton *>(baton);
    if (seb->m_Context) {
        /* check every loop for cancel of operation */
        ContextP l_context = seb->m_Context;
        if (!l_context) {
            return svn_error_create(SVN_ERR_CANCELLED, 0, QCoreApplication::translate("svnqt", "Cancelled by user.").toUtf8());
        }
        svn_client_ctx_t *ctx = l_context->ctx();
        if (ctx && ctx->cancel_func) {
            SVN_ERR(ctx->cancel_func(ctx->cancel_baton));
        }
    }
    seb->entries.push_back(InfoEntry(info, path));
    return NULL;
}

#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,6,0)
static svn_error_t *StatusEntriesFunc(void *baton,
                                      const char *path,
                                      svn_wc_status2_t *status,
                                      apr_pool_t *pool)
{
    // use own pool - the parameter will cleared between loops!
    Q_UNUSED(pool);
    StatusEntriesBaton *seb = static_cast<StatusEntriesBaton *>(baton);
    if (seb->m_Context) {
        /* check every loop for cancel of operation */
        ContextP l_context = seb->m_Context;
        if (!l_context) {
            return svn_error_create(SVN_ERR_CANCELLED, 0, QCoreApplication::translate("svnqt", "Cancelled by user.").toUtf8());
        }
        svn_client_ctx_t *ctx = l_context->ctx();
        if (ctx && ctx->cancel_func) {
            SVN_ERR(ctx->cancel_func(ctx->cancel_baton));
        }
    }

    seb->entries.push_back(StatusPtr(new Status(path, status)));
    return NULL;
}
#else
static void StatusEntriesFunc(void *baton,
                              const char *path,
                              svn_wc_status2_t *status)
{
    StatusEntriesBaton *seb = static_cast<StatusEntriesBaton *>(baton);
    seb->entries.push_back(StatusPtr(new Status(path, status)));
}
#endif

static StatusEntries
localStatus(const StatusParameter &params,
            const ContextP &context)
{
    svn_error_t *error;
    svn_revnum_t revnum;
    Revision rev(Revision::HEAD);
    Pool pool;
    StatusEntriesBaton baton;

    baton.pool = pool;

#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,6,0)
    error = svn_client_status4(
#else
    error = svn_client_status3(
#endif
                & revnum,          // revnum
                params.path().path().toUtf8(),         // path
                rev,
                StatusEntriesFunc, // status func
                &baton,            // status baton
                internal::DepthToSvn(params.depth()), // see svn::Depth
                params.all(),           // get all not only interesting
                params.update(),            // check for updates
                params.noIgnore(),         // hide ignored files or not
                params.ignoreExternals(),    // hide external
                params.changeList().array(pool),
                *context,          //client ctx
                pool);

    Client_impl::checkErrorThrow(error);
    return baton.entries;
}

static StatusPtr
dirEntryToStatus(const Path &path, const DirEntry &dirEntry)
{
    QString url = path.path() + QLatin1Char('/') + dirEntry.name();
    return StatusPtr(new Status(url, dirEntry));
}

static StatusPtr
infoEntryToStatus(const Path &, const InfoEntry &infoEntry)
{
    return StatusPtr(new Status(infoEntry.url(), infoEntry));
}

static StatusEntries
remoteStatus(Client *client,
             const StatusParameter &params,
             const ContextP &)
{
    DirEntries dirEntries = client->list(params.path(), params.revision(), params.revision(), params.depth(), params.detailedRemote());
    DirEntries::const_iterator it;

    StatusEntries entries;
    for (it = dirEntries.constBegin(); it != dirEntries.constEnd(); ++it) {
        DirEntry dirEntry = *it;
        if (dirEntry.name().isEmpty()) {
            continue;
        }
        entries.push_back(dirEntryToStatus(params.path(), dirEntry));
    }
    return entries;
}

StatusEntries
Client_impl::status(const StatusParameter &params) throw (ClientException)
{
    if (Url::isValid(params.path().path())) {
        return remoteStatus(this, params, m_context);
    }
    return localStatus(params, m_context);
}

static StatusPtr
localSingleStatus(const Path &path, const ContextP &context, bool update = false)
{
    svn_error_t *error;
    Pool pool;
    StatusEntriesBaton baton;
    svn_revnum_t revnum;
    Revision rev(Revision::HEAD);

    baton.pool = pool;

#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,6,0)
    error = svn_client_status4(
#else
    error = svn_client_status3(
#endif
                & revnum,          // revnum
                path.path().toUtf8(),         // path
                rev,
                StatusEntriesFunc, // status func
                &baton,            // status baton
                svn_depth_empty, // not recurse
                true,           // get all not only interesting
                update,         // check for updates
                false,          // hide ignored files or not
                false,          // hide external
                0,
                *context,          //client ctx
                pool);
    Client_impl::checkErrorThrow(error);
    if (baton.entries.isEmpty()) {
        return StatusPtr(new Status());
    }

    return baton.entries.at(0);
}

static StatusPtr
remoteSingleStatus(Client *client, const Path &path, const Revision revision, const ContextP &)
{
    const InfoEntries infoEntries = client->info(path, DepthEmpty, revision, Revision(Revision::UNDEFINED));
    if (infoEntries.isEmpty()) {
        return StatusPtr(new Status());
    }
    return infoEntryToStatus(path, infoEntries.at(0));
}

StatusPtr
Client_impl::singleStatus(const Path &path, bool update, const Revision revision) throw (ClientException)
{
    if (Url::isValid(path.path())) {
        return remoteSingleStatus(this, path, revision, m_context);
    }
    return localSingleStatus(path, m_context, update);
}

bool
Client_impl::log(const LogParameter &params, LogEntriesMap &log_target) throw (ClientException)
{
    Pool pool;
    sBaton l_baton;
    QList<qlonglong> revstack;
    l_baton.m_context = m_context;
    l_baton.m_data = &log_target;
    l_baton.m_revstack = &revstack;
    l_baton.excludeList = & (params.excludeList());
    svn_error_t *error;

#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,6,0)
    error = svn_client_log5(
                params.targets().array(pool),
                params.peg().revision(),
                svn::internal::RevisionRangesToHash(params.revisions()).array(pool),
                params.limit(),
                params.discoverChangedPathes() ? 1 : 0,
                params.strictNodeHistory() ? 1 : 0,
                params.includeMergedRevisions() ? 1 : 0,
                params.revisionProperties().array(pool),
                logMapReceiver2,
                &l_baton,
                *m_context, // client ctx
                pool);
#else
    error = svn_client_log4(
                params.targets().array(pool),
                params.peg().revision(),
                params.revisionRange().first.revision(),
                params.revisionRange().second.revision(),
                params.limit(),
                params.discoverChangedPathes() ? 1 : 0,
                params.strictNodeHistory() ? 1 : 0,
                params.includeMergedRevisions() ? 1 : 0,
                params.revisionProperties().array(pool),
                logMapReceiver2,
                &l_baton,
                *m_context, // client ctx
                pool);
#endif
    checkErrorThrow(error);
    return true;
}

InfoEntries
Client_impl::info(const Path &_p,
                  Depth depth,
                  const Revision &rev,
                  const Revision &peg_revision,
                  const StringArray &changelists
                 ) throw (ClientException)
{

    Pool pool;
    svn_error_t *error = NULL;
    InfoEntriesBaton baton;

    baton.pool = pool;
    baton.m_Context = m_context;
    svn_opt_revision_t pegr;
    const char *truepath = 0;
    bool internal_peg = false;
    QByteArray _buf = _p.cstr();

    error = svn_opt_parse_path(&pegr, &truepath,
                               _buf,
                               pool);
    checkErrorThrow(error);
    if (peg_revision.kind() == svn_opt_revision_unspecified) {
        if ((svn_path_is_url(_p.cstr())) && (pegr.kind == svn_opt_revision_unspecified)) {
            pegr.kind = svn_opt_revision_head;
            internal_peg = true;
        }
    }

    error =
#if SVN_API_VERSION >= SVN_VERSION_CHECK(1,7,0)
        svn_client_info3
        (truepath,
         internal_peg ? &pegr : peg_revision.revision(),
         rev.revision(),
         internal::DepthToSvn(depth),
         false, // TODO parameter for fetch exclueded
         false, // TODO parameter for fetch_actual_only
         changelists.array(pool),
         &InfoEntryFunc,
         &baton,
         *m_context,
         pool);
#else
        svn_client_info2
        (truepath,
         internal_peg ? &pegr : peg_revision.revision(),
         rev.revision(),
         &InfoEntryFunc,
         &baton,
         internal::DepthToSvn(depth),
         changelists.array(pool),
         *m_context,    //client ctx
         pool);
#endif

    checkErrorThrow(error);
    return baton.entries;
}

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
