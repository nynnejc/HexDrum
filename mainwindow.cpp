#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    indicator.setParent(this);
    indicator.setFrameShape(QFrame::VLine);
    indicator.resize(4,130);
    for (int instNumber=0; instNumber<5; instNumber++)
    {
        QHBoxLayout *l = new QHBoxLayout(centralWidget());
        ui->verticalLayout->addItem(l);
        QLineEdit *instrument = new QLineEdit(centralWidget());
        l->addWidget(instrument);
        instrument->setProperty("instrument", instNumber);
        connect(instrument, &QLineEdit::editingFinished, this, &MainWindow::onInstrumentRename);
        for (int beat=0; beat<16; beat=beat+1)
        {
            if (beat%4 == 0)
            {
                l->addSpacing(3);
            }
            QCheckBox *cb = new QCheckBox(centralWidget());
            l->addWidget(cb);
            connect(cb, &QCheckBox::toggled, this, &MainWindow::onCheckChanged);
            playNote[instNumber][beat] = false;
            cb->setProperty("instrument", instNumber);
            cb->setProperty("beat", beat);
            if (beat ==0)
                firstCheckBox = cb;
        }

        effect[instNumber].setLoopCount(1);
        effect[instNumber].setVolume(1.0f);
    }
    connect(ui->play, &QPushButton::toggled, this, &MainWindow::onPlay);
    connect(ui->bpm, SIGNAL(valueChanged(int)), this, SLOT(ChangeBpm(int)));

    ui->bpm->setValue(100);
    metronom.setInterval(16*1000/ui->bpm->value());
    metronom.setSingleShot(false);
    connect(&metronom, &QTimer::timeout, this, &MainWindow::onBeat);
    currentBeat=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCheckChanged(bool on)
{
    int instNumber = sender()->property("instrument").toInt();
    int beat = sender()->property("beat").toInt();

    playNote[instNumber][beat] = on;
}

void MainWindow::onPlay(bool on)
{
    if (on)
        metronom.start();
    else
        metronom.stop();
}
void MainWindow::onInstrumentRename()
{
    int instNumber = sender()->property("instrument").toInt();
    QString name = qobject_cast<QLineEdit *>(sender())->text();
    effect[instNumber].setSource(QUrl::fromLocalFile("d:/"+name+ ".wav"));
}

void MainWindow::ChangeBpm(int bpm)
{
    metronom.setInterval(16*1000/bpm);
}

void MainWindow::onBeat()
{
    int bredde = width() - firstCheckBox->x();

    indicator.move(firstCheckBox->x() + currentBeat *bredde/16, 40);

    for (int instNumber=0; instNumber<5; instNumber++)
        if(playNote[instNumber][currentBeat] == true)
           effect[instNumber].play();

    if (currentBeat==15)
        currentBeat=0;
    else
        currentBeat++;
}
