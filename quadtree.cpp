//#include "quadtree.h"

//QuadTree::QuadTree()
//{

//}

//QuadTree::QuadTree(const vector<QuadTreeElement> &treeElements, double maxX, double maxY, double minX, double minY)
//{
//    this->maxX=maxX;
//    this->maxY=maxY;
//    this->minX=minX;
//    this->minY=minY;
//    this->makeTree(treeElements,maxX,maxY,minX,minY);
//}

//void QuadTree::setNewPoints(const vector<QuadTreeElement> &treeElements, double maxX, double maxY, double minX, double minY)
//{
//    this->maxX=maxX;
//    this->maxY=maxY;
//    this->minX=minX;
//    this->minY=minY;
//    this->makeTree(treeElements,maxX,maxY,minX,minY);
//}

//vector<int> QuadTree::getCollidingElementsIds()
//{
//    vector<int> elementsIdsVector;
//    elementsIdsVector.insert(elementsIdsVector.end(), collidingElementsIds.begin(), collidingElementsIds.end());;
//    return elementsIdsVector;
//}

//void QuadTree::clear()
//{
//    nodes.clear();
//    nodeIdToPoint.clear();
//    collidingElementsIds.clear();
//    comparisonsMade=0;
//}

//void QuadTree::makeTree(const vector<QuadTreeElement> &treeElements, double maxX, double maxY, double minX, double minY)
//{
//    if(treeElements.size() > 0){
//        this->clear();
//        vector<int> treeElementsIds(treeElements.size());
//        for(int i=0;i<treeElementsIds.size();i++){
//            treeElementsIds.at(i)=i;
//        }
//        rootId = makeSubtree(treeElements,treeElementsIds,8,maxX,maxY,minX,minY);
//        cout<<"Made "<<comparisonsMade<<" comparisons"<<endl;
//    }
//}

//int QuadTree::makeSubtree(const vector<QuadTreeElement> &treeElements, vector<int> treeElementsIds, int levelsRemaining, double maxX, double maxY, double minX, double minY)
//{
//    if(treeElements.size()==0 || treeElementsIds.size()==0) return -1;
//    else{
//        int currNodeId = this->nodes.size();
//        nodes.push_back(QuadTreeNode());

//        if(treeElementsIds.size() < 5 || levelsRemaining==0){
//            if(treeElementsIds.size()>1){
//                for(int i=0;i<treeElementsIds.size();i++){
//                    const QuadTreeElement &currTreeElement = treeElements.at(treeElementsIds.at(i));
//                    for(int j=i+1;j<treeElementsIds.size();j++){
//                        const QuadTreeElement &anotherTreeElement = treeElements.at(treeElementsIds.at(j));
//                        comparisonsMade++;
//                        if(currTreeElement.doesOverlapQuad(anotherTreeElement.quad)){
//                            collidingElementsIds.insert(treeElementsIds.at(i));
//                            collidingElementsIds.insert(treeElementsIds.at(j));
//                        }
//                    }
//                }
//            }
//        }else{
//            array<vector<int>,4> treeElementsIdsByQuadrant = splitByQuadrant(treeElements,treeElementsIds,(maxX+minX)/2,(maxY+minY)/2);
//            double middleX = minX + (maxX-minX)/2;
//            double middleY = minY + (maxY-minY)/2;
//            int RU = makeSubtree(treeElements, treeElementsIdsByQuadrant.at(QUADRANT::RU), levelsRemaining-1, maxX, maxY, middleX, middleY);
//            int RB = makeSubtree(treeElements, treeElementsIdsByQuadrant.at(QUADRANT::RB), levelsRemaining-1, maxX, middleY, middleX, minY);
//            int LU = makeSubtree(treeElements, treeElementsIdsByQuadrant.at(QUADRANT::LU), levelsRemaining-1, middleX, maxY, minX, middleY);
//            int LB = makeSubtree(treeElements, treeElementsIdsByQuadrant.at(QUADRANT::LB), levelsRemaining-1, middleX, middleY, minX, minY);
//            //Until C++17 '=' evaluation order is undefined, so commands is splitted into 2 parts
//            nodes.at(currNodeId).childrenId.at(QUADRANT::RU) = RU;
//            nodes.at(currNodeId).childrenId.at(QUADRANT::RB) = RB;
//            nodes.at(currNodeId).childrenId.at(QUADRANT::LU) = LU;
//            nodes.at(currNodeId).childrenId.at(QUADRANT::LB) = LB;
//        }
//        return currNodeId;
//    }
//}

//array<vector<int>,4> QuadTree::splitByQuadrant(const vector<QuadTreeElement> &treeElements,const vector<int> &treeElementsIds, double centerX, double centerY){
//    array<vector<int>,4> treeElementsIdsByQuadrant;
//    for(int treeElementId: treeElementsIds){
//        const QuadTreeElement &treeElement = treeElements.at(treeElementId);
//        QUADRANT quadrant;
//        int intersectionCount = 0;
//        if(treeElement.doesOverlapQuad(Quad(centerX,centerY,maxX-centerX,maxY-centerY))){
//            quadrant =QUADRANT::RU;
//            treeElementsIdsByQuadrant.at(quadrant).push_back(treeElementId);
//            intersectionCount++;
//        }
//        if(treeElement.doesOverlapQuad(Quad(centerX,minY,maxX-centerX,centerY-minY))){
//            quadrant =QUADRANT::RB;
//            treeElementsIdsByQuadrant.at(quadrant).push_back(treeElementId);
//            intersectionCount++;
//        }
//        if(treeElement.doesOverlapQuad(Quad(minX,centerY,centerX-minX,maxY-centerY))){
//            quadrant =QUADRANT::LU;
//            treeElementsIdsByQuadrant.at(quadrant).push_back(treeElementId);
//            intersectionCount++;
//        }
//        if(treeElement.doesOverlapQuad(Quad(minX,minY,centerX-minX,centerY-minY))){
//            quadrant =QUADRANT::LB;
//            treeElementsIdsByQuadrant.at(quadrant).push_back(treeElementId);
//            intersectionCount++;
//        }
//    }
//    return treeElementsIdsByQuadrant;
//}























