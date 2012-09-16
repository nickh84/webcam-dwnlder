/**************************************************************************
* videocreator.cpp - Copyright (C) 2012 Nick Heacock.
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

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QDateTime>

#include "QVideoEncoder.h"

#include "videocreator.h"

VideoCreator::VideoCreator(QString _dir, QString _suffix, QWidget *parent) :
    QDialog(parent)
{
    dir = _dir;
    suffix = _suffix;
    suffix.prepend("*.");
    createLayout();
    getFrameList();
    currList = frameList;

    connect(&cvt, SIGNAL(started()), this, SLOT(threadStart()));
    connect(&cvt, SIGNAL(finished()), this, SLOT(threadFinished()));
    connect(&cvt, SIGNAL(updateTextBox(QString)), this, SLOT(status(QString)));
}

void VideoCreator::createLayout()
{
    // Viewport and Textbox layout
    viewPort = new CamViewPort;
    viewPort->setMinimumSize(640,480);
    textBox = new QTextEdit;
    textBox->setFixedHeight(65);
    textBox->setReadOnly(true);

    // Control Layout
    QVBoxLayout *CTL_Layout = new QVBoxLayout;
    progressBar = new QProgressBar;
    progressBar->hide();
    connect(&cvt, SIGNAL(startProgress(int,int)), this, SLOT(startProgress(int,int)));
    connect(&cvt, SIGNAL(updateProgress(int)), progressBar, SLOT(setValue(int)));
    connect(&cvt, SIGNAL(stopProgress()), this, SLOT(stopProgress()));
    createButton = new QPushButton(tr("Create Video"), this);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(createButton, SIGNAL(clicked()), this, SLOT(createVideo()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    progressBar->setFixedWidth(150);
    createButton->setFixedWidth(150);
    buttonBox->setFixedWidth(150);
    CTL_Layout->setSizeConstraint(QLayout::SetFixedSize);
    CTL_Layout->addWidget(progressBar);
    CTL_Layout->addWidget(createButton);
    CTL_Layout->addWidget(buttonBox);

    // Listview and Control layout
    listView = new QListWidget;
    listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(listView, SIGNAL(itemSelectionChanged()), this, SLOT(listSelect()));
    listView->setFixedWidth(150);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(viewPort, 0, 0);
    mainLayout->addWidget(textBox, 1, 0);
    mainLayout->addWidget(listView, 0, 1);
    mainLayout->addLayout(CTL_Layout, 1, 1);
    setLayout(mainLayout);
    setWindowTitle("Create video");
}

void VideoCreator::startProgress(int min, int max)
{
    progressBar->setRange(min, max);
    progressBar->show();
}

void VideoCreator::stopProgress()
{
    progressBar->hide();
    progressBar->reset();
}

void VideoCreator::createVideo()
{
    cvt.createVideo(currList);
}

void VideoCreator::getFrameList()
{
    // Frame List
    QDir dirlist(dir);
    QStringList tmplist = dirlist.entryList(QStringList(suffix), QDir::Files, QDir::Name);
    frameList.clear();
    for (int i = 0; i < tmplist.count(); i++) {
        frameList.append(QString("%1/%2").arg(dir).arg(tmplist.value(i)));
    }
    // Populate Listbox
    QDateTime time;
    QListWidgetItem *item;
    for (int i = 0; i < frameList.count(); i++) {
        time = QDateTime::fromString(QFileInfo(frameList.value(i)).baseName(), "yyyyMMddhhmmss");
        item = new QListWidgetItem(time.toString("MM-dd-yyyy hh:mm:ss"), listView);
        item->setData(Qt::UserRole, QVariant(frameList.value(i)));
        if (i == 0)
            listView->setCurrentItem(item);
    }
}

void VideoCreator::listSelect()
{
    if (!listView->currentItem())
        return;

    viewPort->load(listView->currentItem()->data(Qt::UserRole).toString());

    currList.clear();
    if (listView->selectedItems().count() > 1) {
        for (int i = 0; i < listView->selectedItems().count(); i++)
            currList.append(listView->selectedItems().value(i)->data(Qt::UserRole).toString());
    } else
        currList = frameList;
    currList.sort();
}

void VideoCreator::status(QString str)
{
    textBox->append(str);
}

void VideoCreator::threadStart()
{
    createButton->setEnabled(false);
}

void VideoCreator::threadFinished()
{
    createButton->setEnabled(true);
}

CVThread::CVThread(QObject *parent)
    : QThread(parent)
{

}

void CVThread::createVideo(QStringList list)
{
    frameList = list;
    if (!isRunning())
        start();
    else
        emit updateTextBox("Thread already running...");
}

void CVThread::run()
{
    QVideoEncoder encoder;
    QImage frame(frameList.value(0));
    QString filename = QString("%1/video%2.avi").arg(QFileInfo(frameList.value(0)).absolutePath()).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    int height = frame.height();
    int width = frame.width();
    int fps = 25;
    int bitrate = 0;
    int gop = 20;
    int size = 0;

    // Figure out bitrate
    int totalsize = 0;
    for (int i = 0; i<frameList.count(); i++) {
        totalsize += QFileInfo(frameList.value(i)).size();
    }
    bitrate = ((totalsize/(frameList.count()+1))*fps)*2.5;

    emit updateTextBox(QString("%1:").arg(filename));
    emit updateTextBox(QString("Width: %1 Height: %2 FPS: %3 Bitrate: %4 GOP: %5 Frames: %6")
                    .arg(width).arg(height).arg(fps).arg(bitrate).arg(gop).arg(frameList.count()+1));

    // Start video creation
    if (!encoder.createFile(filename, width, height, bitrate, gop, fps)) {
        emit updateTextBox("Error creating video...");
        return;
    }

    emit startProgress(0, frameList.count());
    for (int i = 0; i<frameList.count(); i++) {
        frame.load(frameList.value(i));
        size = encoder.encodeImage(frame);
        emit updateProgress(i);
        emit updateTextBox(QString("Encoded: %1 Size: %2")
                        .arg(QFileInfo(frameList.value(i)).fileName()).arg(size));
    }
    if (encoder.close()) {
        emit stopProgress();
        emit updateTextBox("Finished encoding video...");
        emit updateTextBox(filename);
        return;
    }
    emit stopProgress();
    emit updateTextBox("Error closing video...");
}
