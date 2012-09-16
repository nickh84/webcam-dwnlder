/**************************************************************************
* mainwindow.h - Copyright (C) 2012 Nick Heacock.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QSystemTrayIcon>
#include "camsettings.h"

class QMenu;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

protected:

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void tabChanged(int idx);
    void closeTab(int idx);
    void about();
    void openCam();
    void newCam();
    void _close();
    void startCam();
    void stopCam();
    void optionsCam();
    void cvideoCam();

private:
    void createTrayIcon();
    void createTab(CamSettings *settings);
    void closeEvent(QCloseEvent *event);
    void readSettings();
    void writeSettings();
    void createActions();
    void createMenus();
    void createStatusBar();
    void test(int i);

    bool killCheck;
    int prevTabIdx;

    QTabWidget *tabWidget;

    // File Menu
    QMenu *fileMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *exitAct;
    // Cam Menu
    QMenu *camMenu;
    QAction *startAct;
    QAction *stopAct;
    QAction *optionsAct;
    QAction *cvideoAct;
    // Help Menu
    QMenu *helpMenu;
    QAction *aboutAct;
    QAction *aboutQtAct;

    // Tray
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *trayMinAct;
    QAction *trayResAct;

};

#endif // MAINWINDOW_H
