#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QtGlobal>
#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    qDebug() << "P_CW_DEAD" << servo.readByte(getID(), P_CW_DEAD);
    qDebug() << "P_CCW_DEAD" << servo.readByte(getID(), P_CCW_DEAD);

    qDebug() << "P_MAX_TORQUE_L" << servo.readWord(getID(), P_MAX_TORQUE_L);

    for(int i = 28; i < 40; i++) {
        qDebug() << "read" << i << servo.readByte(getID(), i);
    }



    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this] {
        auto pos = servo.ReadPos(getID());
        ui->currentposSlider->setValue(pos);
        ui->labelPos->setText(QString::number(pos));
    });
    timer->start(100);

    connect(ui->setposSlider, &QSlider::valueChanged, this, [this](int value) {
        servo.WritePos(getID(), value, 500);
    });

    connect(ui->setposSpin, &QSpinBox::editingFinished, this, [this]() {
        servo.WritePos(getID(), ui->setposSpin->value(), 500);
    });

    connect(ui->enableTorque, &QCheckBox::toggled, this, [this](bool enabled) {
       servo.EnableTorque(getID(), enabled? 1 : 0);
    });

    connect(ui->readByteBtn, &QPushButton::clicked, this, [this] {
       ui->labelReadByte->setText(QString::number(servo.readByte(getID(), ui->regSpinbox->value())));
    });

    connect(ui->regSpinbox, &QSpinBox::editingFinished, this, [this] {
       ui->labelReadByte->setText(QString::number(servo.readByte(getID(), ui->regSpinbox->value())));
    });

    connect(ui->regSpinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this] {
        ui->labelReadByte->clear();
    });

    connect(ui->writeByteBtn, &QPushButton::clicked, this, [this] {
       servo.writeByte(getID(), ui->writeReg->value(), ui->writeValue->value());
    });

    connect(ui->setIDBtn, &QPushButton::clicked, this, [this] {
          if (QMessageBox::warning(this, "Set ID", "Really change servo ID?",
                                    QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
            servo.writeByte(getID(), P_LOCK, 0);
            servo.writeByte(getID(), P_ID, ui->setIdEdit->value());
            servo.writeByte(getID(), P_LOCK, 1);
          }
    });

    connect(ui->btnDefParams, &QPushButton::clicked, this, [this] {
        servo.writeByte(getID(), P_CCW_DEAD, 0);
        servo.writeByte(getID(), P_CW_DEAD, 0);

        servo.writeByte(getID(), P_COMPLIANCE_P, 25); // P
        servo.writeByte(getID(), P_COMPLIANCE_I, 25); // I

        servo.writeWord(getID(), P_INTEGRAL_LIMIT_L, 70); // max Isum

        servo.writeByte(getID(), 30, 5);
        servo.writeByte(getID(), 31, 5);
        servo.writeByte(getID(), 32, 5);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getID()
{
    return ui->currentID->value();
}
