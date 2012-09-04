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
#include "config.h"

CamTab::CamTab(QWidget *parent) :
    QWidget(parent)
{
    createMainLayout();

    title = QString();
    url = QUrl();
    dir = QDir();
    interval = 60;
    timer = new QTimer(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    connect(timer, SIGNAL(timeout()), this, SLOT(execute()));
}

void CamTab::createMainLayout()
{
    viewPort = new QLabel();
    viewPort->setScaledContents(true);
    viewPort->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->addWidget(viewPort);
    setLayout(hLayout);
}

void CamTab::closeEvent(QCloseEvent *event)
{
    saveSettingsFile();
    event->accept();
}

void CamTab::updateViewPort()
{
    QPixmap view(getCurrentCam());
    viewPort->setMinimumSize(view.width()/2, view.height()/2);
    viewPort->setPixmap(view);

    QString stxt(getCurrentCam());
    stxt += " - ";
    stxt += QDateTime::currentDateTime().toString();
    statusText(stxt);
}

void CamTab::statusText(QString txt)
{
    emit updateStatusText(txt);
}

// Cam Operation
void CamTab::start()
{
    timer->start(interval*1000);
    execute();
}

void CamTab::stop()
{
    timer->stop();
}

QString CamTab::getSettingsFile()
{
    if (settingsFile.isEmpty())
        saveSettingsFile();
    return settingsFile;
}

// Settings
void CamTab::loadSettingsFile(QString fileName)
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

void CamTab::saveSettingsFile()
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

void CamTab::setUrl(QString u)
{
    url.clear();
    url.setUrl(u);
}

void CamTab::setInterval(int i)
{
    interval = i;
    if (timer->isActive())
        timer->start(interval*1000);
}

void CamTab::setTitle(QString t)
{
    title.clear();
    title.operator =(t);
}

void CamTab::setDir(QString d)
{
    dir.setPath(d);
}

// Return
QString CamTab::getCurrentCam()
{
    return currentFile;
}

QString CamTab::getUrl()
{
    return url.toString();
}

int CamTab::getInterval()
{
    return interval;
}

QString CamTab::getTitle()
{
    return title;
}

QString CamTab::getDir()
{
    return dir.absolutePath();
}

// Internal
void CamTab::execute()
{
    QNetworkRequest request(url);
    reply = manager.get(request);
}

void CamTab::downloadFinished(QNetworkReply *reply)
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
    updateViewPort();

    return true;
}
