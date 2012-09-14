#ifndef CAMTAB_H
#define CAMTAB_H

#include <QWidget>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QUrl>
#include <QDateTime>
#include <QDir>
#include "camviewport.h"
#include "camsettings.h"

class CamTab : public QWidget
{
    Q_OBJECT

    QNetworkAccessManager manager;

public:
    explicit CamTab(CamSettings *set, QWidget *parent = 0);
    CamSettings *settings;

    // Cam Operation
    void start();
    void stop();

    // Returns
    QString getCurrentCam();


signals:
    void updateStatusText(QString txt);
    void updated();
    
public slots:

private slots:
    void updateViewPort();
    void execute();
    void downloadFinished(QNetworkReply *reply);

private:
    void closeEvent(QCloseEvent *event);
    void createMainLayout();
    void statusText(QString txt);

    CamViewPort *viewPort;
    QFileInfoList frameList;
    QString currentFile;
    QNetworkReply *reply;
    QTimer *timer;
    bool saveToDisk(const QString &filename, QIODevice *data);
};

#endif // CAMTAB_H
