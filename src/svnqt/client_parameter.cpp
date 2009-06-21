#include "client_parameter.hpp"
#include "svnqt/svnqttypes.hpp"
#include "svnqt/stringarray.hpp"

#define SETIT(x,y) _data->x = y ; return *this;
namespace svn
{
    //! internal data structure
    struct SVNQT_NOEXPORT CopyParameterData
    {
        CopyParameterData()
            :_srcPath(),_srcRevision(),_pegRevision(),_destPath(),_asChild(false),_makeParent(false),_ignoreExternal(false),_force(false),_properties()
        {
        }
        Targets  _srcPath;
        Revision _srcRevision;
        Revision _pegRevision;
        Path _destPath;
        bool _asChild;
        bool _makeParent;
        bool _ignoreExternal;
        //! used for move operation instead of copy
        bool _force;
        PropertiesMap _properties;
    };

    CopyParameter::CopyParameter(const Targets&_srcPath,const Path&_destPath)
    {
        _data = new CopyParameterData();
        _data->_srcPath = _srcPath;
        _data->_destPath = _destPath;
    }

    const Targets& CopyParameter::srcPath()const
    {
        return _data->_srcPath;
    }

    const Path&CopyParameter::destination()const
    {
        return _data->_destPath;
    }

    CopyParameter::~CopyParameter()
    {
        _data = 0;
    }

    CopyParameter&CopyParameter::asChild(bool value)
    {
        SETIT(_asChild,value)
    }
    bool CopyParameter::asChild()const
    {
        return _data->_asChild;
    }

    CopyParameter&CopyParameter::makeParent(bool value)
    {
        SETIT(_makeParent,value)
    }
    bool CopyParameter::makeParent()const
    {
        return _data->_makeParent;
    }

    CopyParameter&CopyParameter::force(bool value)
    {
        SETIT(_force,value)
    }
    bool CopyParameter::force()const
    {
        return _data->_force;
    }

    CopyParameter&CopyParameter::ignoreExternal(bool value)
    {
        SETIT(_ignoreExternal,value)
    }
    bool CopyParameter::ignoreExternal()const
    {
        return _data->_ignoreExternal;
    }

    CopyParameter&CopyParameter::srcRevision(const Revision&rev)
    {
        SETIT(_srcRevision,rev)
    }
    const Revision&CopyParameter::srcRevision()const
    {
        return _data->_srcRevision;
    }

    CopyParameter&CopyParameter::pegRevision(const Revision&rev)
    {
        SETIT(_pegRevision,rev)
    }
    const Revision&CopyParameter::pegRevision()const
    {
        return _data->_pegRevision;
    }
    CopyParameter&CopyParameter::properties(const PropertiesMap&props)
    {
        SETIT(_properties,props)
    }
    const PropertiesMap&CopyParameter::properties()const
    {
        return _data->_properties;
    }

    struct SVNQT_NOEXPORT DiffParameterData
    {
    public:
        DiffParameterData()
            :_tmpPath(),_path1(),_path2(),_relativeTo(),_changeList(),_ignoreAncestry(false),_noDiffDeleted(false),
                      _depth(DepthInfinity),_peg_revision(Revision::UNDEFINED),
                      _rev1(Revision::START),_rev2(Revision::HEAD),_extra(),_ignore_contenttype(false)
        {
        }

        Path _tmpPath;
        Path _path1;
        Path _path2;
        Path _relativeTo;
        StringArray _changeList;
        bool _ignoreAncestry;
        bool _noDiffDeleted;
        Depth _depth;
        Revision _peg_revision;
        Revision _rev1;
        Revision _rev2;
        StringArray _extra;
        bool _ignore_contenttype;
    };


    /*!
        \fn DiffParameter::path1()const
    */
    const svn::Path& DiffParameter::path1()const
    {
        return _data->_path1;
    }


    /*!
        \fn DiffParameter::path2()const
    */
    const svn::Path& DiffParameter::path2()const
    {
        return _data->_path2;
    }


    /*!
        \fn DiffParameter::tpPath()const
    */
    const svn::Path& DiffParameter::tmpPath()const
    {
        return _data->_tmpPath;
    }

    /*!
        \fn DiffParameter::relativeTo()const
    */
    const svn::Path& DiffParameter::relativeTo()const
    {
        return _data->_relativeTo;
    }


    /*!
        \fn DiffParameter::depth()const
    */
    svn::Depth DiffParameter::depth()const
    {
        return _data->_depth;
    }


    /*!
        \fn DiffParameter::path1(const Path&path)
    */
    DiffParameter& DiffParameter::path1(const svn::Path&path)
    {
        SETIT(_path1,path)
    }


    /*!
        \fn DiffParameter::path2(const Path&path)
    */
    DiffParameter& DiffParameter::path2(const svn::Path&path)
    {
        SETIT(_path2,path)
    }


    /*!
        \fn DiffParameter::tmpPath(const Path&path)
    */
    DiffParameter& DiffParameter::tmpPath(const svn::Path&path)
    {
        SETIT(_tmpPath,path)
    }

    /*!
        \fn DiffParameter::relativeTo(const Path&path)
    */
    DiffParameter& DiffParameter::relativeTo(const svn::Path&path)
    {
        SETIT(_relativeTo,path)
    }


    /*!
        \fn DiffParameter::depth(Depth _depth)
    */
    DiffParameter& DiffParameter::depth(svn::Depth _depth)
    {
        SETIT(_depth,_depth)
    }


    /*!
        \fn DiffParameter::DiffParameter()
    */
    DiffParameter::DiffParameter()
    {
        _data = new DiffParameterData();
    }

    DiffParameter::~DiffParameter()
    {
        _data = 0;
    }

    /*!
        \fn DiffParameter::ignoreAncestry()const
    */
    bool DiffParameter::ignoreAncestry()const
    {
        return _data->_ignoreAncestry;
    }


    /*!
        \fn DiffParameter::IgnoreContentType()const
    */
    bool DiffParameter::ignoreContentType()const
    {
        return _data->_ignore_contenttype;
    }


    /*!
        \fn DiffParameter::NoDiffDeleted()const
    */
    bool DiffParameter::noDiffDeleted()const
    {
        return _data->_noDiffDeleted;
    }


    /*!
        \fn DiffParameter::Peg()const
    */
    const svn::Revision& DiffParameter::peg()const
    {
        return _data->_peg_revision;
    }


    /*!
        \fn DiffParameter::rev1()const
    */
    const svn::Revision& DiffParameter::rev1()const
    {
        return _data->_rev1;
    }


    /*!
        \fn DiffParameter::rev2()const
    */
    const svn::Revision& DiffParameter::rev2()const
    {
        return _data->_rev2;
    }


    /*!
        \fn DiffParameter::changeList()const
    */
    const svn::StringArray& DiffParameter::changeList()const
    {
        return _data->_changeList;
    }


    /*!
        \fn DiffParameter::extra()const
    */
    const svn::StringArray& DiffParameter::extra()const
    {
        return _data->_extra;
    }


    /*!
        \fn DiffParameter::changeList(const svn::StringArray&changeList)
    */
    DiffParameter& DiffParameter::changeList(const svn::StringArray&changeList)
    {
        SETIT(_changeList,changeList)
    }


    /*!
        \fn DiffParameter::extra(const svn::StringArray&_extra)
    */
    DiffParameter& DiffParameter::extra(const svn::StringArray&_extra)
    {
        SETIT(_extra,_extra)
    }


    /*!
        \fn DiffParameter::ignoreAncestry(bool value)
    */
    DiffParameter& DiffParameter::ignoreAncestry(bool value)
    {
        SETIT(_ignoreAncestry,value)
    }


    /*!
        \fn DiffParameter::ignoreContentType(bool value)
    */
    DiffParameter& DiffParameter::ignoreContentType(bool value)
    {
        SETIT(_ignore_contenttype,value)
    }


    /*!
        \fn DiffParameter::peg_revision(const svn::Revision&_rev)
    */
    DiffParameter& DiffParameter::peg(const svn::Revision&_rev)
    {
        SETIT(_peg_revision,_rev)
    }


    /*!
        \fn DiffParameter::rev1(const svn::Revision&_rev)
    */
    DiffParameter& DiffParameter::rev1(const svn::Revision&_rev)
    {
        SETIT(_rev1,_rev)
    }


    /*!
        \fn DiffParameter::rev2(const svn::Revision&_rev)
    */
    DiffParameter& DiffParameter::rev2(const svn::Revision&_rev)
    {
        SETIT(_rev2,_rev)
    }


    /*!
        \fn DiffParameter::noDiffDeleted(bool value)
    */
    DiffParameter& DiffParameter::noDiffDeleted(bool value)
    {
        SETIT(_noDiffDeleted,value)
    }

    struct StatusParameterData
    {
        StatusParameterData(const Path&path)
            :_path(path),_revision(Revision::UNDEFINED),_depth(DepthInfinity),_getAll(true),_update(true),_noIgnore(false),_ignoreExternals(false)
            ,_detailedRemote(false),_changeList()
        {
        }
        Path _path;
        Revision _revision;
        Depth _depth;
        bool _getAll;
        bool _update;
        bool _noIgnore;
        bool _ignoreExternals;
        bool _detailedRemote;
        StringArray _changeList;
    };

    StatusParameter::StatusParameter(const Path&path)
    {
        _data = new StatusParameterData(path);
    }
    StatusParameter::~StatusParameter()
    {
        _data = 0;
    }

    const Path& StatusParameter::path()const
    {
        return _data->_path;
    }
    StatusParameter& StatusParameter::path(const Path&_path)
    {
        SETIT(_path,_path)
    }
    const Revision& StatusParameter::revision()const
    {
        return _data->_revision;
    }
    StatusParameter& StatusParameter::revision(const Revision&rev)
    {
        SETIT(_revision,rev)
    }
    Depth StatusParameter::depth()const
    {
        return _data->_depth;
    }
    StatusParameter& StatusParameter::depth(Depth d)
    {
        SETIT(_depth,d)
    }
    bool StatusParameter::all()const
    {
        return _data->_getAll;
    }
    StatusParameter& StatusParameter::all(bool getall)
    {
        SETIT(_getAll,getall)
    }
    bool StatusParameter::update()const
    {
        return _data->_update;
    }
    StatusParameter&StatusParameter::update(bool updates)
    {
        SETIT(_update,updates)
    }
    bool StatusParameter::noIgnore()const
    {
        return _data->_noIgnore;
    }
    StatusParameter&StatusParameter::noIgnore(bool noignore)
    {
        SETIT(_noIgnore,noignore)
    }
    bool StatusParameter::ignoreExternals()const
    {
        return _data->_ignoreExternals;
    }
    StatusParameter&StatusParameter::ignoreExternals(bool noexternals)
    {
        SETIT(_ignoreExternals,noexternals)
    }
    const StringArray&StatusParameter::changeList()const
    {
        return _data->_changeList;
    }
    StatusParameter&StatusParameter::changeList(const StringArray&list)
    {
        SETIT(_changeList,list)
    }
    bool StatusParameter::detailedRemote()const
    {
        return _data->_detailedRemote;
    }
    StatusParameter&StatusParameter::detailedRemote(bool value)
    {
        SETIT(_detailedRemote,value)
    }


    struct LogParameterData
    {
    public:
        LogParameterData()
            :_targets(),_ranges(),_peg(Revision::UNDEFINED),_limit(0),
            _discoverChangedPathes(false),_strictNodeHistory(true),_includeMergedRevisions(false),
            _revisionProperties()
        {
        }
        Targets _targets;
        RevisionRanges _ranges;
        Revision _peg;
        int _limit;
        bool _discoverChangedPathes,_strictNodeHistory,_includeMergedRevisions;
        StringArray _revisionProperties;
    };

    LogParameter::LogParameter()
    {
        _data = new LogParameterData();
    }
    LogParameter::~LogParameter()
    {
        _data = 0;
    }

    const Targets&LogParameter::targets()const
    {
        return _data->_targets;
    }
    LogParameter&LogParameter::targets(const Targets&targets)
    {
        SETIT(_targets,targets)
    }

    const RevisionRanges&LogParameter::revisions()const
    {
        return _data->_ranges;
    }
    LogParameter&LogParameter::revisions(const RevisionRanges&revisions)
    {
        SETIT(_ranges,revisions)
    }

    const RevisionRange&LogParameter::revisionRange()const
    {
        if (_data->_ranges.size()<1) {
            const static RevisionRange r(Revision::UNDEFINED,Revision::UNDEFINED);
            return r;
        }
        return _data->_ranges[0];
    }
    LogParameter&LogParameter::revisionRange(const Revision&start,const Revision&end)
    {
        _data->_ranges.clear();
        _data->_ranges.append(RevisionRange(start,end));
        return *this;
    }

    const Revision&LogParameter::peg()const
    {
        return _data->_peg;
    }
    LogParameter&LogParameter::peg(const Revision&peg)
    {
        SETIT(_peg,peg)
    }

    int LogParameter::limit()const
    {
        return _data->_limit;
    }
    LogParameter&LogParameter::limit(int limit)
    {
        SETIT(_limit,limit)
    }

    bool LogParameter::discoverChangedPathes()const
    {
        return _data->_discoverChangedPathes;
    }
    LogParameter&LogParameter::discoverChangedPathes(bool value)
    {
        SETIT(_discoverChangedPathes,value)
    }

    bool LogParameter::strictNodeHistory()const
    {
        return _data->_strictNodeHistory;
    }
    LogParameter&LogParameter::strictNodeHistory(bool value)
    {
        SETIT(_strictNodeHistory,value)
    }

    bool LogParameter::includeMergedRevisions()const
    {
        return _data->_includeMergedRevisions;
    }
    LogParameter&LogParameter::includeMergedRevisions(bool value)
    {
        SETIT(_includeMergedRevisions,value)
    }

    const StringArray& LogParameter::revisionProperties()const
    {
        return _data->_revisionProperties;
    }
    LogParameter&LogParameter::revisionProperties(const StringArray&props)
    {
        SETIT(_revisionProperties,props)
    }

    struct PropertiesParameterData
    {
        PropertiesParameterData()
            :_name(QString()),_value(QString()),_originalValue(QString()),
            _path(),_revision(Revision::UNDEFINED),_force(false),_depth(DepthEmpty),_skipCheck(false),_changeList(),_revProperties()
        {}
        QString _name;
        QString _value;
        QString _originalValue;
        Path _path;
        Revision _revision;
        bool _force;
        Depth _depth;
        bool _skipCheck;
        StringArray _changeList;
        PropertiesMap _revProperties;
    };

    PropertiesParameter::PropertiesParameter()
    {
        _data = new PropertiesParameterData();
    }
    PropertiesParameter::~PropertiesParameter()
    {
        _data = 0;
    }

    PropertiesParameter& PropertiesParameter::propertyName(const QString&_name)
    {
        SETIT(_name,_name)
    }
    const QString&PropertiesParameter::propertyName()const
    {
        return _data->_name;
    }

    PropertiesParameter& PropertiesParameter::propertyValue(const QString&value)
    {
        SETIT(_value,value);
    }
    const QString&PropertiesParameter::propertyValue()const
    {
        return _data->_value;
    }

    PropertiesParameter& PropertiesParameter::propertyOriginalValue(const QString&value)
    {
        SETIT(_originalValue,value);
    }
    const QString&PropertiesParameter::propertyOriginalValue()const
    {
        return _data->_originalValue;
    }

    PropertiesParameter& PropertiesParameter::path(const Path&_path)
    {
        SETIT(_path,_path);
    }
    const Path& PropertiesParameter::path()const
    {
        return _data->_path;
    }

    PropertiesParameter& PropertiesParameter::revision(const Revision&_rev)
    {
        SETIT(_revision,_rev);
    }
    const Revision& PropertiesParameter::revision()const
    {
        return _data->_revision;
    }

    PropertiesParameter& PropertiesParameter::force(bool force)
    {
        SETIT(_force,force);
    }
    bool PropertiesParameter::force()const
    {
        return _data->_force;
    }

    PropertiesParameter&PropertiesParameter::depth(Depth depth)
    {
        SETIT(_depth,depth);
    }
    Depth PropertiesParameter::depth()const
    {
        return _data->_depth;
    }

    PropertiesParameter&PropertiesParameter::skipCheck(bool value)
    {
        SETIT(_skipCheck,value);
    }
    bool PropertiesParameter::skipCheck()const
    {
        return _data->_skipCheck;
    }

    PropertiesParameter&PropertiesParameter::changeList(const StringArray&_list)
    {
        SETIT(_changeList,_list);
    }
    const StringArray&PropertiesParameter::changeList()const
    {
        return _data->_changeList;
    }

    PropertiesParameter&PropertiesParameter::revisionProperties(const PropertiesMap&props)
    {
        SETIT(_revProperties,props);
    }
    const PropertiesMap&PropertiesParameter::revisionProperties()const
    {
        return _data->_revProperties;
    }

}
