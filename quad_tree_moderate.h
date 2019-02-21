#ifndef QUAD_TREE_MODERATE_H
#define QUAD_TREE_MODERATE_H
#include <quad_tree.h>
#include <unordered_map>
template <class T>
class QuadTreeModerate;

template <class T>
struct QuadTreeModerateNode: public QuadTreeNode<T>{
    typedef typename QuadTreeNode<T>::NODE_PTR NODE_PTR;
    typedef typename QuadTreeNode<T>::ELEMENTS_PTR ELEMENTS_PTR;

    virtual const array<NODE_PTR,4> getChildren() const override{

    }
    virtual const ELEMENTS_PTR &getElements() const override{

    }
    virtual const AABB<T> &getBoundingBox() const override{

    }
    bool isLeaf() const{
        return  childrenId[0]==-1 && childrenId[1]==-1 &&
                childrenId[2]==-1 && childrenId[3]==-1;
    }
    array<int,4> childrenId;
    shared_ptr<QuadTreeModerate<T>> quadTree;
};


template <class T>
class QuadTreeModerate: public QuadTree<T>{

public:
    typedef vector<shared_ptr<QuadTreeElement<T>>> ELEMENTS_PTR;
    typedef shared_ptr<QuadTreeElement<T>> ELEMENT_PTR;
    typedef std::unordered_map<int,ELEMENTS_PTR> MAP;

public:
    QuadTreeModerate(){}
    QuadTreeModerate(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6){
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual ~QuadTreeModerate(){}
    virtual void setElements(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6){
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual ELEMENTS_PTR getElementsThatOverlap(const AABB<T> &aabb) const override{}
    virtual ELEMENTS_PTR getAllOverlappingElements() const override{}
    virtual vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> getAllOverlappingElementTuples() const override{}
    virtual const shared_ptr<QuadTreeNode<T>> getRootNode() const override{}
    virtual void reset(){
        nodes.clear();
        elementsPtrs.clear();
        nodeIdToElementId.clear();
    }

protected:
    virtual void buildTree(const ELEMENTS_PTR &inputElementsPtrs,
                           const AABB<T> &boundingBox,
                           int depth,
                           int nodeCapacity){
        reset();
        elementsPtrs = inputElementsPtrs;
        rootId = makeSubtree(elementsPtrs,boundingBox,depth,nodeCapacity);
    }

    int makeSubtree(const ELEMENTS_PTR &elementsPtrs,
                    const AABB<T> &boundingBox,
                    int levelRemaining,
                    int nodeCapacity)
    {
        int rootId = nodes.size();
        nodes.push_back(QuadTreeModerateNode<T>());
        if(elementsPtrs.size() <= nodeCapacity || levelRemaining == 0){
            nodeIdToElementId[rootId] = elementsPtrs;
        }else{
            const array<AABB<T>,4> boundingBoxes = boundingBox.split();
            auto pointsByQuadrant = splitElementsPtrsByQuadrant(elementsPtrs,boundingBoxes);
            for(int i=0;i<4;i++){
                int childId = makeSubtree(pointsByQuadrant.at(i),boundingBoxes.at(i),levelRemaining-1,nodeCapacity);
                nodes.at(rootId).childrenId.at(i) = childId;
            }
        }
        return rootId;
    }
    array<ELEMENTS_PTR,4> splitElementsPtrsByQuadrant(const ELEMENTS_PTR &elementsPtrs,  const array<AABB<T>,4> &boundingBoxes) const{
            array<ELEMENTS_PTR,4> elementsPtrsByQuadrant;
            for(auto const &element: elementsPtrs){
                for(int i=0;i<4;i++){
                    if(element->aabb.doesOverlap(boundingBoxes.at(i))){
                        elementsPtrsByQuadrant.at(i).push_back(element);
                    }
                }
            }
            return elementsPtrsByQuadrant;
    }

    vector<QuadTreeModerateNode<T>> nodes;
    ELEMENTS_PTR elementsPtrs;
    MAP nodeIdToElementId;
    int rootId;


};

#endif // QUAD_TREE_MODERATE_H
