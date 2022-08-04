#ifndef AREA_H
#define AREA_H

#include <QtWidgets>
#include <QPainter>
#include <iostream>
#include <fstream>



class Area : public QWidget
{
    Q_OBJECT

private:

    QVector<QPolygon*> *polyContainer;
    QVector<QRect*> *rectContainer;
    QVector<const QString*> *textContainer;
    QVector<QPoint> textPoints;
    QImage canvas;
    QPoint lastPoint;
    bool scribbling;
    bool polyToolSelected;
    bool rectToolSelected;
    bool squareToolSelected;
    bool textToolSelected;
    bool changed;
    void drawLineTo(const QPoint &endPoint);
    void drawRectTo(const QPoint &endPoint);
    QPoint* drawSquareTo(const QPoint &endPoint);
    void writeTextAt(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event)override;

public:
    Area(QWidget* parent = nullptr);

    //Events
    bool saveToFile();
    bool readFromFile();
    void selectPoly(const bool tool);
    void selectRect(const bool tool);
    void selectText(const bool tool);
    void selectSquare(const bool tool);

    //has area been modified
    bool isChanged() { return changed;}

public slots:
    void newImage();
    void updateArea();

};

#endif // AREA_H
