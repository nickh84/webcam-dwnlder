/**************************************************************************
* camtab.cpp - Copyright (C) 2012 Nick Heacock.
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

#include <QLayout>
#include <QGroupBox>
#include <QFile>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDateTime>
#include <QDir>
#include <QSettings>
#include <QCloseEvent>
#include <stdio.h>

#include "camtab.h"
#include "camviewport.h"
#include "config.h"

CamTab::CamTab(CamSettings * set, QWidget *parent) :
    QWidget(parent)
{
    settings = set;
    createMainLayout();

    QString suffix(settings->getExt());
    suffix.prepend("*.");
    frameList = QDir(settings->getDir()).entryInfoList(QStringList(suffix), QDir::Files, QDir::Name);
    if (!frameList.isEmpty()) {
        currentFile = frameList.last().absoluteFilePath();
        updateViewPort();
    }

    timer = new QTimer(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    connect(timer, SIGNAL(timeout()), this, SLOT(execute()));
    connect(settings, SIGNAL(intChanged()), this, SLOT(updateInterval()));
}

void CamTab::createMainLayout()
{
    viewPort = new CamViewPort;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->addWidget(viewPort);
    setLayout(hLayout);
}

void CamTab::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void CamTab::updateViewPort()
{
    viewPort->load(getCurrentCam());

    QString stxt(getCurrentCam());
    stxt += " - ";
    stxt += QDateTime::currentDateTime().toString();
    statusText(stxt);
}

void CamTab::updateInterval()
{
    if (!timer->isActive())
        return;
    statusText(QString("Interval Changed to %1 seconds. Resetting timer...").arg(settings->getInterval()/1000));
    timer->start(settings->getInterval());
    execute();
}

void CamTab::statusText(QString txt)
{
    emit updateStatusText(txt);
}

// Cam Operation
void CamTab::start()
{
    if (timer->isActive()) {
        statusText("Already started...");
        return;
    }
    statusText("Recording started");
    timer->start(settings->getInterval());
    execute();
}

void CamTab::stop()
{
    if (!timer->isActive()) {
        statusText("Not recording...");
        return;
    }
    statusText("Recording stoped");
    timer->stop();
}

// Return
QString CamTab::getCurrentCam()
{
    return currentFile;
}

// Internal
void CamTab::execute()
{
    // If advanced timing is used, check day and time before downloading.
    if (settings->isAdvTime()) {
        // Check day of week
        if (!settings->getWeekday().contains(QDateTime::currentDateTime().toString("ddd")))
            return;
        // Check if time is between start and end time.
        if (!((QTime::currentTime().operator >=(settings->getStartTime())) &&
              (QTime::currentTime().operator <=(settings->getEndTime()))))
            return;
    }

    QNetworkRequest request(settings->getUrl());
    reply = manager.get(request);
}

void CamTab::downloadFinished(QNetworkReply *reply)
{
    // Create filename based on date and time. (eg yyyyMMddhhmmss.jpg)
    QDateTime now = QDateTime::currentDateTime();
    QString filename = QString("%1/%2.%3")
            .arg(settings->getDir()).arg(now.toString("yyyyMMddhhmmss")).arg(settings->getExt());

    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n", settings->getUrl().toEncoded().constData(), qPrintable(reply->errorString()));
    } else {
        if (saveToDisk(filename, reply))
            printf("Download of %s succeeded (saved to %s)\n", settings->getUrl().toEncoded().constData(), qPrintable(filename));
    }

    reply->deleteLater();
}

bool CamTab::saveToDisk(const QString &filename, QIODevice *data)
{
    // Compare previous file size with current.
    QFile prevfile(currentFile);
    if ((data->size() == 0) || (prevfile.size() == data->size()))
        return false;

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();
    currentFile = filename;
    frameList.append(QFileInfo(currentFile));
    updateViewPort();

    return true;
}
