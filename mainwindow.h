#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <network.h>
#include <networkupdater.h>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QtConcurrentRun>
#include <unistd.h>

#define DEFAULT_UPDATE_INTERVAL 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    Network *network;
    QGraphicsScene *scene;
    bool updating;
    NetworkUpdater *update_thread;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void draw_network();
    void on_updateButton_pressed();
    void on_playPauseButton_pressed();
    void on_stepButton_pressed();
signals:
    void network_updated();
};

#endif // MAINWINDOW_H
