//#include "qtreevisualisationwidget.h"

//QTreeVisualisationWidget::QTreeVisualisationWidget()
//{
//    this->setMinimumSize(800,800);
//}

//void QTreeVisualisationWidget::setPoints(vector<QPoint> points)
//{
//    this->points.clear();
//    this->points = points;
//    sendPointsToQTree();
//    this->update();
//}

//void QTreeVisualisationWidget::reset()
//{
//    points.clear();
//    quadTree.clear();
//    this->update();
//}

//void QTreeVisualisationWidget::mousePressEvent(QMouseEvent *mouseEvent)
//{
//    bool callUpdate = false;
//    cout<<mouseEvent->pos().x()<<" "<<mouseEvent->pos().y()<<endl;
//    if(mouseEvent->buttons() == Qt::RightButton){
//        callUpdate = true;
//    }else if(mouseEvent->buttons() == Qt::LeftButton){
//        points.push_back(mouseEvent->pos());
//        this->sendPointsToQTree();
//        callUpdate = true;
//    }
//    if(callUpdate) this->update();
//}

//void QTreeVisualisationWidget::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);

//    if(quadTree.nodes.size()>0){
//        drawSubtree(painter,quadTree.rootId,800,800,0,0);
//    }

//    for(QPoint point: points){
//        painter.setBrush(QBrush(Qt::yellow));
//        painter.drawRect(point.x(),point.y(),quadSide,quadSide);
//    }

//    vector<int> collidingPointsIds = quadTree.getCollidingElementsIds();
//    for(int id: collidingPointsIds){
//        painter.setBrush(QBrush(Qt::red));
//        painter.drawRect(points.at(id).x(),points.at(id).y(),quadSide,quadSide);
//    }

//}

//void QTreeVisualisationWidget::drawSubtree(QPainter &painter, int currNodeId, int maxX, int maxY, int minX, int minY)
//{
//    if(currNodeId==-1) return;

//    QuadTreeNode currNode = quadTree.nodes[currNodeId];
//    bool isLeaf = true;
//    for(int i=0;i<4;i++){
//        if(currNode.childrenId.at(i)!=-1) isLeaf = false;
//    }

//    if(!isLeaf){
//        int centerX = minX+(maxX-minX)/2;
//        int centerY = minY+(maxY-minY)/2;
//        painter.drawLine(minX,centerY,maxX,centerY);
//        painter.drawLine(centerX,minY,centerX,maxY);
//    //    painter.drawRect(minX,minY,maxX-minX,maxY-minY);
//        drawSubtree(painter, currNode.childrenId[QUADRANT::RU],
//                maxX,
//                maxY,
//                minX+(maxX-minX)/2,
//                minY+(maxY-minY)/2);
//        drawSubtree(painter, currNode.childrenId[QUADRANT::RB],
//                maxX,
//                maxY-(maxY-minY)/2,
//                minX+(maxX-minX)/2,
//                minY);
//        drawSubtree(painter, currNode.childrenId[QUADRANT::LU],
//                maxX-(maxX-minX)/2,
//                maxY,
//                minX,
//                minY+(maxY-minY)/2);
//        drawSubtree(painter, currNode.childrenId[QUADRANT::LB],
//                maxX-(maxX-minX)/2,
//                maxY-(maxY-minY)/2,
//                minX,
//                minY);
//    }
//}

//void QTreeVisualisationWidget::sendPointsToQTree()
//{
//    vector<QuadTreeElement> treeElements(points.size());
//    for(int i=0;i<points.size();i++){
//        treeElements.at(i) = QuadTreeElement(Quad(points.at(i).x(),points.at(i).y(),quadSide,quadSide));
//    }
//    quadTree.setNewPoints(treeElements,800,800,0,0);
//}
