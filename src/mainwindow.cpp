/**************************************************************************
* mainwindow.cpp - Copyright (C) 2012 Nick Heacock.
*
*    This file is part of Webcam Downloader.
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#include <QtGui>
#include <stdio.h>

#include "mainwindow.h"
#include "camtab.h"
#include "newdialog.h"
#include "videocreator.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    killCheck = false;

    setWindowIcon(QIcon(":/images/camicon.png"));
    setWindowTitle("Webcam Downloader - " VERSION);
    setMinimumSize(640,480);
    createActions();
    createMenus();
    createStatusBar();
    createTrayIcon();

    prevTabIdx = 0;
    tabWidget = new QTabWidget;
    tabWidget->setTabsClosable(true);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    setCentralWidget(tabWidget);
    readSettings();
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

void MainWindow::createActions()
{
    // File Menu
    newAct = new QAction(tr("&New"), this);
    connect(newAct, SIGNAL(triggered()), this, SLOT(newCam()));
    openAct = new QAction(tr("&Open"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openCam()));
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(_close()));

    // Cam Menu
    startAct = new QAction(tr("&Start"), this);
    connect(startAct, SIGNAL(triggered()), this, SLOT(startCam()));
    stopAct = new QAction(tr("S&top"), this);
    connect(stopAct, SIGNAL(triggered()), this, SLOT(stopCam()));
    optionsAct = new QAction(tr("&Options"), this);
    connect(optionsAct, SIGNAL(triggered()), this, SLOT(optionsCam()));
    cvideoAct = new QAction(tr("&Create Video"), this);
    connect(cvideoAct, SIGNAL(triggered()), this, SLOT(cvideoCam()));

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
    // File Menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    // Cam Menu
    camMenu = menuBar()->addMenu(tr("&Cam"));
    camMenu->setEnabled(false);
    camMenu->addAction(startAct);
    camMenu->addAction(stopAct);
    camMenu->addAction(optionsAct);
    camMenu->addSeparator();
    camMenu->addAction(cvideoAct);

    // Help Menu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createTab(CamSettings *settings)
{
    if (!settings->isValid()) {
        printf("createTab: Error in settings...\n");
        return;
    }
    CamTab *cam = new CamTab(settings);
    tabWidget->addTab(cam, cam->settings->getTitle());
    cam->start();
}

void MainWindow::openCam()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Cam"), QDir::homePath(), tr("Cam (*.cam)"));
    if (file.isEmpty())
        return;
    CamSettings *settings = new CamSettings(file);
    createTab(settings);
}

void MainWindow::newCam()
{
    newDialog nc(this);

    if(!nc.exec())
        return;

    createTab(nc.settings);
    nc.close();
}

void MainWindow::startCam()
{
    CamTab *cam = (CamTab *)tabWidget->currentWidget();
    cam->start();
}

void MainWindow::stopCam()
{
    CamTab *cam = (CamTab *)tabWidget->currentWidget();
    cam->stop();
}

void MainWindow::optionsCam()
{
    CamTab *cam = (CamTab *)tabWidget->currentWidget();
    newDialog nc(cam->settings, this);

    if(!nc.exec())
        return;
    nc.close();
}

void MainWindow::cvideoCam()
{
    CamTab *cam = (CamTab *)tabWidget->currentWidget();
    VideoCreator *cvideo = new VideoCreator(cam->settings->getDir(), cam->settings->getExt());
    cvideo->show();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       QString("Webcam Downloader " VERSION
                          "\n\n"
                          "Built with Qt " QT_VERSION_STR " (using Qt %1)"
                          "\n\n"
                          "Copyright (C) 2012 Nick Heacock.\n"
                          "This program is free software: you can redistribute it and/or modify\n"
                          "it under the terms of the GNU General Public License as published by\n"
                          "the Free Software Foundation, either version 3 of the License, or\n"
                          "(at your option) any later version.\n"
                          "\n"
                          "This program is distributed in the hope that it will be useful,\n"
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                          "GNU General Public License for more details.\n"
                          "\n"
                          "You should have received a copy of the GNU General Public License\n"
                          "along with this program.  If not, see <http://www.gnu.org/licenses/>.").arg(qVersion()));
}

void MainWindow::readSettings()
{
    QSettings settings(SETTINGSFILE, QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();

    QFileInfo conf;
    int size = settings.beginReadArray("Cams");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        conf.setFile(settings.value("config").toString());
        if (conf.exists())
            createTab(new CamSettings(settings.value("config").toString()));
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
        settings.setValue("title", tab->settings->getTitle());
        settings.setValue("config", tab->settings->getConfig());
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

void MainWindow::tabChanged(int idx)
{
    // Status bar messages
    if (tabWidget->widget(prevTabIdx))
        disconnect(tabWidget->widget(prevTabIdx), SIGNAL(updateStatusText(QString)), statusBar(), SLOT(showMessage(QString)));
    if (tabWidget->widget(idx))
        connect(tabWidget->widget(idx), SIGNAL(updateStatusText(QString)), statusBar(), SLOT(showMessage(QString)));

    // Enable/Disable cam menu
    if (idx < 0)
        camMenu->setEnabled(false);
    else
        camMenu->setEnabled(true);

    prevTabIdx = idx;
}

void MainWindow::closeTab(int idx)
{
    tabWidget->widget(idx)->close();
    tabWidget->removeTab(idx);
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
