#ifndef CAMTAB_H
#define CAMTAB_H

#include <QWidget>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QUrl>
#include <QDateTime>
#include <QDir>

class CamTab : public QWidget
{
    Q_OBJECT

    QNetworkAccessManager manager;

public:
    explicit CamTab(QWidget *parent = 0);

    // Cam Operation
    void start();
    void stop();

    // Settings
    void loadSettingsFile(QString fileName);
    void saveSettingsFile();
    void setUrl(QString u);
    void setInterval(int i);
    void setTitle(QString t);
    void setDir(QString d);


    // Returns
    QString getSettingsFile();
    QString getCurrentCam();
    QString getUrl();
    int getInterval();
    QString getTitle();
    QString getDir();

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

    QLabel *viewPort;
    QString title;
    QDir dir;
    QUrl url;
    int interval;
    QString currentFile;
    QString settingsFile;
    QNetworkReply *reply;
    QTimer *timer;
    bool saveToDisk(const QString &filename, QIODevice *data);
};

#endif // CAMTAB_H
