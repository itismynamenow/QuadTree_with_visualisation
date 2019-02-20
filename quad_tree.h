#ifndef QUADTREE_H
#define QUADTREE_H
#include <iostream>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>
#include <tuple>
#include <ctime>
using std::vector;
using std::array;
using std::shared_ptr;
using std::unique_ptr;
using std::tuple;

enum QUADRANT{LU=0,LB=1,RU=2,RB=3,CENTER=4};

//Axis aligned bounding box
template <class T>
struct AABB{
    AABB():xMin(0),yMin(0),xMax(0),yMax(0){}
    AABB(T xMin,T yMin,T xMax,T yMax):xMin(xMin),yMin(yMin),xMax(xMax),yMax(yMax){}
    T xMin,yMin,xMax,yMax;
    bool doesOverlap(const AABB &another)const{
        bool xOverlap =
                (xMin>=another.xMin && xMin<=another.xMax) ||
                (another.xMin>=xMin && another.xMin<=xMax);
        bool yOverlap =
                (yMin>=another.yMin && yMin<=another.yMax) ||
                (another.yMin>=yMin && another.yMin<=yMax);
        return xOverlap && yOverlap;
    }
    array<AABB<T>,4> split() const {
        T xCtr = xMin+(xMax - xMin)/2;
        T yCtr = yMin+(yMax - yMin)/2;
        return array<AABB<T>,4>{
            AABB<T>(xCtr,yCtr,xMax,yMax),
            AABB<T>(xCtr,yMin,xMax,yCtr),
            AABB<T>(xMin,yCtr,xCtr,yMax),
            AABB<T>(xMin,yMin,xCtr,yCtr)
        };
    }
    void translateBy(T dx,T dy){
        xMin+=dx;
        xMax+=dx;
        yMin+=dy;
        yMax+=dy;
    }
    void print(){
        std::cout<<"xMin = "<<xMin<<" yMin = "<<yMin<<" xMax = "<<xMax<<" yMax = "<<yMax<<std::endl;
    }
};

//Inherit this class for object to be used with quad tree
template <class T>
struct QuadTreeElement{
    QuadTreeElement(){}
    QuadTreeElement(const AABB<T> &aabb):aabb(aabb){}
    bool doesOverlap(const AABB<T> &another)const{
        return aabb.doesOverlap(another);
    }
    bool doesOverlap(const  QuadTreeElement<T> &another)const{
        return aabb.doesOverlap(another.aabb);
    }

    AABB<T> aabb;
};


template <class T>
struct QuadTreeSlowNode{
    array<unique_ptr<QuadTreeSlowNode<T>>,4> children;
    vector<shared_ptr<QuadTreeElement<T>>> quadTreeElementsPtrs;
    AABB<T> boundingBox;
};

template <class T>
struct QuadTreeNode{
    virtual const array<unique_ptr<QuadTreeNode<T>>,4> &getChildren()=0;
    virtual const vector<shared_ptr<QuadTreeElement<T>>> &getElements()=0;
    virtual const AABB<T> getBoundingBox();
};

template <class T>
class QuadTree{

    typedef unique_ptr<QuadTreeSlowNode<T>> NODE_U_PTR;
    typedef vector<shared_ptr<QuadTreeElement<T>>> ELEMENTS_S_PRT;
    typedef shared_ptr<QuadTreeElement<T>> ELEMENT_S_PRT;

public:
    QuadTree(){}
    virtual ~QuadTree(){}
    virtual void setElements(const ELEMENTS_S_PRT &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6)=0;
    virtual ELEMENTS_S_PRT getElementsThatOverlap(const AABB<T> &aabb)=0;
    virtual vector<tuple<ELEMENT_S_PRT,ELEMENT_S_PRT>> getAllOverlappingElementsTuples()=0;
    virtual void reset()=0;
    virtual void printElementsQuads(){
        for(auto &quadTreeElementPtr: quadTreeElementsPtrs){
            quadTreeElementPtr->aabb.print();
        }
    }
    virtual NODE_U_PTR& getRootNode()=0;
protected:
    vector<shared_ptr<QuadTreeElement<T>>> quadTreeElementsPtrs;
};




#endif // QUADTREE_H
