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
    setValue("interval", interval);
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

void CamSettings::setWeekDay(QStringList weekday)
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
    return value("interval").toInt();
}

QString CamSettings::getExt()
{
    return value("extension").toString();
}

bool CamSettings::isAdvTime()
{
    return value("advtime").toBool();
}

QTime CamSettings::getStartTime()
{
    return value("starttime").toTime();
}

QTime CamSettings::getEndTime()
{
    return value("endtime").toTime();
}

QStringList CamSettings::getWeekday()
{
    return value("weekday").toStringList();
}
