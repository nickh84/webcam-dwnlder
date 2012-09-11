#ifndef VIDEOCREATOR_H
#define VIDEOCREATOR_H

#include <QDialog>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFileInfoList>
#include <QThread>

#include "camviewport.h"

class CVThread : public QThread
{
    Q_OBJECT

public:
    CVThread(QObject *parent = 0);

    void createVideo(QFileInfoList list);

signals:
    void updateTextBox(const QString str);

private:
    void run();
    QFileInfoList frameList;
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

private:
    CVThread cvt;
    void createLayout();
    void getFrameList();
    CamViewPort *viewPort;
    QTextEdit *textBox;
    QListWidget *listView;
    QPushButton *createButton;
    QDialogButtonBox *buttonBox;
    QString dir;
    QString suffix;
    QFileInfoList frameList;
};

#endif // VIDEOCREATOR_H
