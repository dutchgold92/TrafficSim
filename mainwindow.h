#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <network.h>
#include <networkupdater.h>
#include <QGraphicsScene>
#include <graphicscellitem.h>
#include <QGraphicsSimpleTextItem>
#include <QBrush>
#include <QtConcurrentRun>
#include <unistd.h>
#include <assert.h>
#include <qcustomplot.h>

#define DEFAULT_CELL_SIZE 10
#define DEFAULT_PLOT_TIME_STEPS 50

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    void init_network();
    void process_road(Cell *first_cell, bool forward_processing, qreal x, qreal y);
    void process_cell(Cell *cell, qreal x, qreal y);
    void follow_vehicle();
    void draw_directional_arrow(qreal x, qreal y, Cell::display_direction direction);
    unsigned long generation;
    Ui::MainWindow *ui;
    Network *network;
    QGraphicsScene *scene;
    bool updating;
    NetworkUpdater *update_thread;
    bool following_vehicle;
    Vehicle *vehicle_to_follow;
    qreal cell_size;
    bool show_road_directions;
    QCustomPlot *plot_widget;
    enum Plot_Type {input_density, overall_density_vs_input_density};
    Plot_Type plot_type;
    signed int plot_time_steps;
    QVector<double> plot_data_x;
    QVector<double> plot_data_y;
    QVector<double> plot_data_y2;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void draw_network();
    void on_playPauseButton_pressed();
    void on_stepButton_pressed();
    void scene_selection();
    void on_displayScaleInput_valueChanged(int value);
    void on_showRoadDirectionsInput_toggled(bool checked);
    void on_updateIntervalInput_valueChanged(int value);
    void on_densityInput_valueChanged(int value);
    void on_closePlotButton_pressed();
    void resizeEvent(QResizeEvent *);
    void plot(); 
    void on_actionPlotInputAndOverallDensity_triggered();
    void on_actionPlotInputDensity_triggered();
signals:
    void network_updated();
};

#endif // MAINWINDOW_H
