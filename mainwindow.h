#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QSystemTrayIcon>

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
    void connectTab(int idx);
    void closeTab(int idx);
    void about();
    void openCam();
    void newCam();
    void _close();

private:
    void createTrayIcon();
    void createTab(QString fileName);
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

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    // Tray
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *trayMinAct;
    QAction *trayResAct;

};

#endif // MAINWINDOW_H
