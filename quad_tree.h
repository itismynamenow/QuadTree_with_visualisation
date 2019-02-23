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
template <class T>
class QuadTree;
template <class T>
struct QuadTreeElement;
template <class T>
class QuadTreeVisualionHelper;

//Axis aligned bounding box
template <class T,
          typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
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

template<typename T> struct is_shared_ptr : std::false_type {};
template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
//Inherit this class for object to be used with quad tree
template <class T>
struct QuadTreeElement{

//    typedef shared_ptr<QuadTreeElement<T>> ELEMENT_PTR;
    typedef QuadTreeElement<T>* ELEMENT_PTR;

    QuadTreeElement(){countDefaultConstructor;}
    QuadTreeElement(const AABB<T> &aabb):aabb(aabb){countConstructor++;}
    QuadTreeElement(const QuadTreeElement &another){
        this->aabb = another.aabb;
        countCopyConstructor++;
    }
    QuadTreeElement(QuadTreeElement &&another){
        this->aabb = another.aabb;
        countMoveConstructor++;
    }
    bool doesOverlap(const AABB<T> &another)const{
        return aabb.doesOverlap(another);
    }
    bool doesOverlap(const  QuadTreeElement<T> &another)const{
        return aabb.doesOverlap(another.aabb);
    }
    static ELEMENT_PTR makeQuadTreeElement(){
        return ELEMENT_PTR();
    }
//    template<typename T_ = ELEMENT_PTR, std::enable_if<is_shared_ptr<T_>::value>* = nullptr>
//    ELEMENT_PTR func()
//    {
//        std::cout << "shared ptr" << std::endl;
//    }
//    template<typename T_ = ELEMENT_PTR, std::enable_if<!is_shared_ptr<T_>::value>* = nullptr>
//    ELEMENT_PTR func()
//    {
//        std::cout << "not shared ptr" << std::endl;
//    }
//    template<typename U = T, typename std::enable_if<std::is_same<U, int>::value>::type * = NULL>
//    int MyFunction()
//    {
//      std::cout << "T is int." << std::endl;
//      return 1;
//    }

//    template<typename U = T, typename std::enable_if<std::is_same<U, float>::value>::type * = NULL>
//    int MyFunction()
//    {
//      std::cout << "T is not int." << std::endl;
//      return 2;
//    }
//    template <class V=T, typename std::enable_if<std::is_same<V, int>::value>::type* = nullptr>
//    int MyFunction()
//    {
//      std::cout << "T is int." << std::endl;
//      return 1;
//    }

//    template <class V=T, typename std::enable_if<std::is_same<V, float>::value>::type* = nullptr>
//    float MyFunction()
//    {
//      std::cout << "T is not int." << std::endl;
//      return 2;
//    }
//    template <class V=ELEMENT_PTR1, typename std::enable_if<is_shared_ptr<V>::value>::type* = nullptr>
//    int MyFunction()
//    {
//      std::cout << "T is shared pointer" << std::endl;
//      return 1;
//    }

//    template <class V=ELEMENT_PTR1, typename std::enable_if<!is_shared_ptr<V>::value>::type* = nullptr>
//    float MyFunction()
//    {
//      std::cout << "T is not shared pointer" << std::endl;
//      return 2;
//    }

    template <class V=ELEMENT_PTR, typename std::enable_if<is_shared_ptr<V>::value>::type* = nullptr>
    static ELEMENT_PTR makeElement(const AABB<T> &aabb)
    {
      return std::make_shared<QuadTreeElement<T>>(QuadTreeElement(aabb));
    }

    template <class V=ELEMENT_PTR, typename std::enable_if<!is_shared_ptr<V>::value>::type* = nullptr>
    static ELEMENT_PTR makeElement(const AABB<T> &aabb)
    {
      return new QuadTreeElement<T>(aabb);
    }

    AABB<T> aabb;
    static int countConstructor;
    static int countDefaultConstructor;
    static int countCopyConstructor;
    static int countMoveConstructor;
};
template <class T>
int QuadTreeElement<T>::countConstructor = 0;
template <class T>
int QuadTreeElement<T>::countDefaultConstructor = 0;
template <class T>
int QuadTreeElement<T>::countCopyConstructor = 0;
template <class T>
int QuadTreeElement<T>::countMoveConstructor = 0;

template <class T>
class QuadTree{

public:

    typedef typename QuadTreeElement<T>::ELEMENT_PTR ELEMENT_PTR;
    typedef vector<ELEMENT_PTR> ELEMENTS_PTR;

public:
    QuadTree(){}
    virtual ~QuadTree(){}
    virtual void setElements(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6)=0;
    virtual ELEMENTS_PTR getElementsThatOverlap(const AABB<T> &aabb) const=0;
    virtual ELEMENTS_PTR getAllOverlappingElements() const=0;
    virtual vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> getAllOverlappingElementTuples() const=0;
    virtual void reset()=0;
    virtual const QuadTreeVisualionHelper<T> *getVisualisationHelper() const=0;
};

template <class T>
class QuadTreeVisualionHelper{
public:
    virtual vector<AABB<T>> getNonLeafNodesBoundingBoxes() const=0;
};

#endif // QUADTREE_H
