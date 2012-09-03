#include <QVBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFileDialog>

#include "newdialog.h"

newDialog::newDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QFormLayout *formLayout = new QFormLayout;
    QWidget *fBox = new QWidget;

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    title = new QLineEdit;
    url = new QLineEdit;
    dir = new dirComboBox;
    interval = new QSpinBox;
    interval->setSuffix("sec");
    interval->setMaximum(86400);
    interval->setValue(60);

    formLayout->addRow(tr("&Title"), title);
    formLayout->addRow(tr("&Url"), url);
    formLayout->addRow(tr("&Dir"), dir);
    formLayout->addRow(tr("&Interval"), interval);

    fBox->setLayout(formLayout);
    mainLayout->addWidget(fBox);
    mainLayout->addWidget(buttonBox, Qt::AlignRight);
    setLayout(mainLayout);

    setWindowTitle(tr("Create new webcam recording"));
    setFixedSize(500,sizeHint().height());
}

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
