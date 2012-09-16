/**************************************************************************
* camtab.h - Copyright (C) 2012 Nick Heacock.
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

#ifndef CAMTAB_H
#define CAMTAB_H

#include <QWidget>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QUrl>
#include <QDateTime>
#include <QDir>
#include "camviewport.h"
#include "camsettings.h"

class CamTab : public QWidget
{
    Q_OBJECT

    QNetworkAccessManager manager;

public:
    explicit CamTab(CamSettings *set, QWidget *parent = 0);
    CamSettings *settings;

    // Cam Operation
    void start();
    void stop();

    // Returns
    QString getCurrentCam();


signals:
    void updateStatusText(QString txt);
    void updated();
    
public slots:

private slots:
    void updateViewPort();
    void updateInterval();
    void execute();
    void downloadFinished(QNetworkReply *reply);

private:
    void closeEvent(QCloseEvent *event);
    void createMainLayout();
    void statusText(QString txt);

    CamViewPort *viewPort;
    QFileInfoList frameList;
    QString currentFile;
    QNetworkReply *reply;
    QTimer *timer;
    bool saveToDisk(const QString &filename, QIODevice *data);
};

#endif // CAMTAB_H
