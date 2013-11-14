#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->updating = true;
    this->scene = new QGraphicsScene(0, 0, ui->gfx->frameSize().width(), ui->gfx->frameSize().height());
    ui->gfx->setScene(scene);
    ui->gfx->show();
    ui->updateIntervalInput->setValue(DEFAULT_UPDATE_INTERVAL);
    ui->densityInput->setValue(DEFAULT_INITIAL_DENSITY * 100);

    Road *road = new Road(50);
    Road *road2 = new Road(25);
    Road *road3 = new Road(70);
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
    this->scene->clear();

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
