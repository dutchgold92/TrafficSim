#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <network.h>
#include <networkupdater.h>
#include <QGraphicsScene>
#include <graphicscellitem.h>
#include <QBrush>
#include <QtConcurrentRun>
#include <unistd.h>

#define DISPLAY_CELL_SIZE 10

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    unsigned long generation;
    Ui::MainWindow *ui;
    Network *network;
    QGraphicsScene *scene;
    bool updating;
    NetworkUpdater *update_thread;
    void init_network();
    void process_road(Cell *first_cell, bool forward_processing, qreal x, qreal y);
    void process_cell(Cell *cell, qreal x, qreal y);
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
