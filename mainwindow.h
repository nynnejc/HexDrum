#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSoundEffect>
#include <QFrame>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void onCheckChanged(bool on);
    Ui::MainWindow *ui;
    QTimer metronom;
    QSoundEffect effect[5];
    bool playNote[5][16];
    int currentBeat;
    QWidget *firstCheckBox;
    QFrame indicator;
    void onPlay (bool on);
    void onInstrumentRename();
private slots:
    void ChangeBpm(int bpm);
    void onBeat ();
};

#endif // MAINWINDOW_H
