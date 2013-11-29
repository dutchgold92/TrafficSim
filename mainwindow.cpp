#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->updating = true;
    this->generation = 0;
    this->scene = new QGraphicsScene(0, 0, ui->gfx->frameSize().width(), ui->gfx->frameSize().height());
    ui->gfx->setScene(scene);
    ui->gfx->show();
    ui->updateIntervalInput->setValue(DEFAULT_UPDATE_INTERVAL * 100);
    ui->densityInput->setValue(DEFAULT_INITIAL_DENSITY * 100);

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
    Road *road5 = new Road(25, Cell::bottom_to_top, 4);
    Road *road6 = new Road(25, Cell::left_to_right, 5);
    Junction *junction = new Junction();
    junction->connect_roads(road, road3);
    junction->connect_roads(road2, road3);
    Junction *junction2 = new Junction();
    junction2->connect_roads(road3, road4);
    junction2->connect_roads(road3, road5);
    junction2->connect_roads(road3, road6);
    vector<Road*> roads;
    roads.push_back(road);
    roads.push_back(road2);
    roads.push_back(road3);
    roads.push_back(road4);
    roads.push_back(road5);
    roads.push_back(road6);
    this->network = new Network(roads);
}

void MainWindow::draw_network()
{
    this->scene->clear();
    this->process_road(((Road*)this->network->get_roads().front())->get_first_cell(), true, 0, 0);
    this->generation++;
    this->scene->setSceneRect(this->scene->itemsBoundingRect());

    /*// FIXME: debugging - testing cell display gens
    for(vector<Road*>::size_type i = 0; i < this->network->get_roads().size(); i++)
        for(unsigned long x = 0; x < this->network->get_roads().at(i)->get_length(); x++)
            cout << this->network->get_roads().at(i)->get_cell(x)->get_display_generation() << ",";
    cout << endl;*/

    /*for(int i = 0; i < 25; i++)
    {
        if(this->network->get_roads().at(3)->get_cell(i)->has_vehicle())
            cout << this->network->get_roads().at(3)->get_cell(i)->get_vehicle()->get_velocity();
        else
            cout << ".";
    }

    cout << endl;*/
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
                            this->process_road(next_cell, true, (x + DISPLAY_CELL_SIZE), y);
                            break;
                        case Cell::right_to_left:
                            this->process_road(next_cell, true, (x - DISPLAY_CELL_SIZE), y);
                            break;
                        case Cell::top_to_bottom:
                            this->process_road(next_cell, true, x, (y + DISPLAY_CELL_SIZE));
                            break;
                        case Cell::bottom_to_top:
                            this->process_road(next_cell, true, x, (y - DISPLAY_CELL_SIZE));
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
                            this->process_road(previous_cell, false, (x - DISPLAY_CELL_SIZE), y);
                            break;
                        case Cell::right_to_left:
                            this->process_road(previous_cell, false, (x + DISPLAY_CELL_SIZE), y);
                            break;
                        case Cell::top_to_bottom:
                            this->process_road(previous_cell, false, x, (y - DISPLAY_CELL_SIZE));
                            break;
                        case Cell::bottom_to_top:
                            this->process_road(previous_cell, false, x, (y + DISPLAY_CELL_SIZE));
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
                        x += DISPLAY_CELL_SIZE;
                        break;
                    case Cell::right_to_left:
                        x -= DISPLAY_CELL_SIZE;
                        break;
                    case Cell::top_to_bottom:
                        y += DISPLAY_CELL_SIZE;
                        break;
                    case Cell::bottom_to_top:
                        y -= DISPLAY_CELL_SIZE;
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
                        x -= DISPLAY_CELL_SIZE;
                        break;
                    case Cell::right_to_left:
                        x += DISPLAY_CELL_SIZE;
                        break;
                    case Cell::top_to_bottom:
                        y -= DISPLAY_CELL_SIZE;
                        break;
                    case Cell::bottom_to_top:
                        y += DISPLAY_CELL_SIZE;
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
    QGraphicsRectItem *display_cell = new QGraphicsRectItem(x, y, DISPLAY_CELL_SIZE, DISPLAY_CELL_SIZE);

    if(cell->has_vehicle())
        display_cell->setBrush(QBrush(Qt::blue));
    else
        display_cell->setBrush(QBrush(Qt::black));

    if(cell->is_junction())
        display_cell->setPen(QPen(Qt::red));

    scene->addItem(display_cell);
    cell->increment_display_generation();
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
