/***************************************************************************
 *   Project TUPI: Magia 2D                                                *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *   Project Leader: Gustav Gonzalez <info@maefloresta.com>                *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez                                                     *
 *                                                                         *
 *   KTooN's versions:                                                     * 
 *                                                                         *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       *
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Gustav Gonzalez - http://www.maefloresta.com       *
 *   License:                                                              *
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
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "tuphelpdialog.h"
#include "tuphelpbrowser.h"
#include "tuphelpwidget.h"

#include <QHBoxLayout>

TupHelpDialog::TupHelpDialog(const QString &path, QWidget *parent) : QFrame(parent)
{
    QFile file(THEME_DIR + "config/help.qss");
    if (file.exists()) {
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        if (styleSheet.length() > 0) 
            setStyleSheet(styleSheet);
    } else {
        #ifdef K_DEBUG
            QString msg = "TupHelpDialog::TupHelpDialog() - theme file doesn't exist -> " + QString(THEME_DIR + "config/ui.qss");
            #ifdef Q_OS_WIN
                qDebug() << msg;
            #else
                tError() << msg;
            #endif
        #endif
    }

    setWindowTitle(tr("Help Content"));
    setWindowIcon(QIcon(QPixmap(THEME_DIR + "icons/help_mode.png")));

    #ifdef Q_OS_WIN
        QString helpPath = SHARE_DIR + "help/";
    #else
        QString helpPath = SHARE_DIR + "data/help/";
    #endif

    TupHelpBrowser *helpBrowser = new TupHelpBrowser(path, this);
    TupHelpWidget *helpMenu = new TupHelpWidget(helpPath);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(helpBrowser);
    splitter->addWidget(helpMenu);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(splitter);

    connect(helpMenu, SIGNAL(pageLoaded(const QString &)), helpBrowser, SLOT(setSource(const QString &)));
    connect(helpMenu, SIGNAL(closeDialog()), this, SLOT(close()));

    connect(helpBrowser, SIGNAL(closeDialog()), this, SLOT(close()));
}

TupHelpDialog::~TupHelpDialog()
{
}
