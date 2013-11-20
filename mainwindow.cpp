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
    ui->updateIntervalInput->setValue(DEFAULT_UPDATE_INTERVAL);
    ui->densityInput->setValue(DEFAULT_INITIAL_DENSITY * 100);

    Road *road = new Road(25, Cell::left_to_right);
    Road *road2 = new Road(25, Cell::bottom_to_top);
    Road *road3 = new Road(25, Cell::left_to_right);
    Junction *junction = new Junction();
    junction->connect_roads(road, road3);
    junction->connect_roads(road2, road3);
    vector<Road*> roads;
    roads.push_back(road);
    roads.push_back(road2);
    roads.push_back(road3);
    this->network = new Network(roads);

    /*// -- Start display
    for(;;)
    {
        bool empty = true;

        for(Cell *c = r->get_first_cell(); c->has_next_cell(); c = c->get_next_cell(c))
        {
            if(c->is_junction())
            {
                cout << "|";

                if(c->has_vehicle())
                    cout << c->get_vehicle()->get_velocity();
                else
                    cout << "X";

                cout << "|";
            }
            else if(c->has_vehicle())
            {
                cout << c->get_vehicle()->get_velocity();
                empty = false;
            }
            else
                cout << ".";
        }

        cout << "\n" << endl;

        if(empty)
            return 0;

        n->step();
    }
    // -- End display*/

    connect(this, SIGNAL(network_updated()), this, SLOT(draw_network()), Qt::QueuedConnection);
    this->update_thread = new NetworkUpdater(this, this->network);
    this->update_thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::draw_network()
{
    /*this->scene->clear();

    for(unsigned short x = 0; x < network->get_roads().size(); x++)
    {
        Road *road = network->get_roads().at(x);

        for(unsigned long i = 0; i < road->get_length(); i++)
        {
            Cell *cell = road->get_cell(i);
            QGraphicsRectItem *cell_item = new QGraphicsRectItem((i * 10), (x * 10), 10, 10);

            if(cell->is_junction())
                cell_item->setPen(QPen(Qt::red));

            if(cell->has_vehicle())
                cell_item->setBrush(QBrush(Qt::blue));
            else
                cell_item->setBrush(QBrush(Qt::black));

            scene->addItem(cell_item);
        }
    }

    for(unsigned short i = 0; i < this->network->get_junctions().size(); i++)
    {
        Cell *cell = this->network->get_junctions().at(i);
        QGraphicsRectItem *j_item = new QGraphicsRectItem((i * 10), (100), 10, 10);
        j_item->setPen(QPen(Qt::red));

        if(cell->has_vehicle())
            j_item->setBrush(QBrush(Qt::blue));
        else
            j_item->setBrush(QBrush(Qt::black));

        scene->addItem(j_item);

    }*/

    this->scene->clear();
    this->process_road(((Road*)this->network->get_roads().front())->get_first_cell(), true, 0, 0);
    this->generation++;
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
                            x += DISPLAY_CELL_SIZE;
                            break;
                        case Cell::right_to_left:
                            x -= DISPLAY_CELL_SIZE;
                            break;
                        case Cell::top_to_bottom:
                            y -= DISPLAY_CELL_SIZE;
                            break;
                        case Cell::bottom_to_top:
                            y += DISPLAY_CELL_SIZE;
                            break;
                    }

                    this->process_road(next_cell, true, x, y);
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
                            x -= DISPLAY_CELL_SIZE;
                            break;
                        case Cell::right_to_left:
                            x += DISPLAY_CELL_SIZE;
                            break;
                        case Cell::top_to_bottom:
                            y += DISPLAY_CELL_SIZE;
                            break;
                        case Cell::bottom_to_top:
                            y -= DISPLAY_CELL_SIZE;
                            break;
                    }

                    this->process_road(previous_cell, false, x, y);
                }
            }

            return;
        }

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
                    y -= DISPLAY_CELL_SIZE;
                    break;
                case Cell::bottom_to_top:
                    y += DISPLAY_CELL_SIZE;
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
                    y += DISPLAY_CELL_SIZE;
                    break;
                case Cell::bottom_to_top:
                    y -= DISPLAY_CELL_SIZE;
                    break;
            }

            cell = cell->get_previous_cell(cell);
        }
        else
            return;
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
    this->update_thread->set_update_interval(ui->updateIntervalInput->value());
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
