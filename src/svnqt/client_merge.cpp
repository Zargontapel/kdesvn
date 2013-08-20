/***************************************************************************
 *   Copyright (C) 2005-2009 by Rajko Albrecht  ral@alwins-world.de        *
 *   http://kdesvn.alwins-world.de/                                        *
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
#include "svnqt/client_impl.h"

// subversion api
#include "svn_client.h"

#include "svnqt/exception.h"
#include "svnqt/pool.h"
#include "svnqt/targets.h"
#include "svnqt/svnqt_defines.h"
#include "svnqt/stringarray.h"

#include "svnqt/helper.h"
#include "svnqt/client_parameter.h"

#include <QDebug>

namespace svn
{

void Client_impl::merge_reintegrate(const MergeParameter&parameters) throw (ClientException)
{
    Pool pool;
    svn_error_t * error = 0;
    error = svn_client_merge_reintegrate(parameters.path1().cstr(),
                                         parameters.peg().revision(),
                                         parameters.localPath().cstr(),
                                         parameters.dry_run(),
                                         parameters.merge_options().array(pool),
                                         *m_context,
                                         pool
                                         );
    if(error != 0) {
        throw ClientException (error);
    }
}

void Client_impl::merge(const MergeParameter&parameters) throw (ClientException)
{
    Pool pool;
    svn_error_t * error = 0;
    if (parameters.reintegrate()) {
        merge_reintegrate(parameters);
    } else {
        error =
#if ((SVN_VER_MAJOR == 1) && (SVN_VER_MINOR >= 7)) || (SVN_VER_MAJOR > 1)
            svn_client_merge4
#else
            svn_client_merge3
#endif
                (parameters.path1().cstr (),
                                  parameters.revision1().revision(),
                                  parameters.path2().cstr(),
                                  parameters.revision2().revision(),
                                  parameters.localPath().cstr (),
                                  internal::DepthToSvn(parameters.depth()),
                                  !parameters.notice_ancestry(),
                                  parameters.force(),
                                  parameters.record_only(),
                                  parameters.dry_run(),
#if ((SVN_VER_MAJOR == 1) && (SVN_VER_MINOR >= 7)) || (SVN_VER_MAJOR > 1)
                                  parameters.allow_mixed_rev(),
#endif
                                  parameters.merge_options().array(pool),
                                  *m_context,
                                  pool);
    }

    if(error != 0) {
        throw ClientException (error);
    }
  }

  void Client_impl::merge_peg(const MergeParameter&parameters) throw (ClientException)
  {
      Pool pool;
      internal::RevisionRangesToHash _rhash(parameters.revisions());

      svn_error_t*error;

      error = 
#if ((SVN_VER_MAJOR == 1) && (SVN_VER_MINOR >= 7)) || (SVN_VER_MAJOR > 1)
            svn_client_merge_peg4
#else
            svn_client_merge_peg3
#endif
                                (
                                    parameters.path1().cstr(),
                                    _rhash.array(pool),
                                    parameters.peg(),
                                    parameters.localPath().cstr(),
                                    internal::DepthToSvn(parameters.depth()),
                                    !parameters.notice_ancestry(),
                                    parameters.force(),
                                    parameters.record_only(),
                                    parameters.dry_run(),
#if ((SVN_VER_MAJOR == 1) && (SVN_VER_MINOR >= 7)) || (SVN_VER_MAJOR > 1)
                                    parameters.allow_mixed_rev(),
#endif
                                    parameters.merge_options().array(pool),
                                    *m_context,
                                    pool
                                   );
      if(error != 0) {
          throw ClientException (error);
      }
  }
}
