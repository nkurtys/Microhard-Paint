#include "area.h"

Area::Area(QWidget *parent) : QWidget(parent)
{
    //no drawing at the start
    scribbling = false;
    polyToolSelected = false;
    rectToolSelected = false;
    squareToolSelected = false;
    textToolSelected = false;
    changed = false;
    setMouseTracking(true);

    //on heap to have more space
    polyContainer = new QVector<QPolygon*>;
    rectContainer = new QVector<QRect*>;
    textContainer = new QVector<const QString*>;
}



//own - selects or unselects tool when button pressed
void Area::selectPoly(const bool tool){polyToolSelected = tool; rectToolSelected = false; textToolSelected = false; squareToolSelected = false;}
void Area::selectRect(const bool tool){rectToolSelected = tool; textToolSelected = false; polyToolSelected = false; squareToolSelected = false;}
void Area::selectText(const bool tool){textToolSelected = tool; polyToolSelected = false; rectToolSelected = false; squareToolSelected = false;}
void Area::selectSquare(const bool tool){squareToolSelected = tool; textToolSelected = false; polyToolSelected = false; rectToolSelected = false; }

//If left button pressed and tool selected start drawing and save current position
void Area::mousePressEvent(QMouseEvent *event){

    if(event->button() == Qt::LeftButton){
        if(scribbling){
            lastPoint = event->pos();
            if (polyToolSelected){
                drawLineTo(lastPoint);
                int len = polyContainer->length()-1;

                (*polyContainer)[len]->append(lastPoint);
            }
            else if (rectToolSelected){
                drawRectTo(lastPoint);
                int len = rectContainer->length()-1;
                (*rectContainer)[len]->setTopLeft(lastPoint);
                scribbling = false;

            } else if (squareToolSelected){
                ;
                int len = rectContainer->length()-1;
                (*rectContainer)[len]->setTopLeft(*drawSquareTo(lastPoint));
                scribbling = false;
            }

        }
        else if (!scribbling){
            if (polyToolSelected){
                QPolygon* polygon = new QPolygon;
                *polyContainer << polygon;
                lastPoint = event->pos();
                *polygon << lastPoint;
                scribbling = true;
            }
            else if ((rectToolSelected)||(squareToolSelected)){
                QRect* rect = new QRect;
                rectContainer->append(rect);
                lastPoint = event->pos();
                rect->setBottomRight(lastPoint);
                scribbling = true;
            }
            else if (textToolSelected){
                lastPoint = event->pos();
                bool ok;
                QString t = QInputDialog::getText(this, tr("Text Tool"), tr("Your text here:"), QLineEdit::Normal, QString(), &ok);
                if (ok && !t.isEmpty()){
                    QString* text = new QString(t);
                    textContainer->append(text);
                    textPoints.append(lastPoint);
                    writeTextAt(lastPoint);
                }
                scribbling = false;
            }
        }
    }
}


//own
void Area::mouseMoveEvent(QMouseEvent *event){
    if (scribbling){

        if (polyToolSelected){
            updateArea();
            drawLineTo(event->pos());
        }
        else if (rectToolSelected){
            updateArea();
            drawRectTo(event->pos());

        } else if (squareToolSelected){
            updateArea();
            drawSquareTo(event->pos());
        }
    }
}

//own - only needed for poly tool
void Area::mouseDoubleClickEvent(QMouseEvent *event){
    if ((scribbling)&&(polyToolSelected)){
        drawLineTo(event->pos());
        lastPoint = event->pos();
        int len = polyContainer->length()-1;
       (*polyContainer)[len]->append(lastPoint);
        QPolygon* temp = (*polyContainer)[len];
        drawLineTo(temp->first());
        (*polyContainer)[len]->append(temp->first());
        scribbling = false;
    }
}
//own - deletes the drawn objects during mouse move events and redraws the saved Objects
void Area::updateArea(){

    QImage newImage(canvas.size(), QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.setPen(QPen(Qt::black, 2));

    for (int i = 0; i < polyContainer->size(); i++){

            painter.drawPolyline(*(*polyContainer)[i]);

    }
    if ((rectToolSelected)||(squareToolSelected)){
        for (int i = 0; i < rectContainer->size()-1; i++){
            painter.drawRect(*(*rectContainer)[i]);
        }
    }
    else{
        for (int i = 0; i < rectContainer->size(); i++){
            painter.drawRect(*(*rectContainer)[i]);
        }
    }


    for (int i = 0; i < textContainer->size(); i++){
        QPoint pos = textPoints[i];
        painter.drawText(pos, *(*textContainer)[i]);
    }


    canvas = newImage;
    changed = true;
    update();

}
//own - uses clear() funct from QVector and paints over the canvas
void Area::newImage(){
    QImage newImage(canvas.size(), QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    polyContainer->clear();
    rectContainer->clear();
    textContainer->clear();
    textPoints.clear();
    canvas = newImage;
    changed = true;
    update();
}

//not own [1]
void Area::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect frame = event->rect();
    painter.drawImage(frame, canvas, frame);

}

void Area::drawLineTo(const QPoint &endPoint){
    QPainter painter(&canvas);
    painter.setPen(QPen(Qt::black, 2));
    changed = true;
    painter.drawLine(lastPoint, endPoint);
    /* restricted update on only the rectangle to not update the whole painting [1]*/
    update(QRect(lastPoint, endPoint));
}

void Area::drawRectTo(const QPoint &endPoint){
    QPainter painter(&canvas);
    painter.setPen(QPen(Qt::black, 2));
    changed = true;
    int len = rectContainer->length()-1;
    QPoint bpoint = (*rectContainer)[len]->bottomRight();
    int w = endPoint.x()-bpoint.rx();
    int h = endPoint.y()-bpoint.ry();
    painter.drawRect(bpoint.x(), bpoint.y(), w, h);
    update();
}

QPoint* Area::drawSquareTo(const QPoint &endPoint){
    QPainter painter(&canvas);
    painter.setPen(QPen(Qt::black, 2));
    changed = true;
    int len = rectContainer->length()-1;
    QPoint bpoint = (*rectContainer)[len]->bottomRight();
    int w = std::min(endPoint.x(), endPoint.y());
    painter.drawRect(bpoint.x(), bpoint.y(), w, w);
    int x = bpoint.x()+w;
    int y = bpoint.y()+w;
    QPoint *temp = new QPoint(x, y);
    update();
    return temp;

}

//takes Point of clicking and draws the entered text
void Area::writeTextAt(const QPoint &endPoint){
    QPainter painter(&canvas);
    painter.setPen(QPen(Qt::black, 2));
    changed = true;
    int len = textContainer->length()-1;
    painter.drawText(endPoint, *(*textContainer)[len]);
    update();
}


//not own Quelle [1]
void Area::resizeEvent(QResizeEvent *event)
{
    if (width() > canvas.width() || height() > canvas.height()) {
        int newWidth = std::max(width() + 128, canvas.width());
        int newHeight = std::max(height() + 128, canvas.height());
        resizeImage(&canvas, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
//not own Quelle [1]
void Area::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

//own - Save current Objects into drawing.txt
bool Area::saveToFile(){
    std::ofstream myfile;
    myfile.open ("drawing.txt");
    if (myfile.is_open()){
        myfile << "p \n";
        myfile << polyContainer->size();
        myfile << "\n";
        for (int i = 0; i < polyContainer->size(); i++){
            QPolygon* temp = (*polyContainer)[i];
            myfile << temp->size();
            myfile << "\n";
            for (int j = 0; j < temp->size(); j++){
                int a = (*temp)[j].rx();
                int b = (*temp)[j].ry();
                myfile << a;
                myfile << " ";
                myfile << b;
                myfile << " ";
            }
            myfile << "\n";
        }
        myfile << "r \n";
        myfile << rectContainer->size();
        myfile << "\n";
        for (int i = 0; i < rectContainer->size(); i++){
            QRect* temp = (*rectContainer)[i];
                int a = temp->bottomRight().rx();
                int b = temp->bottomRight().ry();
                int c = temp->topLeft().rx();
                int d = temp->topLeft().ry();

                myfile << a;
                myfile << " ";
                myfile << b;
                myfile << " ";
                myfile << c;
                myfile << " ";
                myfile << d;

                myfile << "\n";
        }
        myfile << "t \n";
        myfile << textContainer->size();
        myfile << "\n";
        for (int i = 0; i < textContainer->size(); i++){
            int a, b;
            a = textPoints[i].rx();
            b = textPoints[i].ry();
            myfile << a;
            myfile << " ";
            myfile << b;
            myfile << " ";
            myfile << (*textContainer)[i];
            myfile << "\n";
        }
        myfile.close();
    }
    this->changed = false;
    return true;
}

//not working
bool Area::readFromFile(){
    std::string kind;
    int count;
    int points;
    std::fstream myfile;
    myfile.open ("drawing.txt");
    if (myfile.is_open()){
        myfile >> kind;
        myfile >> count;
        if (kind == "p"){
            for (int i = 0; i < count; i++){
                QPolygon* pemp = new QPolygon();
                myfile >> points;
                for (int j = 0; j < points; j++){
                    int a, b;
                    myfile >> a;
                    myfile >> b;
                    QPoint temp(a, b);
                    *pemp << temp;
                }
            }
            myfile >> kind;
            myfile >> count;
            if ( kind == "r"){
                for (int i = 0; i < count; i++){
                    QRect *remp = new QRect();
                    int a, b, c, d;
                    myfile >> a;
                    myfile >> b;
                    myfile >> c;
                    myfile >> d;
                    remp->bottomRight().setX(a);
                    remp->bottomRight().setY(b);
                    remp->topLeft().setX(c);
                    remp->topLeft().setY(d);
                    rectContainer->append(remp);
                }
            }
        }
        else if ( kind == "r"){
            for (int i = 0; i < count; i++){
                QRect *remp = new QRect();
                int a, b, c, d;
                myfile >> a;
                myfile >> b;
                myfile >> c;
                myfile >> d;
                remp->bottomRight().setX(a);
                remp->bottomRight().setY(b);
                remp->topLeft().setX(c);
                remp->topLeft().setY(d);
                rectContainer->append(remp);

            }
        }
    }
    changed = true;
    updateArea();
    return true;
}
