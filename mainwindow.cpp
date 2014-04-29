#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow Initialises the MainWindow.
 * @param parent Unused parameter.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->updating = true;
    this->generation = 0;
    this->cell_size = DEFAULT_CELL_SIZE;
    this->show_road_directions = false;
    this->scene = new QGraphicsScene(0, 0, ui->gfx->frameSize().width(), ui->gfx->frameSize().height());
    this->scene->setBackgroundBrush(QBrush(Qt::black));
    this->following_vehicle = false;
    this->vehicle_to_follow = 0;
    ui->gfx->setScene(scene);
    ui->gfx->addAction(ui->actionPlotInputDensity);
    ui->gfx->addAction(ui->actionPlotInputAndOverallDensity);
    ui->gfx->addAction(ui->actionPlotComputationTime);
    ui->gfx->show();
    ui->updateIntervalInput->setValue(DEFAULT_UPDATE_INTERVAL * 100);
    ui->densityInput->setValue(DEFAULT_INITIAL_DENSITY * 100);
    ui->displayScaleInput->setValue(DEFAULT_CELL_SIZE);
    this->plot_widget = 0;
    this->plot_time_steps = DEFAULT_PLOT_TIME_STEPS;
    ui->closePlotButton->hide();

    QString updateValueString;
    updateValueString.append("(");
    updateValueString.append(QString::number(DEFAULT_UPDATE_INTERVAL));
    updateValueString.append(" ms)");
    ui->updateIntervalValueLabel->setText(updateValueString);

    QString densityValueString;
    densityValueString.append("(");
    densityValueString.append(QString::number(DEFAULT_INITIAL_DENSITY));
    densityValueString.append(")");
    ui->densityInputValueLabel->setText(densityValueString);

    this->init_network();

    assert(this->network->get_roads().size() > 0);

    switch(((Road*)this->network->get_roads().front())->get_direction())
    {
        case Cell::left_to_right:
            ui->gfx->setAlignment(Qt::AlignLeft);
            break;
        case Cell::right_to_left:
            ui->gfx->setAlignment(Qt::AlignRight);
            break;
        case Cell::top_to_bottom:
            ui->gfx->setAlignment(Qt::AlignTop);
            break;
        case Cell::bottom_to_top:
            ui->gfx->setAlignment(Qt::AlignBottom);
            break;
    }

    draw_network();

    connect(this, SIGNAL(network_updated()), this, SLOT(draw_network()), Qt::QueuedConnection);
    connect(this, SIGNAL(network_updated()), this, SLOT(plot()), Qt::QueuedConnection);
    connect(this->scene, SIGNAL(selectionChanged()), this, SLOT(scene_selection()), Qt::QueuedConnection);
    this->update_thread = new NetworkUpdater(this, this->network);
    this->update_thread->start();
}

/**
 * @brief MainWindow::~MainWindow Destroys the MainWindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::init_network Initialises the model road network.
 */
void MainWindow::init_network()
{
    // -- Init complex network
    Road *road = new Road(25, Cell::left_to_right);
   Road *road2 = new Road(25, Cell::bottom_to_top);
   Road *road3 = new Road(25, Cell::left_to_right);
   Road *road4 = new Road(25, Cell::top_to_bottom);
   Road *road5 = new Road(5, Cell::bottom_to_top);
   Road *road6 = new Road(25, Cell::left_to_right);
   Road *road7 = new Road(25, Cell::left_to_right);
   Road *road8 = new Road(5, Cell::top_to_bottom);
   Road *road9 = new Road(500, Cell::left_to_right);
   Road *road10 = new Road(25, Cell::top_to_bottom);
   Road *road11 = new Road(25, Cell::right_to_left);
   Road *road12 = new Road(5000, Cell::top_to_bottom);
   Road *road13 = new Road(500, Cell::right_to_left);
   Road *road14 = new Road(25, Cell::right_to_left);
   Road *road15 = new Road(25, Cell::left_to_right);
   Road *road16 = new Road(500, Cell::bottom_to_top);
   Road *road17 = new Road(500, Cell::bottom_to_top);
   Road *road18 = new Road(25, Cell::left_to_right);
   Road *road19 = new Road(1001, Cell::right_to_left);
   Road *road20 = new Road(25, Cell::top_to_bottom);
   Road *road21 = new Road(500, Cell::right_to_left);
   Road *road22 = new Road(495, Cell::top_to_bottom);
   Road *road23 = new Road(500, Cell::left_to_right);

    Junction *junction = new Junction();
    junction->connect_roads(road, road3);
    junction->connect_roads(road2, road3);
    Junction *junction2 = new Junction();
    junction2->connect_roads(road3, road4);
    junction2->connect_roads(road3, road5);
    junction2->connect_roads(road3, road6);
    Junction *junction3 = new Junction();
    junction3->connect_roads(road5, road7);
    Junction *junction4 = new Junction();
    junction4->connect_roads(road7, road8);
    Junction *junction5 = new Junction();
    junction5->connect_roads(road6, road9);
    junction5->connect_roads(road8, road9);
    junction5->connect_roads(road6, road10);
    junction5->connect_roads(road8, road10);
    Junction *junction6 = new Junction();
    junction6->connect_roads(road10, road11);
    junction6->connect_roads(road13, road11);
    junction6->connect_roads(road17, road11);
    Junction *junction7 = new Junction();
    junction7->connect_roads(road11, road12);
    junction7->connect_roads(road11, road14);
    junction7->connect_roads(road4, road12);
    Junction *junction8 = new Junction();
    junction8->connect_roads(road14, road2);
    junction8->connect_roads(road15, road2);
    junction8->connect_roads(road16, road2);
    Junction *junction9 = new Junction();
    junction9->connect_roads(road18, road16);
    Junction *junction10 = new Junction();
    junction10->connect_roads(road19, road17);
    Junction *junction11 = new Junction();
    junction11->connect_roads(road9, road20);
    Junction *junction12 = new Junction();
    junction12->connect_roads(road20, road13);
    junction12->connect_roads(road20, road22);
    junction12->connect_roads(road21, road13);
    junction12->connect_roads(road21, road22);
    Junction *junction13 = new Junction();
    junction13->connect_roads(road22, road23);

    vector<Road*> roads;
    roads.push_back(road);
    roads.push_back(road2);
    roads.push_back(road3);
    roads.push_back(road4);
    roads.push_back(road5);
    roads.push_back(road6);
    roads.push_back(road7);
    roads.push_back(road8);
    roads.push_back(road9);
    roads.push_back(road10);
    roads.push_back(road11);
    roads.push_back(road12);
    roads.push_back(road13);
    roads.push_back(road14);
    roads.push_back(road15);
    roads.push_back(road16);
    roads.push_back(road17);
    roads.push_back(road18);
    roads.push_back(road19);
    roads.push_back(road20);
    roads.push_back(road21);
    roads.push_back(road22);
    roads.push_back(road23);

    this->network = new Network(roads);
}

/**
 * @brief MainWindow::draw_network Draws the model road network unto the GUI.
 */
void MainWindow::draw_network()
{
    this->scene->clear();
    this->process_road(((Road*)this->network->get_roads().front())->get_first_cell(), true, 0, 0);
    this->generation++;
    this->scene->setSceneRect(this->scene->itemsBoundingRect());

    if(this->following_vehicle)
        this->follow_vehicle();
}

/**
 * @brief MainWindow::process_road Processes a road, drawing its cells unto the GUI.
 * @param first_cell Starting cell.
 * @param forward_processing Direction of processing.
 * @param x Drawing coordinate on the x-axis.
 * @param y Drawing coordinate on the y-axis.
 */
void MainWindow::process_road(Cell *first_cell, bool forward_processing, qreal x, qreal y)
{
    Cell *cell = first_cell;

    for(;;)
    {
        if(cell->get_display_generation() <= this->generation)
            this->process_cell(cell, x, y);

        if(cell->is_junction())
        {
            Junction *junction = (Junction*)cell;

            for(unsigned long i = 0; i < junction->get_next_cells().size(); i++)
            {
                Cell *next_cell = junction->get_next_cells().at(i);

                if(next_cell->get_display_generation() <= this->generation)
                {
                    Cell::display_direction direction = next_cell->get_direction();

                    switch(direction)
                    {
                        case Cell::left_to_right:
                            this->process_road(next_cell, true, (x + this->cell_size), y);
                            break;
                        case Cell::right_to_left:
                            this->process_road(next_cell, true, (x - this->cell_size), y);
                            break;
                        case Cell::top_to_bottom:
                            this->process_road(next_cell, true, x, (y + this->cell_size));
                            break;
                        case Cell::bottom_to_top:
                            this->process_road(next_cell, true, x, (y - this->cell_size));
                            break;
                    }
                }
            }

            for(unsigned long i = 0; i < junction->get_previous_cells().size(); i++)
            {
                Cell *previous_cell = junction->get_previous_cells().at(i);

                if(previous_cell->get_display_generation() <= this->generation)
                {
                    Cell::display_direction direction = previous_cell->get_direction();

                    switch(direction)
                    {
                        case Cell::left_to_right:
                            this->process_road(previous_cell, false, (x - this->cell_size), y);
                            break;
                        case Cell::right_to_left:
                            this->process_road(previous_cell, false, (x + this->cell_size), y);
                            break;
                        case Cell::top_to_bottom:
                            this->process_road(previous_cell, false, x, (y - this->cell_size));
                            break;
                        case Cell::bottom_to_top:
                            this->process_road(previous_cell, false, x, (y + this->cell_size));
                            break;
                    }
                }
            }

            return;
        }
        else
        {
            if(forward_processing && cell->has_next_cell())
            {
                Cell::display_direction direction = cell->get_direction();

                switch(direction)
                {
                    case Cell::left_to_right:
                        x += this->cell_size;
                        break;
                    case Cell::right_to_left:
                        x -= this->cell_size;
                        break;
                    case Cell::top_to_bottom:
                        y += this->cell_size;
                        break;
                    case Cell::bottom_to_top:
                        y -= this->cell_size;
                        break;
                }

                cell = cell->get_next_cell(cell);
            }
            else if(!forward_processing && cell->has_previous_cell())
            {
                Cell::display_direction direction = cell->get_direction();

                switch(direction)
                {
                    case Cell::left_to_right:
                        x -= this->cell_size;
                        break;
                    case Cell::right_to_left:
                        x += this->cell_size;
                        break;
                    case Cell::top_to_bottom:
                        y -= this->cell_size;
                        break;
                    case Cell::bottom_to_top:
                        y += this->cell_size;
                        break;
                }

                cell = cell->get_previous_cell(cell);
            }
            else
                return;
        }
    }
}

/**
 * @brief MainWindow::process_cell Processes an individual cell, drawing it unto the GUI.
 * @param cell Cell to represent.
 * @param x Drawing coordinate on the x-axis.
 * @param y Drawing coordinate on the y-axis.
 */
void MainWindow::process_cell(Cell *cell, qreal x, qreal y)
{
    scene->addItem(new GraphicsCellItem(cell, x, y, this->cell_size, this->cell_size));
    cell->increment_display_generation();

    if(this->show_road_directions && cell->is_middle_cell())
        this->draw_directional_arrow(x, y, cell->get_direction());
}

/**
 * @brief MainWindow::draw_directional_arrow Draws an arrow unto the GUI, to indicate the direction of a road.
 * @param x Drawing coordinate on the x-axis.
 * @param y Drawing coordinate on the y-axis.
 * @param direction Direction of arrow to draw.
 */
void MainWindow::draw_directional_arrow(qreal x, qreal y, Cell::display_direction direction)
{
    qreal arrow_x = x;
    qreal arrow_y = y;
    QString arrow_text;

    switch(direction)
    {
        case Cell::left_to_right:
            arrow_y += this->cell_size;
            arrow_text = QString::fromUtf8("→");
            break;
        case Cell::right_to_left:
            arrow_y += this->cell_size;
            arrow_text = QString::fromUtf8("←");
            break;
        case Cell::top_to_bottom:
            arrow_x += this->cell_size;
            arrow_text = QString::fromUtf8("↓");
            break;
        case Cell::bottom_to_top:
            arrow_x += this->cell_size;
            arrow_text = QString::fromUtf8("↑");
            break;
    }

    QGraphicsSimpleTextItem *arrow = new QGraphicsSimpleTextItem(0);
    arrow->setBrush(QBrush(Qt::white));
    arrow->setText(arrow_text);
    arrow->setX(arrow_x);
    arrow->setY(arrow_y);
    scene->addItem(arrow);
}

/**
 * @brief MainWindow::on_playPauseButton_pressed Toggles updating of the simulation.
 */
void MainWindow::on_playPauseButton_pressed()
{
    if(this->updating)
    {
        this->update_thread->stop();
        ui->playPauseButton->setText("Play");
        ui->stepButton->setEnabled(true);
    }
    else
    {
        this->update_thread->start();
        ui->playPauseButton->setText("Pause");
        ui->stepButton->setDisabled(true);
    }

    this->updating = !this->updating;
}

/**
 * @brief MainWindow::on_stepButton_pressed Invokes a single evolution of the model.
 */
void MainWindow::on_stepButton_pressed()
{
    this->network->step();
    emit(network_updated());
}

/**
 * @brief MainWindow::follow_vehicle Tracks the followed vehicle's movement through the model since last generation,
 * setting its new parent cell as selected.
 */
void MainWindow::follow_vehicle()
{
    for(QList<QGraphicsItem*>::size_type i = 0; i < this->scene->items().size(); i++)
    {
        if(this->scene->items().at(i)->type() != GraphicsCellItem::Type)
            continue;

        GraphicsCellItem *cell = ((GraphicsCellItem*)this->scene->items().at(i));

        if(cell->has_vehicle())
        {
            if(cell->get_vehicle() == this->vehicle_to_follow)
            {
                cell->setSelected(true);
                return;
            }
        }
    }

    this->following_vehicle = false;
    this->vehicle_to_follow = 0;
}

/**
 * @brief MainWindow::scene_selection Reacts to selections in the model drawing area of the GUI.
 */
void MainWindow::scene_selection()
{
    if(this->scene->selectedItems().isEmpty())
        return;
    else
    {
        GraphicsCellItem *selected = qgraphicsitem_cast<GraphicsCellItem*>(this->scene->selectedItems().back());
        selected->setSelected(true);
        this->vehicle_to_follow = selected->get_vehicle();
        this->following_vehicle = true;
    }
}
/**
 * @brief MainWindow::on_displayScaleInput_valueChanged Scales display.
 */

void MainWindow::on_displayScaleInput_valueChanged(int value)
{
    this->cell_size = value;
}

/**
 * @brief MainWindow::on_showRoadDirectionsInput_toggled Toggles display of directional arrows next to roads in the GUI.
 */
void MainWindow::on_showRoadDirectionsInput_toggled(bool checked)
{
    this->show_road_directions = checked;
}

/**
 * @brief MainWindow::on_updateIntervalInput_valueChanged Reacts to user modification of the GUI's update interval.
 */
void MainWindow::on_updateIntervalInput_valueChanged(int value)
{
    QString string;
    string.append("(");
    string.append(QString::number(value));
    string.append(" ms)");
    ui->updateIntervalValueLabel->setText(string);
    this->update_thread->set_update_interval((float)ui->updateIntervalInput->value() / 100);
}

/**
 * @brief MainWindow::on_densityInput_valueChanged Reacts to user modification of the model's desired traffic density.
 */
void MainWindow::on_densityInput_valueChanged(int value)
{
    QString string;
    string.append("(");
    string.append(QString::number(float(value) / 100));
    string.append(")");
    ui->densityInputValueLabel->setText(string);
    this->network->set_desired_input_density(((float)ui->densityInput->value()) / 100);
}

/**
 * @brief MainWindow::on_closePlotButton_pressed Closes the plotting pane.
 */
void MainWindow::on_closePlotButton_pressed()
{
    this->plot_widget->hide();
    this->plot_widget = 0;
    this->plot_data_x.clear();
    this->plot_data_y.clear();
    this->plot_data_y2.clear();
    ui->closePlotButton->hide();
}

/**
 * @brief MainWindow::resizeEvent Reacts to user resizing of the window.
 */
void MainWindow::resizeEvent(QResizeEvent *)
{
    if(this->plot_widget != 0 && !this->plot_widget->isHidden())
    {
        this->plot_widget->setMinimumWidth(this->frameSize().width() / 2);
        this->plot_widget->setMinimumHeight(this->frameSize().height() / 2);
    }
}

/**
 * @brief MainWindow::plot Plots model data.
 */
void MainWindow::plot()
{
    if(this->plot_widget != 0)
    {
        switch(this->plot_type)
        {
            case input_density:
                this->plot_data_y.pop_front();
                this->plot_data_y.push_back(this->network->get_actual_input_density());
                this->plot_widget->graph(0)->setData(this->plot_data_x, this->plot_data_y);
                break;
            case overall_density_vs_input_density:
                this->plot_data_y.pop_front();
                this->plot_data_y2.pop_front();
                this->plot_data_y.push_back(this->network->get_actual_input_density());
                this->plot_data_y2.push_back(this->network->get_overall_density());
                this->plot_widget->graph(0)->setData(this->plot_data_x, this->plot_data_y);
                this->plot_widget->graph(1)->setData(this->plot_data_x, this->plot_data_y2);
                this->plot_widget->graph(1)->setPen(QPen(Qt::red));
                break;
            case compute_time:
                this->plot_data_y.pop_front();
                this->plot_data_y.push_back(this->network->get_last_evolution_time());
                this->plot_widget->graph(0)->setData(this->plot_data_x, this->plot_data_y);
                break;
        }

        this->plot_widget->replot();
    }
}

/**
 * @brief MainWindow::on_actionPlotInputDensity_triggered Opens the plotting pane to plot input density,
 * based on user selection.
 */
void MainWindow::on_actionPlotInputDensity_triggered()
{
    if(this->plot_widget != 0)
        this->on_closePlotButton_pressed();

    this->plot_type = input_density;
    ui->plotLayout->removeWidget(this->plot_widget);
    ui->plotLayout->addWidget(this->plot_widget = new QCustomPlot(this->plot_widget));
    this->plot_widget->xAxis->setLabel("Time Steps");
    this->plot_widget->xAxis->setRange(-this->plot_time_steps, 0);
    this->plot_widget->yAxis->setLabel("Input Traffic Density");
    this->plot_widget->yAxis->setRange(0, 1);
    this->plot_widget->setMinimumWidth(this->frameSize().width() / 2);
    this->plot_widget->setMinimumHeight(this->frameSize().height() / 2);
    this->plot_data_x.clear();
    this->plot_data_y.clear();
    this->plot_data_y2.clear();

    for(signed int i = 0; i < this->plot_time_steps; i++)
    {
        this->plot_data_x.push_back(i - this->plot_time_steps);
        this->plot_data_y.push_back(0);
    }

    this->plot_widget->addGraph();
    this->plot();
    this->plot_widget->show();
    ui->closePlotButton->show();
}

/**
 * @brief MainWindow::on_actionPlotInputAndOverallDensity_triggered Opens the plotting pane to plot input density
 * and overall density, based on user selection.
 */
void MainWindow::on_actionPlotInputAndOverallDensity_triggered()
{
    if(this->plot_widget != 0)
        this->on_closePlotButton_pressed();

    this->plot_type = overall_density_vs_input_density;
    ui->plotLayout->removeWidget(this->plot_widget);
    ui->plotLayout->addWidget(this->plot_widget = new QCustomPlot(this->plot_widget));
    this->plot_widget->xAxis->setLabel("Time Steps");
    this->plot_widget->xAxis->setRange(-this->plot_time_steps, 0);
    this->plot_widget->yAxis->setLabel("Input (blue)/Overall (red) Traffic Density");
    this->plot_widget->yAxis->setRange(0, 1);
    this->plot_widget->setMinimumWidth(this->frameSize().width() / 2);
    this->plot_widget->setMinimumHeight(this->frameSize().height() / 2);
    this->plot_data_x.clear();
    this->plot_data_y.clear();
    this->plot_data_y2.clear();

    for(signed int i = 0; i < this->plot_time_steps; i++)
    {
        this->plot_data_x.push_back(i - this->plot_time_steps);
        this->plot_data_y.push_back(0);
        this->plot_data_y2.push_back(0);
    }

    this->plot_widget->addGraph();
    this->plot_widget->addGraph();
    this->plot();
    this->plot_widget->show();
    ui->closePlotButton->show();
}

/**
 * @brief MainWindow::on_actionPlotComputationTime_triggered Plots time taken to compute model
 * in each of the last 20 generations.
 */
void MainWindow::on_actionPlotComputationTime_triggered()
{
    if(this->plot_widget != 0)
        this->on_closePlotButton_pressed();

    this->plot_type = compute_time;
    ui->plotLayout->removeWidget(this->plot_widget);
    ui->plotLayout->addWidget(this->plot_widget = new QCustomPlot(this->plot_widget));
    this->plot_widget->xAxis->setLabel("Model Generation");
    this->plot_widget->xAxis->setRange(-20, 0);
    this->plot_widget->yAxis->setLabel("Computation Time (ms)");
    this->plot_widget->yAxis->setRange(0, (this->network->get_last_evolution_time() * 10));
    this->plot_widget->setMinimumWidth(this->frameSize().width() / 2);
    this->plot_widget->setMinimumHeight(this->frameSize().height() / 2);
    this->plot_data_x.clear();
    this->plot_data_y.clear();
    this->plot_data_y2.clear();

    for(signed int i = 0; i < 20; i++)
    {
        this->plot_data_x.push_back(i - 20);
        this->plot_data_y.push_back(0);
    }

    this->plot_widget->addGraph();
    this->plot();
    this->plot_widget->show();
    ui->closePlotButton->show();
}
