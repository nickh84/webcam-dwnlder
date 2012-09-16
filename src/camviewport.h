/**************************************************************************
* camviewport.h - Copyright (C) 2012 Nick Heacock.
*
*    This file is part of Webcam Downloader.
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#ifndef CAMVIEWPORT_H
#define CAMVIEWPORT_H

#include <QGraphicsView>

class CamViewPort : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CamViewPort(QWidget *parent = 0);
    void load(QString file);
    
signals:
    
public slots:
    
protected:
    void resizeEvent(QResizeEvent *event);

private:
    QGraphicsScene *scene;
};

#endif // CAMVIEWPORT_H
