/***************************************************************************
 *   Project TUPI: Magia 2D                                                *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *   Project Leader: Gustav Gonzalez <info@maefloresta.com>                *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez / xtingray                                          *
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
 *   the Free Software Foundation; either version 3 of the License, or     *
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

#include "settings.h"
#include "kradiobuttongroup.h"
#include "kimagebutton.h"
#include "kdebug.h"
#include "ktitemtweener.h"
#include "stepsviewer.h"
#include "kttweenerstep.h"
#include "kosd.h"

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QComboBox>
#include <QBoxLayout>
#include <QHeaderView>
#include <QGraphicsPathItem>

struct Settings::Private
{
    QWidget *innerPanel;
    QBoxLayout *layout; 

    QLineEdit *input;
    KRadioButtonGroup *options;
    StepsViewer *stepViewer;
    QComboBox *comboInit;
    QLabel *totalLabel;
    bool selectionDone;
    Mode mode; 

    KImageButton *apply;
    KImageButton *remove;
};

Settings::Settings(QWidget *parent) : QWidget(parent), k(new Private)
{
    k->selectionDone = false;

    k->layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    k->layout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    setFont(QFont("Arial", 8, QFont::Normal, false));

    QLabel *nameLabel = new QLabel(tr("Name") + ": ");
    k->input = new QLineEdit;

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    nameLayout->setMargin(0);
    nameLayout->setSpacing(0);
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(k->input);

    k->options = new KRadioButtonGroup(tr("Options"), Qt::Vertical);
    k->options->addItem(tr("Select object"), 0);
    k->options->addItem(tr("Set Properties"), 1);
    connect(k->options, SIGNAL(clicked(int)), this, SLOT(emitOptionChanged(int)));

    k->apply = new KImageButton(QPixmap(THEME_DIR + "icons/save.png"), 22);

    connect(k->apply, SIGNAL(clicked()), this, SLOT(applyTween()));

    k->remove = new KImageButton(QPixmap(THEME_DIR + "icons/close.png"), 22);
    k->remove->setToolTip(tr("Cancel Tween"));

    connect(k->remove, SIGNAL(clicked()), this, SIGNAL(clickedResetTween()));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    buttonsLayout->setMargin(0);
    buttonsLayout->setSpacing(10);
    buttonsLayout->addWidget(k->apply);
    buttonsLayout->addWidget(k->remove);

    k->layout->addLayout(nameLayout);
    k->layout->addWidget(k->options);

    setInnerForm();

    k->layout->addSpacing(10);
    k->layout->addLayout(buttonsLayout);
    k->layout->setSpacing(5);

    activateSelectionMode();
}

Settings::~Settings()
{
    delete k;
}

void Settings::setInnerForm()
{
    k->innerPanel = new QWidget; 

    QBoxLayout *innerLayout = new QBoxLayout(QBoxLayout::TopToBottom, k->innerPanel);
    innerLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    QLabel *startingLabel = new QLabel(tr("Starting at frame") + ": ");
    startingLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    k->comboInit = new QComboBox();
    k->comboInit->setFixedWidth(60);
    //k->comboInit->setMaximumWidth(50);
    k->comboInit->setEditable(false);

    connect(k->comboInit, SIGNAL(currentIndexChanged(int)), this, SIGNAL(startingPointChanged(int)));

    QHBoxLayout *startLayout = new QHBoxLayout;
    startLayout->setAlignment(Qt::AlignHCenter);
    startLayout->setMargin(0);
    startLayout->setSpacing(0);
    // startLayout->addWidget(startingLabel);
    startLayout->addWidget(k->comboInit);

    k->stepViewer = new StepsViewer;
    k->stepViewer->verticalHeader()->hide();

    k->totalLabel = new QLabel(tr("Frames Total") + ": 0");
    k->totalLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    QHBoxLayout *totalLayout = new QHBoxLayout;
    totalLayout->setAlignment(Qt::AlignHCenter);
    totalLayout->setMargin(0);
    totalLayout->setSpacing(0);
    totalLayout->addWidget(k->totalLabel);

    innerLayout->addWidget(startingLabel);
    innerLayout->addLayout(startLayout);
    innerLayout->addWidget(k->stepViewer);

    innerLayout->addLayout(totalLayout);

    // k->innerPanel->setLayout(innerLayout);

    k->layout->addWidget(k->innerPanel);

    activeInnerForm(false);
}

void Settings::activeInnerForm(bool enable)
{
    if (enable && !k->innerPanel->isVisible())
        k->innerPanel->show();
    else
        k->innerPanel->hide();
}

void Settings::setParameters(const QString &name, int framesTotal, int startFrame)
{
    k->mode = Add;
    k->input->setText(name);

    activateSelectionMode();
    k->stepViewer->cleanRows();

    k->comboInit->setEnabled(false);
    k->apply->setToolTip(tr("Save Tween"));
    k->remove->setIcon(QPixmap(THEME_DIR + "icons/close.png"));
    k->remove->setToolTip(tr("Cancel Tween"));

    initStartCombo(framesTotal, startFrame);
}

void Settings::setParameters(KTItemTweener *currentTween)
{
    setEditMode();

    k->input->setText(currentTween->name());

    notifySelection(true);
    activatePathMode();

    k->comboInit->setEnabled(true);

    initStartCombo(currentTween->frames(), currentTween->startFrame());

    k->stepViewer->setPath(currentTween->graphicsPath());
    k->totalLabel->setText(tr("Frames Total") + ": " + QString::number(k->stepViewer->totalSteps()));
}

void Settings::initStartCombo(int framesTotal, int currentIndex)
{
    k->comboInit->clear();
    for (int i=1; i<=framesTotal; i++)
         k->comboInit->addItem(QString::number(i));

    k->comboInit->setCurrentIndex(currentIndex);
}

void Settings::setStartFrame(int currentIndex)
{
    k->comboInit->setCurrentIndex(currentIndex);
}

int Settings::startFrame()
{
    return k->comboInit->currentIndex();
}

int Settings::startComboSize()
{
    return k->comboInit->count();
}

void Settings::updateSteps(const QGraphicsPathItem *path)
{
    k->stepViewer->setPath(path);
    k->totalLabel->setText(tr("Frames Total") + ": " + QString::number(k->stepViewer->totalSteps()));
}

void Settings::emitOptionChanged(int option)
{
    switch (option) {
            case 0:
             {
                 kFatal() << "Settings::emitOptionChanged() - Position / Just tracing!";
                 activeInnerForm(false);
                 emit clickedSelect();
             }
            break;
            case 1:
             {
                 if (k->selectionDone) {
                     activeInnerForm(true);
                     emit clickedCreatePath();
                 } else {
                     k->options->setCurrentIndex(0);
                     KOsd::self()->display(tr("Info"), tr("Select objects for Tweening first!"), KOsd::Info);   
                 }
             }
    }
}

QString Settings::tweenToXml(int currentFrame, QString &path)
{
    QDomDocument doc;

    QDomElement root = doc.createElement("tweening");
    root.setAttribute("name", currentTweenName());
    root.setAttribute("type", KTItemTweener::Position);
    root.setAttribute("init", currentFrame);
    root.setAttribute("frames", k->stepViewer->totalSteps());
    root.setAttribute("coords", path);

    foreach (KTTweenerStep *step, k->stepViewer->steps())
             root.appendChild(step->toXml(doc));

    doc.appendChild(root);

    return doc.toString();
}

int Settings::totalSteps()
{
    return k->stepViewer->totalSteps();
}

void Settings::activatePathMode()
{
    k->options->setCurrentIndex(1);
}

void Settings::activateSelectionMode()
{
    kFatal() << "Settings::activateSelectionMode() - Just tracing!";
    k->options->setCurrentIndex(0);
}

void Settings::cleanData()
{
    k->stepViewer->cleanRows();
}

void Settings::notifySelection(bool flag)
{
    k->selectionDone = flag;
}

void Settings::applyTween()
{
    if (!k->selectionDone) {
        k->options->setCurrentIndex(0);
        KOsd::self()->display(tr("Info"), tr("Select objects for Tweening first!"), KOsd::Info);
        return;
    }

    if (totalSteps() <= 2) {
        KOsd::self()->display(tr("Error"), tr("No path created for Tweening"), KOsd::Error);
        return;
    }

    // SQA: Verify Tween is really well applied before call setEditMode!
    setEditMode();

    if (!k->comboInit->isEnabled())
        k->comboInit->setEnabled(true);

    emit clickedApplyTween();
}

void Settings::setEditMode()
{
    k->mode = Edit;
    k->apply->setToolTip(tr("Update Tween"));
    k->remove->setIcon(QPixmap(THEME_DIR + "icons/close_properties.png"));
    k->remove->setToolTip(tr("Close Tween properties"));
}

QString Settings::currentTweenName() const
{
    QString tweenName = k->input->text();
    if (tweenName.length() > 0)
        k->input->setFocus();

    return tweenName;
}