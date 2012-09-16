/**************************************************************************
* videocreator.h - Copyright (C) 2012 Nick Heacock.
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

#ifndef VIDEOCREATOR_H
#define VIDEOCREATOR_H

#include <QDialog>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QDialogButtonBox>
#include <QFileInfoList>
#include <QThread>

#include "camviewport.h"

class CVThread : public QThread
{
    Q_OBJECT

public:
    CVThread(QObject *parent = 0);

    void createVideo(QStringList list);

signals:
    void updateTextBox(const QString str);
    void startProgress(int min, int max);
    void updateProgress(int pos);
    void stopProgress();

private:
    void run();
    QStringList frameList;
};

class VideoCreator : public QDialog
{
    Q_OBJECT
public:
    explicit VideoCreator(QString _dir = 0, QString _suffix = 0, QWidget *parent = 0);
    
signals:
    
public slots:

private slots:
    void listSelect();
    void createVideo();
    void threadStart();
    void threadFinished();
    void status(QString str);
    void startProgress(int min, int max);
    void stopProgress();

private:
    CVThread cvt;
    void createLayout();
    void getFrameList();
    CamViewPort *viewPort;
    QTextEdit *textBox;
    QListWidget *listView;
    QPushButton *createButton;
    QDialogButtonBox *buttonBox;
    QProgressBar *progressBar;
    QString dir;
    QString suffix;
    QStringList frameList;
    QStringList currList;
};

#endif // VIDEOCREATOR_H
