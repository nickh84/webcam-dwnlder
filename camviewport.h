#ifndef CAMVIEWPORT_H
#define CAMVIEWPORT_H

#include <QGraphicsView>

class CamViewPort : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CamViewPort(QWidget *parent = 0);
    void load(QString file);
    
signals:
    
public slots:
    
protected:
    void resizeEvent(QResizeEvent *event);

private:
    QGraphicsScene *scene;
};

#endif // CAMVIEWPORT_H
