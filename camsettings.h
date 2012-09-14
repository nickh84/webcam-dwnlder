#ifndef CAMSETTINGS_H
#define CAMSETTINGS_H

#include <QSettings>
#include <QTime>
#include <QStringList>
#include <QDir>
#include <QUrl>

class CamSettings : public QSettings
{
    Q_OBJECT
public:
    explicit CamSettings(const QString &fileName, QObject *parent = 0);

    // Required settings...
    void setConfig(QString conf);
    void setTitle(QString title);
    void setDir(QString dir);
    void setUrl(QString url);
    void setInterval(int interval);
    void setExt(QString ext);

    // Optional settings...
    void setAdvTime(bool set);
    void setStartTime(QTime time);
    void setEndTime(QTime time);
    void setWeekDay(QStringList weekday);

    // Returns
    QString getConfig();
    QString getTitle();
    QString getDir();
    QUrl getUrl();
    int getInterval();
    QString getExt();

    bool isAdvTime();
    QTime getStartTime();
    QTime getEndTime();
    QStringList getWeekday();

signals:
    
public slots:
    
};

#endif // CAMSETTINGS_H
