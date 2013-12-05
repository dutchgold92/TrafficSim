#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    ui->gfx->show();
    ui->updateIntervalInput->setValue(DEFAULT_UPDATE_INTERVAL * 100);
    ui->densityInput->setValue(DEFAULT_INITIAL_DENSITY * 100);
    ui->displayScaleInput->setValue(DEFAULT_CELL_SIZE);

    this->init_network();

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
    connect(this->scene, SIGNAL(selectionChanged()), this, SLOT(scene_selection()), Qt::QueuedConnection);
    this->update_thread = new NetworkUpdater(this, this->network);
    this->update_thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_network()
{
    Road *road = new Road(25, Cell::left_to_right, 0);
    Road *road2 = new Road(25, Cell::bottom_to_top, 1);
    Road *road3 = new Road(25, Cell::left_to_right, 2);
    Road *road4 = new Road(25, Cell::top_to_bottom, 3);
    Road *road5 = new Road(5, Cell::bottom_to_top, 4);
    Road *road6 = new Road(25, Cell::left_to_right, 5);
    Road *road7 = new Road(25, Cell::left_to_right, 6);
    Road *road8 = new Road(5, Cell::top_to_bottom, 7);
    Road *road9 = new Road(25, Cell::left_to_right, 8);
    Road *road10 = new Road(25, Cell::top_to_bottom, 9);
    Road *road11 = new Road(25, Cell::right_to_left, 10);
    Road *road12 = new Road(10, Cell::top_to_bottom, 11);
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
    Junction *junction7 = new Junction();
    junction7->connect_roads(road11, road12);
    junction7->connect_roads(road4, road12);

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
    this->network = new Network(roads);
}

void MainWindow::draw_network()
{
    this->scene->clear();
    this->process_road(((Road*)this->network->get_roads().front())->get_first_cell(), true, 0, 0);
    this->generation++;
    this->scene->setSceneRect(this->scene->itemsBoundingRect());

    if(this->following_vehicle)
        this->follow_vehicle();
}

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

void MainWindow::process_cell(Cell *cell, qreal x, qreal y)
{
    scene->addItem(new GraphicsCellItem(cell, x, y, this->cell_size, this->cell_size));
    cell->increment_display_generation();

    if(this->show_road_directions && !cell->is_junction())
        this->draw_directional_arrow(x, y, cell->get_direction());
}

void MainWindow::draw_directional_arrow(qreal x, qreal y, Cell::display_direction direction)
{
    qreal arrow_x = x;
    qreal arrow_y = y;
    QString arrow_text;

    switch(direction)
    {
        case Cell::left_to_right:
            arrow_y += this->cell_size;
            arrow_text = ">";
            break;
        case Cell::right_to_left:
            arrow_y += this->cell_size;
            arrow_text = "<";
            break;
        case Cell::top_to_bottom:
            arrow_x += this->cell_size;
            arrow_text = "v";
            break;
        case Cell::bottom_to_top:
            arrow_x += this->cell_size;
            arrow_text = "^";
            break;
    }

    QGraphicsSimpleTextItem *arrow = new QGraphicsSimpleTextItem(0, this->scene);
    arrow->setBrush(QBrush(Qt::white));
    arrow->setText(arrow_text);
    arrow->setX(arrow_x);
    arrow->setY(arrow_y);
    scene->addItem(arrow);
}

void MainWindow::on_updateButton_pressed()
{
    this->update_thread->set_update_interval((float)ui->updateIntervalInput->value() / 100);
    this->network->set_desired_input_density(((float)ui->densityInput->value()) / 100);
}

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

void MainWindow::on_stepButton_pressed()
{
    this->network->step();
    emit(network_updated());
}

void MainWindow::follow_vehicle()
{
    for(QList<QGraphicsItem*>::size_type i = 0; i < this->scene->items().size(); i++)
    {
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

void MainWindow::on_displayScaleInput_valueChanged(int value)
{
    this->cell_size = value;
}

void MainWindow::on_showRoadDirectionsInput_toggled(bool checked)
{
    this->show_road_directions = checked;
}
