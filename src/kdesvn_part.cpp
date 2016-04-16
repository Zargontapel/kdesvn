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

#include "kdesvn_part.h"
#include "settings/kdesvnsettings.h"
#include "settings/displaysettings_impl.h"
#include "settings/dispcolorsettings_impl.h"
#include "settings/revisiontreesettingsdlg_impl.h"
#include "settings/diffmergesettings_impl.h"
#include "settings/subversionsettings_impl.h"
#include "settings/cmdexecsettings_impl.h"
#include "settings/polling_settings_impl.h"
#include "kdesvnview.h"
#include "commandline_part.h"
#include "svnqt/version_check.h"
#include "svnqt/url.h"
#include "helpers/kdesvn_debug.h"
#include "helpers/sshagent.h"
#include "svnfrontend/database/dboverview.h"

#include <ktoggleaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <kxmlguifactory.h>
#include <kaboutapplicationdialog.h>
#include <kconfigdialog.h>
#include <kaboutdata.h>
#include <klocalizedstring.h>
#include <khelpclient.h>
#include <kpluginfactory.h>

K_PLUGIN_FACTORY(KdesvnFactory, registerPlugin<kdesvnpart>(); registerPlugin<commandline_part>("commandline_part");)

static const char version[] = KDESVN_VERSION;

kdesvnpart::kdesvnpart(QWidget *parentWidget, QObject *parent, const QVariantList &args)
    : KParts::ReadOnlyPart(parent)
{
    Q_UNUSED(args);
    init(parentWidget, false);
}

kdesvnpart::kdesvnpart(QWidget *parentWidget, QObject *parent, bool ownapp, const QVariantList &args)
    : KParts::ReadOnlyPart(parent)
{
    Q_UNUSED(args);
    init(parentWidget, ownapp);
}

void kdesvnpart::init(QWidget *parentWidget, bool full)
{
    m_aboutDlg = 0;
    KLocalizedString::setApplicationDomain("kdesvn");
    // we need an instance
    // TODO: KF5 port
    //setComponentData(KdesvnFactory::componentData());

    m_browserExt = new KdesvnBrowserExtension(this);

    // this should be your custom internal widget
    m_view = new kdesvnView(actionCollection(), parentWidget, full);

    // notify the part that this is our internal widget
    setWidget(m_view);

    // create our actions
    setupActions();
    // set our XML-UI resource file
#ifdef TESTING_PARTRC
    setXMLFile(TESTING_PARTRC);
    qCDebug(KDESVN_LOG) << "Using test rc file in " << TESTING_PARTRC << endl;
#else
    setXMLFile("kdesvn_part.rc");
#endif
    connect(m_view, SIGNAL(sigShowPopup(QString,QWidget**)), this, SLOT(slotDispPopup(QString,QWidget**)));
    connect(m_view, SIGNAL(sigSwitchUrl(QUrl)), this, SLOT(openUrl(QUrl)));
    connect(this, SIGNAL(refreshTree()), m_view, SLOT(refreshCurrentTree()));
    connect(m_view, SIGNAL(setWindowCaption(QString)), this, SIGNAL(setWindowCaption(QString)));
    connect(m_view, SIGNAL(sigUrlChanged(QString)), this, SLOT(slotUrlChanged(QString)));
    connect(this, SIGNAL(settingsChanged()), widget(), SLOT(slotSettingsChanged()));
    SshAgent ssh;
    ssh.querySshAgent();
}

kdesvnpart::~kdesvnpart()
{
    ///@todo replace with KDE4 like stuff
    //kdesvnpartFactory::instance()->config()->sync();
}

void kdesvnpart::slotUrlChanged(const QString &url)
{
    setUrl(QUrl(url));
}

bool kdesvnpart::openFile()
{
    m_view->openUrl(url());
    // just for fun, set the status bar
    emit setStatusBarText(url().toString());

    return true;
}

bool kdesvnpart::openUrl(const QUrl &aUrl)
{
    QUrl _url(aUrl);

    _url.setScheme(svn::Url::transformProtokoll(_url.scheme()));

    if (!_url.isValid() || !closeUrl()) {
        return false;
    }
    setUrl(_url);
    emit started(0);
    bool ret = m_view->openUrl(url());
    if (ret) {
        emit completed();
        emit setWindowCaption(url().toString());
    }
    return ret;
}

void kdesvnpart::slotFileProperties()
{
}

void kdesvnpart::slotDispPopup(const QString &name, QWidget **target)
{
    *target = hostContainer(name);
}

/*!
    \fn kdesvnpart::setupActions()
 */
void kdesvnpart::setupActions()
{
    KToggleAction *toggletemp;

    toggletemp = new KToggleAction(i18n("Logs follow node changes"), this);
    actionCollection()->addAction("toggle_log_follows", toggletemp);
    toggletemp->setChecked(Kdesvnsettings::log_follows_nodes());
    connect(toggletemp, SIGNAL(toggled(bool)), this, SLOT(slotLogFollowNodes(bool)));

    toggletemp = new KToggleAction(i18n("Display ignored files"), this);
    actionCollection()->addAction("toggle_ignored_files", toggletemp);
    toggletemp->setChecked(Kdesvnsettings::display_ignored_files());
    connect(toggletemp, SIGNAL(toggled(bool)), this, SLOT(slotDisplayIgnored(bool)));

    toggletemp = new KToggleAction(i18n("Display unknown files"), this);
    actionCollection()->addAction("toggle_unknown_files", toggletemp);
    toggletemp->setChecked(Kdesvnsettings::display_unknown_files());
    connect(toggletemp, SIGNAL(toggled(bool)), this, SLOT(slotDisplayUnkown(bool)));

    toggletemp = new KToggleAction(i18n("Hide unchanged files"), this);
    actionCollection()->addAction("toggle_hide_unchanged_files", toggletemp);
    toggletemp->setChecked(Kdesvnsettings::hide_unchanged_files());
    connect(toggletemp, SIGNAL(toggled(bool)), this, SLOT(slotHideUnchanged(bool)));

    toggletemp = new KToggleAction(i18n("Work online"), this);
    actionCollection()->addAction("toggle_network", toggletemp);
    toggletemp->setChecked(Kdesvnsettings::network_on());
    connect(toggletemp, SIGNAL(toggled(bool)), this, SLOT(slotEnableNetwork(bool)));

    QAction *t = KStandardAction::preferences(this, SLOT(slotShowSettings()), actionCollection());

    t->setText(i18n("Configure Kdesvn..."));
    actionCollection()->addAction("kdesvnpart_pref", t);

    if (QCoreApplication::applicationName() != QLatin1String("kdesvn")) {
        t = new QAction(QIcon::fromTheme("kdesvn"), i18n("About kdesvn part"), this);
        connect(t, SIGNAL(triggered(bool)), SLOT(showAboutApplication()));
        actionCollection()->addAction("help_about_kdesvnpart", t);

        t = new QAction(QIcon::fromTheme("help-contents"), i18n("Kdesvn Handbook"), this);
        connect(t, SIGNAL(triggered(bool)), SLOT(appHelpActivated()));
        actionCollection()->addAction("help_kdesvn", t);
    }
}

void kdesvnpart::slotSshAdd()
{
    SshAgent ag;
    ag.addSshIdentities(true);
}

/*!
    \fn kdesvnpart::slotLogFollowNodes(bool)
 */
void kdesvnpart::slotLogFollowNodes(bool how)
{
    Kdesvnsettings::setLog_follows_nodes(how);
    Kdesvnsettings::self()->save();
}

/*!
    \fn kdesvnpart::slotDiplayIgnored(bool)
 */
void kdesvnpart::slotDisplayIgnored(bool how)
{
    Kdesvnsettings::setDisplay_ignored_files(how);
    Kdesvnsettings::self()->save();
    emit settingsChanged();
}

/*!
    \fn kdesvnpart::slotDisplayUnknown(bool)
 */
void kdesvnpart::slotDisplayUnkown(bool how)
{
    Kdesvnsettings::setDisplay_unknown_files(how);
    Kdesvnsettings::self()->save();
    emit settingsChanged();
}

/*!
    \fn kdesvnpart::slotHideUnchanged(bool)
 */
void kdesvnpart::slotHideUnchanged(bool how)
{
    Kdesvnsettings::setHide_unchanged_files(how);
    Kdesvnsettings::self()->save();
    emit settingsChanged();
}

void kdesvnpart::slotEnableNetwork(bool how)
{
    Kdesvnsettings::setNetwork_on(how);
    Kdesvnsettings::self()->save();
    emit settingsChanged();
}

/*!
    \fn kdesvnpart::closeURL()
 */
bool kdesvnpart::closeUrl()
{
    KParts::ReadOnlyPart::closeUrl();
    setUrl(QUrl());
    m_view->closeMe();
    emit setWindowCaption(QString());
    return true;
}

KdesvnBrowserExtension::KdesvnBrowserExtension(kdesvnpart *p)
    : KParts::BrowserExtension(p)
{}

KdesvnBrowserExtension::~KdesvnBrowserExtension()
{

}

void KdesvnBrowserExtension::properties()
{
    static_cast<kdesvnpart *>(parent())->slotFileProperties();
}

/*!
    \fn kdesvnpart::showAboutApplication()
 */
void kdesvnpart::showAboutApplication()
{
    if (!m_aboutDlg) {
        QString m_Extratext = i18n("Built with Subversion library: %1\nRunning Subversion library: %2").arg(svn::Version::linked_version(), svn::Version::running_version());

        KAboutData about(QLatin1String("kdesvnpart"),
                         i18n("kdesvn Part"),
                         version,
                         i18n("A Subversion Client for KDE (dynamic Part component)"),
                         KAboutLicense::LGPL_V2,
                         i18n("(C) 2005-2009 Rajko Albrecht"),
                         m_Extratext);

        about.addAuthor(QLatin1String("Rajko Albrecht"), i18n("Original author and maintainer"), QLatin1String("ral@alwins-world.de"));
        about.addAuthor(QLatin1String("Christian Ehrlicher"), i18n("Developer"), QLatin1String("ch.ehrlicher@gmx.de"));
        about.setHomepage("https://projects.kde.org/kdesvn");
        qApp->setWindowIcon(QIcon::fromTheme(QLatin1String("kdesvn")));
        m_aboutDlg = new KAboutApplicationDialog(about);
    }
    if (m_aboutDlg == 0) {
        return;
    }
    if (!m_aboutDlg->isVisible()) {
        m_aboutDlg->show();
    } else {
        m_aboutDlg->raise();
    }
}

/*!
    \fn kdesvnpart::appHelpActivated()
 */
void kdesvnpart::appHelpActivated()
{
    KHelpClient::invokeHelp(QString(), "kdesvn");
}

/*!
    \fn kdesvnpart::slotShowSettings()
 */
void kdesvnpart::slotShowSettings()
{
    if (KConfigDialog::showDialog("kdesvnpart_settings")) {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(widget(),
            "kdesvnpart_settings",
            Kdesvnsettings::self());
    dialog->setFaceType(KPageDialog::List);

    // TODO: KF5
    //dialog->setHelp("setup", "kdesvn");
    dialog->addPage(new DisplaySettings_impl(0),
                    i18n("General"), "configure", i18n("General Settings"), true);
    dialog->addPage(new SubversionSettings_impl(0),
                    i18n("Subversion"), "kdesvn", i18n("Subversion Settings"), true);
    dialog->addPage(new PollingSettings_impl(0),
                    i18n("Timed jobs"), "kdesvnclock", i18n("Settings for timed jobs"), true);
    dialog->addPage(new DiffMergeSettings_impl(0),
                    i18n("Diff & Merge"), "kdesvnmerge", i18n("Settings for diff and merge"), true);
    dialog->addPage(new DispColorSettings_impl(0),
                    i18n("Colors"), "kdesvncolors", i18n("Color Settings"), true);
    dialog->addPage(new RevisiontreeSettingsDlg_impl(0),
                    i18n("Revision tree"), "kdesvntree", i18n("Revision tree Settings"), true);
    dialog->addPage(new CmdExecSettings_impl(0),
                    i18n("KIO / Command line"), "kdesvnterminal", i18n("Settings for command line and KIO execution"), true);

    connect(dialog, SIGNAL(settingsChanged(QString)), this, SLOT(slotSettingsChanged(QString)));
    dialog->show();
}

/*!
    \fn kdesvnpart::slotSettingsChanged()
 */
void kdesvnpart::slotSettingsChanged(const QString &)
{
    QAction *temp;
    temp = actionCollection()->action("toggle_log_follows");
    if (temp) {
        temp->setChecked(Kdesvnsettings::log_follows_nodes());
    }
    temp = actionCollection()->action("toggle_ignored_files");
    if (temp) {
        temp->setChecked(Kdesvnsettings::display_ignored_files());
    }
#if 0
    /// not needed this momenta
    temp = actionCollection()->action("toggle_unknown_files");
    if (temp) {
        ((KToggleAction *)temp)->setChecked(kdesvnpart_Prefs::self()->mdisp_unknown_files);
    }
#endif
    emit settingsChanged();
}

void kdesvnpart::showDbStatus()
{
    if (m_view) {
        m_view->stopCacheThreads();
        DbOverview::showDbOverview(svn::ClientP());
    }
}

#include "kdesvn_part.moc"
