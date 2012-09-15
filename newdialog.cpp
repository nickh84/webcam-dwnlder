#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFileDialog>
#include <QDateTime>

#include "newdialog.h"
#include "camsettings.h"

newDialog::newDialog(QWidget *parent) :
    QDialog(parent)
{
    isNew = true;
    createLayout();
    setWindowTitle(tr("Create new webcam recording"));
    title->setText("Pagoda");
    url->setText("http://174.121.245.214/cp/PagodaCam_1024.jpg");
    dir->setText("C:/Users/Nick/Documents/QTWorkspace/camtmp");
}

newDialog::newDialog(CamSettings *camset, QWidget *parent) :
    QDialog(parent)
{
    isNew = false;
    settings = camset;
    createLayout();
    setWindowTitle(tr("Change webcam settings"));

    title->setEnabled(false);
    title->setText(settings->getTitle());
    dir->setEnabled(false);
    dir->setText(settings->getDir());
    url->setText(settings->getUrl().toString());
    interval->setValue(settings->getInterval()/1000);
    advtime->setChecked(settings->isAdvTime());

    for (int i = 0; i<7; i++) {
        if (settings->getWeekday().contains(QDate::shortDayName(i+1)))
            weekdayList.value(i)->setChecked(true);
        else
            weekdayList.value(i)->setChecked(false);
    }

    starttime->setTime(settings->getStartTime());
    endtime->setTime(settings->getEndTime());
}

void newDialog::createLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QFormLayout *formLayout = new QFormLayout;

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                      | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(createSettings()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    title = new QLineEdit;
    url = new QLineEdit;
    dir = new dirComboBox;
    interval = new QSpinBox;
    interval->setSuffix(" seconds");
    interval->setValue(60);
    advtime = new QCheckBox;
    connect(advtime, SIGNAL(stateChanged(int)), this, SLOT(enableAdvTime(int)));

    formLayout->addRow(tr("&Title"), title);
    formLayout->addRow(tr("&Dir"), dir);
    formLayout->addRow(tr("&Url"), url);
    formLayout->addRow(tr("&Interval"), interval);
    formLayout->addRow(tr("&Enable advanced timing"), advtime);
    mainLayout->addLayout(formLayout);

    // ADVANCED TIMING LAYOUT
    advwidget = new QWidget;
    QVBoxLayout *advmainlayout = new QVBoxLayout;

    // Weekday layout
    QGridLayout *wdlayout = new QGridLayout;
    for (int i = 0; i<7; i++) {
        weekdayList.append(new QCheckBox());
        wdlayout->addWidget(new QLabel(QDate::shortDayName(i+1)), 0, i);
        wdlayout->addWidget(weekdayList.value(i), 1, i);
    }
    advmainlayout->addLayout(wdlayout);

    // Start and End time layouts
    QFormLayout *timeslayout = new QFormLayout;
    starttime = new QTimeEdit;
    timeslayout->addRow(tr("Start Time:"), starttime);
    endtime = new QTimeEdit;
    timeslayout->addRow(tr("End Time:"), endtime);
    advmainlayout->addLayout(timeslayout);


    advwidget->setLayout(advmainlayout);
    advwidget->setEnabled(false);
    mainLayout->addWidget(advwidget);
    mainLayout->addWidget(buttonBox, Qt::AlignRight);
    setLayout(mainLayout);
    setFixedSize(500,sizeHint().height());
}

void newDialog::enableAdvTime(int state)
{
    if (state)
        advwidget->setEnabled(true);
    else
        advwidget->setEnabled(false);
}

void newDialog::createSettings()
{
    if (isNew)
        settings = new CamSettings(QString("%1/%2.cam").arg(dir->text()).arg(title->text()));
    settings->setTitle(title->text());
    settings->setDir(dir->text());
    settings->setUrl(url->text());
    settings->setInterval(interval->value()*1000);
    settings->setAdvTime(advtime->isChecked());
    settings->setStartTime(starttime->time());
    settings->setEndTime(endtime->time());
    weekday.clear();
    for (int i = 0; i < 7; i++) {
        if (weekdayList.value(i)->isChecked())
            weekday.append(QDate::shortDayName(i+1));
    }
    settings->setWeekDay(weekday);
    accept();
}


// Dir Combo Box class.
dirComboBox::dirComboBox(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *dcLayout = new QHBoxLayout;
    QPushButton *dirButton = new QPushButton("Browse");
    dirEdit = new QLineEdit;
    connect(dirButton, SIGNAL(clicked()), this, SLOT(dirDialog()));
    dcLayout->addWidget(dirEdit);
    dcLayout->addWidget(dirButton);
    dcLayout->setContentsMargins(0,0,0,0);
    dcLayout->setSpacing(0);
    setLayout(dcLayout);

}

void dirComboBox::dirDialog()
{
    dirEdit->setText(QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks));
}

void dirComboBox::setText(const QString &str)
{
    dirEdit->setText(str);
}

QString dirComboBox::text() const
{
    return dirEdit->text();
}
