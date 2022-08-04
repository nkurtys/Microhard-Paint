#include "mainwindow.h"

//MainWindow constructor
MainWindow::MainWindow() : QMainWindow(), area(new Area(this)){

    this->setCentralWidget(area);
    this->createActions();
    this->createMenus();
    this->setWindowTitle(tr("Macrohard Paint"));
    this->resize(1000, 700);
}

//not own [1] - catch close event and check if wuser wants to save
void MainWindow::closeEvent(QCloseEvent *event){
    if(AreYouSure()){
        event->accept();
    }else{
        event->ignore();
    }
}

//own
void MainWindow::open(){
    if(AreYouSure()){
        if (area->readFromFile()){
            QMessageBox::information(this, tr("WIP"), tr("The File should open here, but it doesn't"), QMessageBox::Ok);
        }
    }
}

//own
void MainWindow::save(){
    if(area->saveToFile()){
        QMessageBox::information(this, tr("Saved!"), tr("File saved!"), QMessageBox::Ok);
    }
}

//own
void MainWindow::clear(){
    if(AreYouSure()){
        area->newImage();
    }
}
//QMessagebox aus Quelle [2]
void MainWindow::about(){
    QMessageBox::information(this, tr("About MH Paint"), tr("Microhard Paint \n by ANONYMOUS ANONYMOUS ANONYMOUS"), QMessageBox::Ok);
}

//Following functions send information to Area about the used tools
void MainWindow::polyTool(){
    QMessageBox::StandardButton box;
    box = QMessageBox::information(this, tr("How to draw a Polygon!"),
                                   tr("Click on the painting area below to start painting! \n"
                                      "If you want to add another corner to your polygon click again to confirm your choice. \n"
                                      "Doubleclick to finish drawing!"), QMessageBox::Ok | QMessageBox::Discard);
    if (box == QMessageBox::Ok){
        area->selectPoly(true);
    }
    else {
        area->selectPoly(false);
    }

}

void MainWindow::rectTool(){
    QMessageBox::StandardButton box;
    box = QMessageBox::information(this, tr("How to draw a Rectangle!"),
                                   tr("Click on the painting area below to start painting! \n"
                                      "Move your mouse over the screen to see a preview of your Rectangle. \n"
                                      "Click again to finish drawing!"), QMessageBox::Ok | QMessageBox::Discard);
    if (box == QMessageBox::Ok){
        area->selectRect(true);
    }
    else {
        area->selectRect(false);
    }

}

void MainWindow::textTool(){

    QMessageBox::StandardButton box;
    box = QMessageBox::information(this, tr("How to add Text!"),
                                       tr("Click on the painting area below to open up a Dialog! \n "
                                          "Enter your text and it will appear where you clicked"),
                                   QMessageBox::Ok | QMessageBox::Discard);
    if (box == QMessageBox::Ok){
            area->selectText(true);
    }
    else {
        area->selectText(false);
    }

}

void MainWindow::SquareTool(){
    QMessageBox::StandardButton box;
    box = QMessageBox::information(this, tr("How to draw a Square!"),
                                   tr("Click on the painting area below to start painting! \n"
                                      "Move your mouse over the scree to see a preview of your Square. \n"
                                      "Click again to finish drawing!"), QMessageBox::Ok | QMessageBox::Discard);
    if (box == QMessageBox::Ok){
        area->selectSquare(true);
    }
    else{
        area->selectSquare(false);
    }
}

//Idee einheitliche Funktion zu machen aus Quelle[3] übernommen
void MainWindow::createActions(){

    //Create open action and connect to MainWindow::open()
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    //create save action and connect to MainWindow::save()
    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    //Create close action and connect to MainWindow::close()
    closeAct = new QAction(tr("&Close..."), this);
    closeAct->setShortcuts(QKeySequence::Close);
    connect(closeAct, &QAction::triggered, this, &MainWindow::close);

    //create clear action and connect to ScribbleArea::clearImage()
    newAct = new QAction(tr("&New..."), this);
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &MainWindow::clear);

    //creat Polygon action and connect to MainWindow::polyTool()
    selectPolyAct = new QAction(tr("&Polygon"), this);
    selectPolyAct->setShortcut(tr("Ctrl+1"));
    connect(selectPolyAct, &QAction::triggered, this, &MainWindow::polyTool);

    //create Rectangle action and connect to MainWindow::rectTool()
    selectRectAct = new QAction(tr("&Rectangle"), this);
    selectRectAct->setShortcut(tr("Ctrl+2"));
    connect(selectRectAct, &QAction::triggered, this, &MainWindow::rectTool);

    selectSquareAct = new QAction(tr("&Square"), this);
    selectSquareAct->setShortcut(tr("Ctrl+3"));
    connect(selectSquareAct, &QAction::triggered, this, &MainWindow::SquareTool);


    //create Text action and connect to MainWindow::textTool()
    selectTextAct = new QAction(tr("&Text"), this);
    selectTextAct->setShortcut(tr("Ctrl+4"));
    connect(selectTextAct, &QAction::triggered, this, &MainWindow::textTool);

    //create about action and connect to MainWindow::about()
    aboutAct = new QAction(tr("&Info..."), this);
    aboutAct->setShortcut(QKeySequence::HelpContents);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
}

//Idee einheitliche Funktion zu machen aus Quelle[1] übernommen
void MainWindow::createMenus(){

    //Attach all actions to File
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(newAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);


    //Attach all actions to Tools
    toolsMenu = new QMenu(tr("&Tools"), this);
    toolsMenu->addAction(selectPolyAct);
    toolsMenu->addAction(selectRectAct);
    toolsMenu->addAction(selectSquareAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(selectTextAct);


    //Attach all actions to Info
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);

    //Add all menu items to menubar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(helpMenu);


}
//checks if user has modified the Area and if yes ask if user wants to save
bool MainWindow::AreYouSure(){
    if(area->isChanged()){
        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, tr("Just think about it..."), tr("Are you sure? You didn't save..."),
                                    QMessageBox::Save | QMessageBox::Yes | QMessageBox::Discard);
        if (box == QMessageBox::Yes){
            return true;
        }
        else if (box == QMessageBox::Save){
            save();
        }
        return false;
    }
    return true;
}


