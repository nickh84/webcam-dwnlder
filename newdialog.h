#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include "camsettings.h"

// DIR COMBO BOX, LEAVE ALONE!
class dirComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit dirComboBox(QWidget *parent = 0);
    QString text() const;
    void setText(const QString &);

private:
    QLineEdit *dirEdit;

private slots:
    void dirDialog();

};
// DIR COMBO BOX END.

class newDialog : public QDialog
{
    Q_OBJECT
public:
    explicit newDialog(QWidget *parent = 0);
    CamSettings *settings;

signals:
    
public slots:

private slots:
    void createSettings();

private:
    QLineEdit *title;
    QLineEdit *url;
    dirComboBox *dir;
    QSpinBox *interval;
    void createLayout();
    
};

#endif // NEWDIALOG_H
