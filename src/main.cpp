/*************************************************************************
 *  Copyright (C) 2020 by Caio Jordão Carvalho <caiojcarvalho@gmail.com> *
 *                                                                       *
 *  This program is free software; you can redistribute it and/or        *
 *  modify it under the terms of the GNU General Public License as       *
 *  published by the Free Software Foundation; either version 3 of       *
 *  the License, or (at your option) any later version.                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 *************************************************************************/

#include "ui/mark.h"
#include <QApplication>
#include <QScreen>

#include <KAboutData>
#include <KXmlGuiWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KAboutData aboutData(
            // The program name used internally. (componentName)
            QStringLiteral("mark"),
            // A displayable program name string. (displayName)
            QStringLiteral("marK"),
            // The program version string. (version)
            QStringLiteral("0.1.0"),
            // Short description of what the app does. (shortDescription)
            QStringLiteral("Scientific tool for annotation of machine learning datasets."),
            // The license this code is released under
            KAboutLicense::GPL_V3,
            // Copyright Statement (copyrightStatement = QString())
            QStringLiteral("(c) 2020"),
            // Optional text shown in the About box.
            // Can contain any information desired. (otherText)
            QStringLiteral("The project is in its early stage of development, currently supports image and text annotation."),
            // The program homepage string. (homePageAddress = QString())
            QStringLiteral("https://invent.kde.org/education/mark"),
            // The bug report email address
            // (bugsEmailAddress = QLatin1String("submit@bugs.kde.org")
            QStringLiteral("submit@bugs.kde.org"));
    aboutData.addAuthor(QStringLiteral("Caio Jordão Carvalho"), QStringLiteral("Mantainer"), QStringLiteral("caiojcarvalho@gmail.com"),
                        QStringLiteral("caiojcarvalho.wordpress.com"), QStringLiteral("cjlcarvalho"));
    aboutData.addAuthor(QStringLiteral("Jean Lima Andrade"), QStringLiteral("Contribuitor"), QStringLiteral("jyeno@protonmail.com"),
                        QStringLiteral("jyeno.home.blog"), QStringLiteral("jyeno"));
    KAboutData::setApplicationData(aboutData);

    marK w;
    w.setFixedSize(qApp->primaryScreen()->availableSize());
    w.setWindowState(Qt::WindowMaximized);
    w.setWindowIcon(QIcon::fromTheme("document-edit"));
    w.show();

    return app.exec();
}
