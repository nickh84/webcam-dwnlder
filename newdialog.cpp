#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFileDialog>

#include "newdialog.h"
#include "camsettings.h"

newDialog::newDialog(QWidget *parent) :
    QDialog(parent)
{
    createLayout();
    title->setText("Pagoda");
    url->setText("http://174.121.245.214/cp/PagodaCam_1024.jpg");
    dir->setText("C:/Users/Nick/Documents/QTWorkspace/camtmp");
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

    formLayout->addRow(tr("&Title"), title);
    formLayout->addRow(tr("&Url"), url);
    formLayout->addRow(tr("&Dir"), dir);
    formLayout->addRow(tr("&Interval"), interval);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox, Qt::AlignRight);
    setLayout(mainLayout);

    setWindowTitle(tr("Create new webcam recording"));
    setFixedSize(500,sizeHint().height());
}

void newDialog::createSettings()
{
    settings = new CamSettings(QString("%1/%2.cam").arg(dir->text()).arg(title->text()));
    settings->setTitle(title->text());
    settings->setDir(dir->text());
    settings->setUrl(url->text());
    settings->setInterval(interval->value()*1000);
    settings->setAdvTime(false);
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
