/***************************************************************************
 *   Copyright (C) 2005-2009 by Rajko Albrecht                             *
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
/*!
 * \file client_parameter.hpp
 * \brief defining classes working as named parameters for different subversion methods
 *
 * Subversion has various functions which has growing / changing parameter lists from version to version.
 * since subversion 1.4 this changes are more and more unhandy for a c++ wrapper due every time changes to
 * virtual class. This special data containers may reduce changes of signatures to the client interface.
 */

#ifndef CLIENT_PARAMETER_H
#define CLIENT_PARAMETER_H

#include "svnqt/svnqt_defines.hpp"
#include "svnqt/svnqttypes.hpp"
#include "svnqt/revision.hpp"
#include "svnqt/targets.hpp"
#include "svnqt/path.hpp"
#include "svnqt/shared_pointer.hpp"

namespace svn
{

    struct CopyParameterData;
    struct DiffParameterData;

    //! parameter for svn_copy wrapper
    /*!
     * This class should never contains virtual methods. New Methods are always appended at end of class definition
     * \sa svn::Client::copy
     */
    class SVNQT_EXPORT CopyParameter
    {
    private:
        SharedPointer<CopyParameterData> _data;
    public:
        CopyParameter(const Targets&_srcPath,const Path&_destPath);
        ~CopyParameter();

        //! Targets for copy operation
        const Targets&srcPath()const;
        //! Destination path for copy operation
        const Path&destination()const;

        //! set copy operation parameter asChild to true
        CopyParameter&asChild(bool);
        //! return value for asChild
        bool getAsChild()const;

        //! copy should ignore externals
        /*!
         * \since subversion 1.6
         */
        CopyParameter&ignoreExternal(bool);
        //! return externals has to ignored
        /*!
         * \since subversion 1.6
         */
        bool getIgnoreExternal()const;

        //! set copy/move operation parameter makeParent to true
        CopyParameter&makeParent(bool);
        //! return value for asChild
        bool getMakeParent()const;

        //! set move operation parameter force to true
        /*! this is ignored for copy operation */
        CopyParameter&force(bool);
        //! return value for force
        /*! this is ignored for copy operation */
        bool getForce()const;

        //! set the source revision for the copy operation
        CopyParameter&srcRevision(const Revision&);
        //! get the source revision for the copy operation
        const Revision& getSrcRevision()const;
        //! set the peg revision for the copy operation
        CopyParameter&pegRevision(const Revision&);
        //! get the peg revision for the copy operation
        const Revision& getPegRevision()const;

        //! set the properties map for the copy operation
        CopyParameter&properties(const PropertiesMap&);
        //! get the properties map for the copy operation
        const PropertiesMap& getProperties()const;

    };

    //! parameter for svn_copy wrapper
    /*!
     * This class should never contains virtual methods. New Methods are always appended at end of class definition
     * \sa svn::Client::copy
     */
    class SVNQT_EXPORT DiffParameter
    {
    private:
        SharedPointer<DiffParameterData> _data;
    public:
        DiffParameter();
        
        const svn::StringArray& getChangeList()const;
        Depth getDepth()const;
        const svn::StringArray& getExtra()const;
        bool getIgnoreAncestry()const;
        bool getIgnoreContentType()const;
        bool getNoDiffDeleted()const;
        const Path& getPath1()const;
        const Path& getPath2()const;
        const svn::Revision& getPeg()const;
        const Path&getRelativeTo()const;
        const svn::Revision& getRev1()const;
        const svn::Revision& getRev2()const;
        const Path& getTmpPath()const;

        DiffParameter& path1(const Path&path);
        DiffParameter& path2(const Path&path);
        DiffParameter& tmpPath(const Path&path);
        DiffParameter& relativeTo(const Path&path);
        DiffParameter& depth(Depth _depth);
        svn::DiffParameter& changeList(const svn::StringArray&changeList);
        svn::DiffParameter& extra(const svn::StringArray&_extra);
        svn::DiffParameter& ignoreAncestry(bool value);
        svn::DiffParameter& ignoreContentType(bool value);
        svn::DiffParameter& peg_revision(const svn::Revision&_rev);
        svn::DiffParameter& rev1(const svn::Revision&_rev);
        svn::DiffParameter& rev2(const svn::Revision&_rev);
        svn::DiffParameter& noDiffDeleted(bool value);
    };
}


#endif
