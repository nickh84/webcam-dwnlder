/**************************************************************************
* camsettings.cpp - Copyright (C) 2012 Nick Heacock.
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

#include <QFileInfo>
#include "camsettings.h"

CamSettings::CamSettings(const QString & fileName, QObject * parent) :
    QSettings(fileName, QSettings::IniFormat, parent)
{
    setConfig(fileName);
}

void CamSettings::setConfig(QString conf)
{
    setValue("conf", conf);
}

void CamSettings::setTitle(QString title)
{
    setValue("title", title);
}

void CamSettings::setDir(QString dir)
{
    setValue("dir", dir);
}

void CamSettings::setUrl(QString url)
{
    setValue("url", url);
    setExt(QFileInfo(QUrl(url).path()).completeSuffix());
}

void CamSettings::setInterval(int interval)
{
    int oldval = getInterval();
    setValue("interval", interval);
    if (interval != oldval)
        emit intChanged();
}

void CamSettings::setExt(QString ext)
{
    setValue("extension", ext);
}

void CamSettings::setAdvTime(bool set)
{
    setValue("advtime", set);
}

void CamSettings::setStartTime(QTime time)
{
    setValue("starttime", time);
}

void CamSettings::setEndTime(QTime time)
{
    setValue("endtime", time);
}

void CamSettings::setWeekDay(QList<QVariant> weekday)
{
    setValue("weekday", weekday);
}

QString CamSettings::getConfig()
{
    return value("conf").toString();
}

QString CamSettings::getTitle()
{
    return value("title").toString();
}

QString CamSettings::getDir()
{
    return value("dir").toString();
}

QUrl CamSettings::getUrl()
{
    return value("url").toUrl();
}

int CamSettings::getInterval()
{
    return value("interval", 60000).toInt();
}

QString CamSettings::getExt()
{
    return value("extension", QFileInfo(getUrl().path()).completeSuffix()).toString();
}

bool CamSettings::isAdvTime()
{
    return value("advtime", false).toBool();
}

QTime CamSettings::getStartTime()
{
    return value("starttime").toTime();
}

QTime CamSettings::getEndTime()
{
    return value("endtime").toTime();
}

QList<QVariant> CamSettings::getWeekday()
{
    return value("weekday").toList();
}

// Return true if all settings are valid.
bool CamSettings::isValid()
{
    if (getTitle().isEmpty())
        return false;
    if (!QDir(getDir()).exists())
        return false;
    if (!getUrl().isValid())
        return false;
    if (!getInterval())
        return false;
    if (getExt().isEmpty())
        return false;
    if (isAdvTime()) {
        if (getWeekday().isEmpty())
            return false;
        if (!getStartTime().isValid())
            return false;
        if (!getEndTime().isValid())
            return false;
    }
    return true;
}

// Return true is current time is between starttime and endtime.
// Fixes previous Midnight Transistion bug. (eg. ST=11:00pm, ET=3:00am would not work.)
bool CamSettings::currentTimeValid()
{
    if (!isAdvTime())
        return true;

    int d = QDate(QDate::currentDate()).dayOfWeek();
    int nd = 1;
    int pd = 7;
    if (d != 7)
        nd = d+1;
    if (d != 1)
        pd = d-1;

    if (!getWeekday().contains(d))
        return false;

    // Non-Midnight Transition.
    if (getStartTime().operator <(getEndTime()))
        if ((QTime::currentTime().operator >=(getStartTime())) &&
                (QTime::currentTime().operator <=(getEndTime())))
            return true;

    // Midnight Transition. (only happens if starttime is greater than endtime)
    if (getStartTime().operator >(getEndTime())) {
        if (QTime::currentTime().operator >=(getStartTime()))
            if (getWeekday().contains(nd))
                return true;
        if (QTime::currentTime().operator <=(getEndTime()))
            if (getWeekday().contains(pd))
                return true;
    }
    return false;
}

