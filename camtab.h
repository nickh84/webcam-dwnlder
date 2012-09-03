#ifndef CAMTAB_H
#define CAMTAB_H

#include <QWidget>
#include <QLabel>

#include "getcam.h"

class CamTab : public QWidget
{
    Q_OBJECT
public:
    explicit CamTab(QWidget *parent = 0);
    getCam cam;

signals:
    void updateStatusText(QString txt);
    
public slots:

private slots:
    void updateViewPort();

private:
    void createMainLayout();
    void statusText(QString txt);

    QLabel *viewPort;
};

#endif // CAMTAB_H
