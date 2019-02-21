#ifndef QUAD_TREE_MODERATE_H
#define QUAD_TREE_MODERATE_H
#include <quad_tree.h>
#include <unordered_map>
#include <set>
template <class T>
class QuadTreeModerate;
template <class T>
class QuadTreeModerateVisualionHelper;

template <class T>
struct QuadTreeModerateNode{
    QuadTreeModerateNode(){
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


template <class T>
class QuadTreeModerate: public QuadTree<T>{

public:
    typedef vector<shared_ptr<QuadTreeElement<T>>> ELEMENTS_PTR;
    typedef shared_ptr<QuadTreeElement<T>> ELEMENT_PTR;
    typedef std::unordered_map<int,ELEMENTS_PTR> MAP;
    typedef bool(*ELEMENT_COMPARATOR)(const ELEMENT_PTR &x, const ELEMENT_PTR &y);
    typedef std::set<ELEMENT_PTR,ELEMENT_COMPARATOR> ELEMENT_SET;

    friend class QuadTreeModerateVisualionHelper<T>;

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
    virtual ELEMENTS_PTR getAllOverlappingElements() const override{
        ELEMENT_COMPARATOR comparator = [](const ELEMENT_PTR &x, const ELEMENT_PTR &y){ return x.get() < y.get(); };
        ELEMENT_SET elementSet(comparator);
        getAllOverlappingElementsRecursively(elementSet,rootId);
        return ELEMENTS_PTR(elementSet.begin(),elementSet.end());
    }
    virtual vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> getAllOverlappingElementTuples() const override{
        vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> overlappingTuples;
        getAllOverlappingElementTuplesRecursively(overlappingTuples,rootId);
        return overlappingTuples;
    }
    virtual void reset(){
        nodes.clear();
        elementsPtrs.clear();
        nodeIdToElementId.clear();
        boundingBoxes.clear();
        rootId=-1;
    }
    virtual const QuadTreeVisualionHelper<T> *getVisualisationHelper() const override{
        return &visualisationHelper;
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
        boundingBoxes.push_back(boundingBox);
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
    void getAllOverlappingElementTuplesRecursively(vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> &tuples, int nodeId) const{
        if(nodeId != -1){
            auto node = nodes.at(nodeId);
            if(node.isLeaf()){
                ELEMENTS_PTR elements = nodeIdToElementId.at(nodeId);
                if(elements.size()>1){
                    for(int i=0;i<elements.size();i++){
                        for(int j=i+1;j<elements.size();j++){
                            if(elements.at(i)->doesOverlap(elements.at(j)->aabb)){
                                tuples.push_back(tuple<ELEMENT_PTR,ELEMENT_PTR>(elements.at(i),elements.at(j)));
                            }
                        }
                    }
                }
            }else{
                for(int childId: node.childrenId){
                    getAllOverlappingElementTuplesRecursively(tuples,childId);
                }
            }
        }
    }
    void getAllOverlappingElementsRecursively(ELEMENT_SET &elementSet, int nodeId) const{
        if(nodeId != -1){
            auto node = nodes.at(nodeId);
            if(node.isLeaf()){
                ELEMENTS_PTR elements = nodeIdToElementId.at(nodeId);
                if(elements.size()>1){
                    for(int i=0;i<elements.size();i++){
                        for(int j=i+1;j<elements.size();j++){
                            if(elements.at(i)->doesOverlap(elements.at(j)->aabb)){
                                elementSet.insert(elements.at(i));
                                elementSet.insert(elements.at(j));
                            }
                        }
                    }
                }
            }else{
                for(int childId: node.childrenId){
                    getAllOverlappingElementsRecursively(elementSet,childId);
                }
            }
        }
    }
    vector<QuadTreeModerateNode<T>> nodes;
    vector<AABB<T>> boundingBoxes;
    ELEMENTS_PTR elementsPtrs;
    MAP nodeIdToElementId;
    int rootId=-1;
    QuadTreeModerateVisualionHelper<T> visualisationHelper{this};
};

template <class T>
class QuadTreeModerateVisualionHelper: public QuadTreeVisualionHelper<T>{
public:
    QuadTreeModerateVisualionHelper(QuadTreeModerate<T>* quadTree): quadTree(quadTree){}
    virtual vector<AABB<T>> getNonLeafNodesBoundingBoxes() const{
        vector<AABB<T>> boundingBoxes;
        getNonLeafNodesBoundingBoxesRecursivly(boundingBoxes,quadTree->rootId);
        return boundingBoxes;
    }
private:
    QuadTreeModerate<T>* quadTree;
    void getNonLeafNodesBoundingBoxesRecursivly(vector<AABB<T>> &boundingBoxes, int nodeId) const{
        if(nodeId != -1){
            auto node = quadTree->nodes.at(nodeId);
            if(!node.isLeaf()){
                boundingBoxes.push_back(quadTree->boundingBoxes.at(nodeId));
                for(int childId: node.childrenId){
                    getNonLeafNodesBoundingBoxesRecursivly(boundingBoxes,childId);
                }
            }
        }
    }
};

#endif // QUAD_TREE_MODERATE_H
