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

    connect(&cvt, SIGNAL(started()), this, SLOT(threadStart()));
    connect(&cvt, SIGNAL(finished()), this, SLOT(threadFinished()));
    connect(&cvt, SIGNAL(updateTextBox(QString)), this, SLOT(status(QString)));
}

void VideoCreator::createLayout()
{
    // Viewport and Textbox layout
    viewPort = new CamViewPort;
    textBox = new QTextEdit;
    textBox->setFixedHeight(65);
    textBox->setReadOnly(true);

    // Control Layout
    QVBoxLayout *CTL_Layout = new QVBoxLayout;
    createButton = new QPushButton(tr("Create Video"), this);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(createButton, SIGNAL(clicked()), this, SLOT(createVideo()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    createButton->setFixedWidth(150);
    buttonBox->setFixedWidth(150);
    CTL_Layout->setSizeConstraint(QLayout::SetFixedSize);
    CTL_Layout->addWidget(createButton);
    CTL_Layout->addWidget(buttonBox);

    // Listview and Control layout
    listView = new QListWidget;
    connect(listView, SIGNAL(itemSelectionChanged()), this, SLOT(listSelect()));
    listView->setFixedWidth(150);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(viewPort, 0, 0);
    mainLayout->addWidget(textBox, 1, 0);
    mainLayout->addWidget(listView, 0, 1);
    mainLayout->addLayout(CTL_Layout, 1, 1);
    setLayout(mainLayout);
}

void VideoCreator::createVideo()
{
    cvt.createVideo(frameList);
}

void VideoCreator::getFrameList()
{
    // Frame List
    QDir dirlist(dir);
    frameList = dirlist.entryInfoList(QStringList(suffix), QDir::Files, QDir::Name);

    // Populate Listbox
    QDateTime time;
    QListWidgetItem *item;
    for (int i = 0; i < frameList.count(); i++) {
        time = QDateTime::fromString(frameList.value(i).baseName(), "yyyyMMddhhmmss");
        item = new QListWidgetItem(time.toString("MM-dd-yyyy hh:mm:ss"), listView);
        item->setData(Qt::UserRole, QVariant(frameList.value(i).absoluteFilePath()));
    }
}

void VideoCreator::listSelect()
{
    if (!listView->currentItem())
        return;

    viewPort->load(listView->currentItem()->data(Qt::UserRole).toString());
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

void CVThread::createVideo(QFileInfoList list)
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
    QImage frame(frameList.value(0).absoluteFilePath());
    QString filename = QString("%1/video%2.avi").arg(frameList.value(0).absolutePath()).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    int height = frame.height();
    int width = frame.width();
    int fps = 25;
    int bitrate = 0;
    int gop = 20;
    int size = 0;

    // Figure out bitrate
    int totalsize = 0;
    for (int i = 0; i<frameList.count(); i++) {
        totalsize += frameList.value(i).size();
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

    for (int i = 0; i<frameList.count(); i++) {
        frame.load(frameList.value(i).absoluteFilePath());
        size = encoder.encodeImage(frame);
        emit updateTextBox(QString("Encoded: %1 Size: %2")
                        .arg(frameList.value(i).fileName()).arg(size));
    }
    if (encoder.close()) {
        emit updateTextBox("Finished encoding video...");
        emit updateTextBox(filename);
        return;
    }
    emit updateTextBox("Error closing video...");
}
