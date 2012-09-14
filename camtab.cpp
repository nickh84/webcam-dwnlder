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

CamTab::CamTab(QWidget *parent) :
    QWidget(parent)
{
    createMainLayout();

    timer = new QTimer(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    connect(timer, SIGNAL(timeout()), this, SLOT(execute()));
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

// Settings
void CamTab::loadSettings(CamSettings *camset)
{
    settings = camset;
    printf("%s Settings Loaded\n", qPrintable(settings->getTitle()));
}

// Return
QString CamTab::getCurrentCam()
{
    return currentFile;
}

// Internal
void CamTab::execute()
{
    QNetworkRequest request(settings->getUrl());
    reply = manager.get(request);
}

void CamTab::downloadFinished(QNetworkReply *reply)
{
    // Create filename based on date and time. (eg yyyyMMddhhmmss.jpg)
    QDateTime now = QDateTime::currentDateTime();
    QString filename = QString("%1/%2.%3")
            .arg(settings->getDir()).arg(now.toString("yyyyMMddhhmmss")).arg(QFileInfo(settings->getUrl().path()).completeSuffix());

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
    updateViewPort();

    return true;
}
