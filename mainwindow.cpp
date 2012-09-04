#include <QtGui>
#include <stdio.h>

#include "mainwindow.h"
#include "camtab.h"
#include "newdialog.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    killCheck = false;

    setWindowIcon(QIcon(":/images/camicon.png"));
    createActions();
    createMenus();
    createStatusBar();
    createTrayIcon();

    prevTabIdx = 0;
    tabWidget = new QTabWidget;
    tabWidget->setTabsClosable(true);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(connectTab(int)));
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    setCentralWidget(tabWidget);
    readSettings();
}

void MainWindow::readSettings()
{
    QSettings settings(SETTINGSFILE, QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();

    int size = settings.beginReadArray("Cams");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        createTab(settings.value("config").toString());
    }
    settings.endArray();
}

void MainWindow::writeSettings()
{
    QSettings settings(SETTINGSFILE, QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("pos", pos());
    settings.endGroup();

    settings.beginWriteArray("Cams");
    for (int i = 0; i < tabWidget->count(); ++i) {
        settings.setArrayIndex(i);
        CamTab *tab = (CamTab *)tabWidget->widget(i);
        settings.setValue("title", tab->getTitle());
        settings.setValue("config", tab->getSettingsFile());
    }
    settings.endArray();
}

void MainWindow::_close()
{
    killCheck = true;
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (killCheck) {
        writeSettings();
        event->accept();
    } else {
        hide();
        event->ignore();
    }
}

void MainWindow::connectTab(int idx)
{
    // Status bar messages
    if (tabWidget->widget(prevTabIdx)) {
        printf("Disconnect\n");
        disconnect(tabWidget->widget(prevTabIdx), SIGNAL(updateStatusText(QString)), statusBar(), SLOT(showMessage(QString)));
    }
    if (tabWidget->widget(idx)) {
        printf("Connect\n");
        connect(tabWidget->widget(idx), SIGNAL(updateStatusText(QString)), statusBar(), SLOT(showMessage(QString)));
    }
    prevTabIdx = idx;
}

void MainWindow::closeTab(int idx)
{
    tabWidget->widget(idx)->close();
    tabWidget->removeTab(idx);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(trayResAct);
    trayIconMenu->addAction(trayMinAct);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(exitAct);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(windowIcon());
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason != QSystemTrayIcon::Trigger)
        return;

    if (isVisible())
        hide();
    else
        showNormal();
}

void MainWindow::createActions()
{
    // File Menu
    newAct = new QAction(tr("&New"), this);
    newAct->setStatusTip(tr("Create new webcam recording"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newCam()));
    openAct = new QAction(tr("&Open"), this);
    openAct->setStatusTip(tr("Open a webcam recording"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openCam()));
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(_close()));

    //Help Menu
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("About application"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("About the Qt library"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // Tray Menu
    trayResAct = new QAction(tr("&Restore"), this);
    connect(trayResAct, SIGNAL(triggered()), this, SLOT(showNormal()));
    trayMinAct = new QAction(tr("&Hide"), this);
    connect(trayMinAct, SIGNAL(triggered()), this, SLOT(hide()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createTab(QString fileName)
{
    CamTab *cam = new CamTab;
    cam->loadSettingsFile(fileName);
    tabWidget->addTab(cam, cam->getTitle());
    cam->start();
}

void MainWindow::openCam()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Cam"), QDir::homePath(), tr("Cam (*.cam)"));
    if (file.isEmpty())
        return;
    createTab(file);
}

void MainWindow::newCam()
{
    newDialog *nc = new newDialog(this);

    //PLACEHOLDERS
    nc->title->setText("Dragster");
    nc->url->setText("http://174.121.245.214/cp/DragsterCam_1024.jpg");
    nc->dir->setText("C:/Users/Nick/Documents/QTWorkspace/dragtmp");

    if(!nc->exec())
        return;

    // Create a settings file for new cam
    QString file = nc->dir->text();
    file += '/';
    file += nc->title->text();
    file += ".cam";
    printf("%s\n", qPrintable(file));

    QSettings camsettings(file, QSettings::IniFormat);
    camsettings.beginGroup("Cam");
    camsettings.setValue("title", nc->title->text());
    camsettings.setValue("url", nc->url->text());
    camsettings.setValue("dir", nc->dir->text());
    camsettings.setValue("interval", nc->interval->value());
    camsettings.endGroup();

    createTab(file);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("Download a Webcam Image over the internet at a timed interval"));
}
