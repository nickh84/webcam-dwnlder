#include <QLayout>
#include <QGroupBox>

#include "camtab.h"
#include "config.h"

CamTab::CamTab(QWidget *parent) :
    QWidget(parent)
{
    createMainLayout();

    connect(&cam, SIGNAL(updated()), this, SLOT(updateViewPort()));

    //PLACEHOLDER
    //cam.setTitle("Dragster");
    //cam.setUrl(CAMURL);
    //cam.setDir("C:/Users/Nick/Documents/CPWeb/Dragster");
    //cam.setInterval(60);
    //cam.start();
}

void CamTab::createMainLayout()
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;
    QWidget *vBox = new QWidget;

    vBox->setLayout(vLayout);
    vBox->setFixedWidth(200);

    viewPort = new QLabel();
    viewPort->setScaledContents(true);

    QGroupBox *ibox1 = new QGroupBox("Cam Info");
    QGroupBox *ibox2 = new QGroupBox("Current View");
    QGroupBox *ibox3 = new QGroupBox("Total");

    hLayout->addWidget(viewPort);
    hLayout->addWidget(vBox);

    vLayout->addWidget(ibox1);
    vLayout->addWidget(ibox2);
    vLayout->addWidget(ibox3);

    setLayout(hLayout);
}

void CamTab::updateViewPort()
{
    viewPort->setPixmap(QPixmap(cam.getCurrentCam()));
    statusText(cam.getCurrentCam());
}

void CamTab::statusText(QString txt)
{
    emit updateStatusText(txt);
}
