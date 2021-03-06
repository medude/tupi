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

#ifndef TUPPROJECTACTIONBAR_H
#define TUPPROJECTACTIONBAR_H

#include "tglobal.h"
#include "tapplicationproperties.h"
#include "tseparator.h"
#include "tconfig.h"
#include "toptionaldialog.h"
#include "timagebutton.h"

#include <QWidget>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QVariant>
#include <QSpacerItem>
#include <QDesktopWidget>

class TImageButton;
class QBoxLayout;

/**
 * @author David Cuadrado
*/

class TUPI_EXPORT TupProjectActionBar : public QWidget
{
    Q_OBJECT
    
    public:
        enum Action
        {
            NoAction = 0x00,
            
            InsertFrame = 1 << 1,
            RemoveFrame = 1 << 2,
            ExtendFrame = 1 << 3, 
            MoveFrameBackward = 1 << 4,
            MoveFrameForward = 1 << 5,
            LockFrame = 1 << 6,
            
            InsertLayer = 1 << 7,
            RemoveLayer = 1 << 8,
            MoveLayerUp = 1 << 9,
            MoveLayerDown = 1 << 10,
            LockLayer = 1 << 11,
            
            InsertScene = 1 << 12,
            RemoveScene = 1 << 13,
            MoveSceneUp = 1 << 14,
            MoveSceneDown = 1 << 15,
            LockScene = 1 << 16,
            Separator = 1 << 17,

            AllActions = InsertFrame | RemoveFrame | ExtendFrame | MoveFrameBackward | MoveFrameForward | LockFrame | InsertLayer | RemoveLayer 
                                     | MoveLayerUp | MoveLayerDown | InsertScene | RemoveScene | MoveSceneUp | MoveSceneDown,

            FrameActions = InsertFrame | ExtendFrame | RemoveFrame | MoveFrameBackward | MoveFrameForward | LockFrame,
            LayerActions = InsertLayer | RemoveLayer | MoveLayerUp | MoveLayerDown | LockLayer,
            SceneActions = InsertScene | RemoveScene | MoveSceneUp | MoveSceneDown
        };
        
        Q_DECLARE_FLAGS(Actions, Action);
        
        TupProjectActionBar(const QString &container = QString(), Actions actions = NoAction, Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = 0);
        ~TupProjectActionBar();
        void setFixedSize(int s);
        
        void insertSeparator(int position);
        void insertBlankSpace(int position);
        
        TImageButton *button(Action action);
        
    public slots:
        void emitActionSelected(int action);
        
    signals:
        void actionSelected(int action);
        
    private:
        void setup(Actions actions);
        
    private:
        struct Private;
        Private *const k;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(TupProjectActionBar::Actions);

#endif
