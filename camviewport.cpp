#include "camviewport.h"

CamViewPort::CamViewPort(QWidget *parent) :
    QGraphicsView(parent)
{
   setInteractive(false);
   scene = new QGraphicsScene;
   setScene(scene);
}

void CamViewPort::load(QString file)
{
    scene->clear();
    scene->addPixmap(QPixmap(file));
}

void CamViewPort::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}
