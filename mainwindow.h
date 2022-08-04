#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "area.h"

class Area;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Area *area;
    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;
    QActionGroup *AGroup;
    QAction *openAct;
    QAction *saveAct;
    QAction *closeAct;
    QAction *newAct;
    QAction *selectPolyAct;
    QAction *selectRectAct;
    QAction *selectTextAct;
    QAction *selectSquareAct;
    QAction *aboutAct;
    void createActions();
    void createMenus();
    bool AreYouSure();

protected:

    void closeEvent(QCloseEvent *event) override;

public:
    MainWindow();

private slots:
    void open();
    void save();
    void about();
    void clear();
    void polyTool();
    void rectTool();
    void textTool();
    void SquareTool();



};
#endif // MAINWINDOW_H
