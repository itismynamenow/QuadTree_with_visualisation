#ifndef QTREEVISUALISATIONWIDGET_H
#define QTREEVISUALISATIONWIDGET_H

#include <QtWidgets>
#include <iostream>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>
#include <tuple>
#include <ctime>
#include <cmath>

#include "quad_tree.h"
#include "quad_tree_slow.h"
using std::vector;
using std::array;
using std::shared_ptr;
using std::unique_ptr;
using std::tuple;

struct MyCustomElement: public QuadTreeElement<int>{
    MyCustomElement(){
        init();
    }
    MyCustomElement(const AABB<int> &aabb, QColor color=Qt::yellow): QuadTreeElement(aabb), color(color){
        pos = QVector2D(aabb.xMin,aabb.yMin);
        init();
    }
    void init(){
        QMatrix4x4 rotationMat;
        rotationMat.rotate(rand()%360,0,0,1);
        direction = (QVector2D(1,0).toVector4D()*rotationMat).toVector2D();
        speed = 1+rand()%3;
    }
    void update(){
        pos = pos + direction * speed;
        this->aabb.translateBy(pos.x()-this->aabb.xMin,pos.y()-this->aabb.yMin);
    }
    QColor color;
    float speed = 6;
    QVector2D direction;
    QVector2D pos;
};

struct MyCustomElementsHolder{
    MyCustomElementsHolder():quadTree(new QuadTreeSlow<int>()){}
    void addElement(const AABB<int> &aabb, QColor color=Qt::yellow){
        elementsPtrs.push_back(std::make_shared<MyCustomElement>(MyCustomElement(aabb,color)));
        vector<shared_ptr<QuadTreeElement<int>>> elementsCastedPtrs(elementsPtrs.begin(),elementsPtrs.end());
        quadTree->setElements(elementsCastedPtrs,AABB<int>(0,0,799,799));
    }
    void addElements(vector<AABB<int>> aabbs){
        for(auto aabb: aabbs){
            elementsPtrs.push_back(std::make_shared<MyCustomElement>(MyCustomElement(aabb)));
        }
        vector<shared_ptr<QuadTreeElement<int>>> elementsCastedPtrs(elementsPtrs.begin(),elementsPtrs.end());
        quadTree->setElements(elementsCastedPtrs,boundingBox,6,4);
    }
    vector<shared_ptr<MyCustomElement>> getOverlappingObjects(){
//        vector<shared_ptr<MyCustomElement>> overlappingElements;
//        auto elementPtrsTuples = quadTree->getAllOverlappingElementTuples();
//        if(elementPtrsTuples.size() > 0){
//            for(auto &tuple: elementPtrsTuples){
//                overlappingElements.push_back(std::static_pointer_cast<MyCustomElement>(std::get<0>(tuple)));
//                overlappingElements.push_back(std::static_pointer_cast<MyCustomElement>(std::get<1>(tuple)));
//            }
//            std::sort(overlappingElements.begin(),overlappingElements.end());
//            overlappingElements.erase(
//                        std::unique(overlappingElements.begin(),overlappingElements.end()),
//                        overlappingElements.end());
//        }

//        return overlappingElements;
        auto overlappingElements = quadTree->getAllOverlappingElements();
        vector<shared_ptr<MyCustomElement>> overlappingElementsCasted(overlappingElements.size());
        for(int i=0;i<overlappingElements.size();i++){
            overlappingElementsCasted.at(i) = std::static_pointer_cast<MyCustomElement>(overlappingElements.at(i));
        }
        return overlappingElementsCasted;
    }
    void reset(){
        elementsPtrs.clear();
        quadTree->reset();
    }
    void update(){

        auto overlappingTuples = quadTree->getAllOverlappingElementTuples();
        for(auto overlapingTuple: overlappingTuples){
            auto element0 = std::static_pointer_cast<MyCustomElement>(std::get<0>(overlapingTuple));
            auto element1 = std::static_pointer_cast<MyCustomElement>(std::get<1>(overlapingTuple));
            int width0 =  element0->aabb.xMax - element0->aabb.xMin;
            int height0 = element0->aabb.yMax - element0->aabb.yMin;
            int width1 =  element1->aabb.xMax - element1->aabb.xMin;
            int height1 = element1->aabb.yMax - element1->aabb.yMin;
            int x0Ctr = element0->aabb.xMin + width0 /2;
            int y0Ctr = element0->aabb.yMin + height0/2;
            int x1Ctr = element1->aabb.xMin + width1 /2;
            int y1Ctr = element1->aabb.yMin + height1/2;
            int xOverlap = abs(abs(x0Ctr-x1Ctr) - (width0+width1)/2);
            int yOverlap = abs(abs(y0Ctr-y1Ctr) - (height0+height1)/2);
            if(xOverlap < yOverlap){
                if(x0Ctr < x1Ctr){
                    element0->direction.setX(-abs(element0->direction.x()));
                    element1->direction.setX( abs(element1->direction.x()));
//                    element0->aabb.translateBy(-xOverlap/2,0);
//                    element1->aabb.translateBy( xOverlap/2,0);
                }else{
                    element0->direction.setX( abs(element0->direction.x()));
                    element1->direction.setX(-abs(element1->direction.x()));
//                    element0->aabb.translateBy( xOverlap/2,0);
//                    element1->aabb.translateBy(-xOverlap/2,0);
                }
//                element0->direction.setX(-element0->direction.x());
//                element1->direction.setX(-element1->direction.x());
            }else{
                if(y0Ctr < y1Ctr){
                    element0->direction.setY(-abs(element0->direction.y()));
                    element1->direction.setY( abs(element1->direction.y()));
//                    element0->aabb.translateBy(0,-yOverlap/2);
//                    element0->aabb.translateBy(0, yOverlap/2);
                }else{
                    element0->direction.setY( abs(element0->direction.y()));
                    element1->direction.setY(-abs(element1->direction.y()));
//                    element0->aabb.translateBy(0, yOverlap/2);
//                    element0->aabb.translateBy(0,-yOverlap/2);
                }
//                element0->direction.setY(-element0->direction.y());
//                element1->direction.setY(-element1->direction.y());
            }
//            element0->speed = 0;
//            element1->speed = 0;

        }
        for(auto element: elementsPtrs){

            if(element->aabb.xMin < boundingBox.xMin){
                element->direction.setX(abs(element->direction.x()));
            }
            if(element->aabb.xMax > boundingBox.xMax){
                element->direction.setX(-abs(element->direction.x()));
            }
            if(element->aabb.yMin < boundingBox.yMin){
                element->direction.setY(abs(element->direction.y()));
            }
            if(element->aabb.yMax > boundingBox.yMax){
                element->direction.setY(-abs(element->direction.y()));
            }
            element->update();
        }
    }
    vector<shared_ptr<MyCustomElement>> elementsPtrs;
    unique_ptr<QuadTree<int>> quadTree;
    AABB<int> boundingBox{0,0,799,799};
};

class QTreeVisualisationWidget: public QWidget
{
    Q_OBJECT
    typedef unique_ptr<QuadTreeNode<int>> NODE_U_PTR;
    typedef shared_ptr<QuadTreeNode<int>> NODE_S_PTR;
public:
    QTreeVisualisationWidget(){
        this->setMinimumSize(800,800);
        connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
        timer.start(updateIntervalMs);
        srand( time( 0 ) );
    }
    void setPoints(vector<QPoint> points){
        holder.reset();
        holder.boundingBox = AABB<int>(0,0,this->width(),this->height());
        holder.addElements(pointsToElements(points));
        this->update();
    }
public slots:
    void reset(){
        points.clear();
        holder.reset();
//        this->update();
    }
protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent) override{
        bool callUpdate = false;
        std::cout<<mouseEvent->pos().x()<<" "<<mouseEvent->pos().y()<<std::endl;
        if(mouseEvent->buttons() == Qt::RightButton){
            if(timer.isActive()){
                timer.stop();
            }else{
                timer.start(updateIntervalMs);
            }
            callUpdate = true;
        }else if(mouseEvent->buttons() == Qt::LeftButton){
            points.push_back(mouseEvent->pos());
            this->sendPointsToQTree();
            callUpdate = true;
        }
//        if(callUpdate) this->update();
    }
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        sendPointsToQTree();

        if(holder.quadTree->getRootNode()){
            drawSubtree(painter,holder.quadTree->getRootNode());
        }

        for(auto element: holder.elementsPtrs){
            painter.setBrush(QBrush(Qt::cyan));
            int width = element->aabb.xMax - element->aabb.xMin;
            int height = element->aabb.yMax - element->aabb.yMin;
            painter.drawRect(element->aabb.xMin,element->aabb.yMin,width,height);
        }
//        for(QPoint point: points){
//            painter.setBrush(QBrush(Qt::yellow));
//            painter.drawRect(point.x(),point.y(),quadSide,quadSide);
//        }

        auto overlappingObjects = holder.getOverlappingObjects();
        for(auto overlappingObject: overlappingObjects){
            painter.setBrush(QBrush(Qt::red));
            int width = overlappingObject->aabb.xMax - overlappingObject->aabb.xMin;
            int height = overlappingObject->aabb.yMax - overlappingObject->aabb.yMin;
            painter.drawRect(overlappingObject->aabb.xMin,overlappingObject->aabb.yMin,width,height);
        }

    }
    void drawSubtree(QPainter &painter,const NODE_U_PTR &node){
        if(!node || node->getElements().size()>0){
        }else{
            int xCtr = node->getBoundingBox().xMin + (node->getBoundingBox().xMax - node->getBoundingBox().xMin)/2;
            int yCtr = node->getBoundingBox().yMin + (node->getBoundingBox().yMax - node->getBoundingBox().yMin)/2;
            painter.drawLine(node->getBoundingBox().xMin,yCtr,node->getBoundingBox().xMax,yCtr);
            painter.drawLine(xCtr,node->getBoundingBox().yMin,xCtr,node->getBoundingBox().yMax);
            for(auto &child: node->getChildren()){
                drawSubtree(painter,child);
            }
        }
    }

    void drawSubtree(QPainter &painter,NODE_S_PTR node){
        if(!node || node->getElements().size()>0){
        }else{
            int xCtr = node->getBoundingBox().xMin + (node->getBoundingBox().xMax - node->getBoundingBox().xMin)/2;
            int yCtr = node->getBoundingBox().yMin + (node->getBoundingBox().yMax - node->getBoundingBox().yMin)/2;
            painter.drawLine(node->getBoundingBox().xMin,yCtr,node->getBoundingBox().xMax,yCtr);
            painter.drawLine(xCtr,node->getBoundingBox().yMin,xCtr,node->getBoundingBox().yMax);
            for(auto &child: node->getChildren()){
                drawSubtree(painter,child);
            }
        }
    }
    void sendPointsToQTree(){
        holder.update();
        holder.boundingBox = AABB<int>(0,0,this->width(),this->height());
        holder.addElements(pointsToElements(points));
        points.clear();
//        update();
    }
    vector<AABB<int>> pointsToElements(vector<QPoint> &points){
        vector<AABB<int>> elements;
        for(auto point: points){
            int width = 10 + point.x()+rand()%30;
            int height = 10 + point.y()+rand()%30;
            elements.push_back(AABB<int>(point.x(),point.y(),width,height));
        }
        return  elements;
    }

    MyCustomElementsHolder holder;
    int quadSide = 5;
    vector<QPoint> points;
    QTimer timer{this};
    int updateIntervalMs = 30;
};

#endif // QTREEVISUALISATIONWIDGET_H
