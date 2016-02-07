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

#include "kdesvn.h"
#include "commandline.h"
#include "kdesvn-config.h"
#include <QApplication>
#include <KAboutData>

#include <klocale.h>
#include <kdebug.h>
#include <QCommandLineParser>

static const char description[] =
    I18N_NOOP("A Subversion Client for KDE (standalone application)");

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("kdesvn");
    app.setApplicationDisplayName("kdesvn");
    app.setOrganizationDomain("kde.org");
    app.setApplicationVersion(KDESVN_VERSION);

    KAboutData aboutData(QLatin1String("kdesvn"), i18n("kdesvn"), QString(KDESVN_VERSION), i18n(description),
                         KAboutLicense::GPL, i18n("(C) 2005-2009 Rajko Albrecht"));
    aboutData.addAuthor(i18n("Rajko Albrecht"), i18n("Developer"), QString("ral@alwins-world.de"));
    aboutData.addAuthor(i18n("Ovidiu-Florin BOGDAN"), i18n("KF5/Qt5 Porting"), QString("ovidiu.b13@gmail.com"));
    aboutData.addAuthor(i18n("Christian Ehrlicher"), i18n("Developer"), QLatin1String("ch.ehrlicher@gmx.de"));
    aboutData.setHomepage("https://projects.kde.org/kdesvn");

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    aboutData.processCommandLine(&parser);
    parser.addOption(QCommandLineOption(QStringList() << "r startrev[:endrev]", i18n("Execute single Subversion command on specific revision(-range)")));
    parser.addOption(QCommandLineOption(QStringList() << "R", i18n("Ask for revision when executing single command")));
    parser.addOption(QCommandLineOption(QStringList() << "f", i18n("Force operation")));
    parser.addOption(QCommandLineOption(QStringList() << "o <file>", i18n("Save output of Subversion command (eg \"cat\") into file <file>")));
    parser.addOption(QCommandLineOption(QStringList() << "l <number>", i18n("Limit log output to <number>")));
    parser.addPositionalArgument("+exec <command>", i18n("Execute Subversion command (\"exec help\" for more information)"));
    parser.addPositionalArgument("+[URL]", i18n("Document to open"));
    parser.process(app); // PORTING SCRIPT: move this to after any parser.addOption

    // see if we are starting with session management
    if (app.isSessionRestored()) {
        RESTORE(kdesvn);
    } else {
        // no session.. just start up normally
        if (parser.positionalArguments().isEmpty()) {
            kdesvn *widget = new kdesvn;
            widget->show();
            widget->checkReload();
        } else {
            if (parser.positionalArguments().at(0) == QLatin1String("exec")) {
                CommandLine cl(&parser);
                return cl.exec();
            } else {
                int i = 0;
                for (; i < parser.positionalArguments().count(); i++) {
                    kdesvn *widget = new kdesvn;
                    widget->show();
                    widget->load(QUrl::fromUserInput(parser.positionalArguments().at(i)), true);
                }
            }
        }
    }
    return app.exec();
}
