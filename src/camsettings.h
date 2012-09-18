/**************************************************************************
* camsettings.h - Copyright (C) 2012 Nick Heacock.
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

#ifndef CAMSETTINGS_H
#define CAMSETTINGS_H

#include <QSettings>
#include <QTime>
#include <QStringList>
#include <QDir>
#include <QUrl>

class CamSettings : public QSettings
{
    Q_OBJECT
public:
    explicit CamSettings(const QString &fileName, QObject *parent = 0);

    // Required settings...
    void setConfig(QString conf);
    void setTitle(QString title);
    void setDir(QString dir);
    void setUrl(QString url);
    void setInterval(int interval);
    void setExt(QString ext);
    // Optional settings...
    void setAdvTime(bool set);
    void setStartTime(QTime time);
    void setEndTime(QTime time);
    void setWeekDay(QList<QVariant> weekday);

    // Setting Returns
    QString getConfig();
    QString getTitle();
    QString getDir();
    QUrl getUrl();
    int getInterval();
    QString getExt();
    bool isAdvTime();
    QTime getStartTime();
    QTime getEndTime();
    QList<QVariant> getWeekday();

    // Utility Returns
    bool isValid();
    bool currentTimeValid();


signals:
    void intChanged();
    
public slots:
    
};

#endif // CAMSETTINGS_H
