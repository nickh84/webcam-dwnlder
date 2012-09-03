#include <QFile>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDateTime>
#include <QDir>
#include <QSettings>
#include <stdio.h>
#include "getcam.h"

getCam::getCam()
{
    title = QString();
    url = QUrl();
    dir = QDir();
    interval = 60;
    timer = new QTimer(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    connect(timer, SIGNAL(timeout()), this, SLOT(execute()));
}

getCam::~getCam()
{
    saveSettingsFile();
}

// Cam Operation
void getCam::start()
{
    timer->start(interval*1000);
    execute();
}

void getCam::stop()
{
    timer->stop();
}

QString getCam::getSettingsFile()
{
    if (settingsFile.isEmpty())
        saveSettingsFile();
    return settingsFile;
}

// Settings
void getCam::loadSettingsFile(QString fileName)
{
    QSettings settings(fileName, QSettings::IniFormat);
    settings.beginGroup("Cam");
    setTitle(settings.value("title").toString());
    setUrl(settings.value("url").toString());
    setDir(settings.value("dir").toString());
    setInterval(settings.value("interval").toInt());
    settings.endGroup();
    settingsFile = fileName;
    printf("%s Settings Loaded\n", qPrintable(getTitle()));
}

void getCam::saveSettingsFile()
{
    if (settingsFile.isEmpty()) {
        settingsFile = getDir();
        settingsFile += '/';
        settingsFile += getTitle();
        settingsFile += ".cam";
    }

    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.beginGroup("Cam");
    settings.setValue("title", getTitle());
    settings.setValue("url", getUrl());
    settings.setValue("dir", getDir());
    settings.setValue("interval", getInterval());
    settings.endGroup();
    printf("%s Settings Saved\n", qPrintable(getTitle()));
}

void getCam::setUrl(QString u)
{
    url.clear();
    url.setUrl(u);
}

void getCam::setInterval(int i)
{
    interval = i;
    if (timer->isActive())
        timer->start(interval*1000);
}

void getCam::setTitle(QString t)
{
    title.clear();
    title.operator =(t);
}

void getCam::setDir(QString d)
{
    dir.setPath(d);
}

// Return
QString getCam::getCurrentCam()
{
    return currentFile;
}

QString getCam::getUrl()
{
    return url.toString();
}

int getCam::getInterval()
{
    return interval;
}

QString getCam::getTitle()
{
    return title;
}

QString getCam::getDir()
{
    return dir.absolutePath();
}

// Internal
void getCam::execute()
{
    QNetworkRequest request(url);
    reply = manager.get(request);
}

void getCam::downloadFinished(QNetworkReply *reply)
{
    // Create filename based on date and time. (eg yyyyMMddhhmmss.jpg)
    QDateTime now = QDateTime::currentDateTime();
    QString filename = getDir();
    filename += '/';
    filename += now.toString("yyyyMMddhhmmss");
    filename += '.';
    filename += QFileInfo(url.path()).completeSuffix();

    if (reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n", url.toEncoded().constData(), qPrintable(reply->errorString()));
    } else {
        if (saveToDisk(filename, reply))
            printf("Download of %s succeeded (saved to %s)\n", url.toEncoded().constData(), qPrintable(filename));
    }

    reply->deleteLater();
}

bool getCam::saveToDisk(const QString &filename, QIODevice *data)
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
    emit updated();

    return true;
}
