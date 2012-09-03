#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QUrl>
#include <QDateTime>
#include <QDir>

QT_USE_NAMESPACE

class getCam: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;

public:
    getCam();
    ~getCam();

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

public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);

signals:
    void updated();

private:
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
