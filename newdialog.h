#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

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

class newDialog : public QDialog
{
    Q_OBJECT
public:
    explicit newDialog(QWidget *parent = 0);
    QLineEdit *title;
    QLineEdit *url;
    dirComboBox *dir;
    QSpinBox *interval;
    
signals:
    
public slots:
    
};

#endif // NEWDIALOG_H
