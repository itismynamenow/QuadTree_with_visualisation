#ifndef QUAD_TREE_FAST_H
#define QUAD_TREE_FAST_H
#include "quad_tree.h"
#include <unordered_map>
#include <set>
#include <unordered_set>
template <class T>
class QuadTreeFast;
template <class T>
class QuadTreeFastVisualionHelper;

template <class T>
struct QuadTreeFastNode{
    QuadTreeFastNode(){
        for(int i=0;i<4;i++){
            childrenId.at(i) = -1;
        }
    }
    bool isLeaf() const{
        return  childrenId[0]==-1 && childrenId[1]==-1 &&
                childrenId[2]==-1 && childrenId[3]==-1;
    }
    array<int,4> childrenId;
};
/*
 * The main difference from moderate quad tree is the fact that elements are stored not only in leafs
 * but in ordinary node if given element overlaps node's bounding box completly
 */
template <class T>
class QuadTreeFast{

public:

    typedef typename QuadTreeElement<T>::ELEMENT_PTR ELEMENT_PTR;
    typedef vector<ELEMENT_PTR> ELEMENTS_PTR;
    typedef std::unordered_map<int,vector<int>> MAP;
    typedef std::unordered_set<int> SET;

    friend class QuadTreeFastVisualionHelper<T>;

public:
    QuadTreeFast(){}
    virtual ~QuadTreeFast(){}
    virtual void setElements(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6)=0;
    virtual ELEMENTS_PTR getElementsThatOverlap(const AABB<T> &aabb) const=0;
    virtual ELEMENTS_PTR getAllOverlappingElements() const=0;
    virtual vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> getAllOverlappingElementTuples() const=0;
    virtual void reset()=0;
    virtual const QuadTreeVisualionHelper<T> *getVisualisationHelper() const=0;

protected:



    vector<QuadTreeFastNode<T>> nodes;
    vector<AABB<T>> boundingBoxes;
    ELEMENTS_PTR elementsPtrs;
    MAP nodeIdToElementId;
    int rootId=-1;
    QuadTreeFastVisualionHelper<T> visualisationHelper{this};

};

template <class T>
class QuadTreeFastVisualionHelper{
public:
    QuadTreeFastVisualionHelper(QuadTreeFast<T>* quadTree): quadTree(quadTree){}
    virtual vector<AABB<T>> getNonLeafNodesBoundingBoxes() const=0;
private:
    QuadTreeFast<T>* quadTree;
};

#endif // QUAD_TREE_FAST_H
