/**************************************************************************
* newdialog.h - Copyright (C) 2012 Nick Heacock.
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

#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QTimeEdit>
#include <QList>
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
    newDialog(CamSettings *camset, QWidget *parent = 0);
    CamSettings *settings;

signals:
    
public slots:

private slots:
    void createSettings();
    void enableAdvTime(int state);

private:
    QLineEdit *title;
    QLineEdit *url;
    dirComboBox *dir;
    QSpinBox *interval;
    QCheckBox *advtime;
    QTimeEdit *starttime;
    QTimeEdit *endtime;
    QList<QCheckBox *> weekdayList;
    QStringList weekday;
    QWidget *advwidget;
    bool isNew;
    bool isValid();
    void createLayout();
};

#endif // NEWDIALOG_H
