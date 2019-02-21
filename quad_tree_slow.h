#ifndef QUADTREESLOW_H
#define QUADTREESLOW_H
#include <quad_tree.h>


template <class T>
struct QuadTreeSlowNode: public QuadTreeNode<T>{

    typedef typename QuadTreeNode<T>::NODE_PTR NODE_PTR;
    typedef typename QuadTreeNode<T>::ELEMENTS_PTR ELEMENTS_PTR;
    typedef shared_ptr<QuadTreeSlowNode<T>> NODE_SLOW_PTR;

    virtual const array<NODE_PTR,4> getChildren() const override{
        array<NODE_PTR,4> castedChildren;
        for(int i=0;i<4;i++){
            castedChildren.at(i) = children.at(i);
        }
        return castedChildren;
    }
    virtual const ELEMENTS_PTR &getElements() const override{
        return elementsPtr;
    }
    virtual const AABB<T> &getBoundingBox() const override{
        return boundingBox;
    }

    array<NODE_SLOW_PTR,4> children;
    ELEMENTS_PTR elementsPtr;
    AABB<T> boundingBox;
};

template <class T>
class QuadTreeSlow: public QuadTree<T>{

protected:
    typedef typename QuadTree<T>::ELEMENTS_PTR ELEMENTS_PTR;
    typedef typename QuadTree<T>::ELEMENT_PTR ELEMENT_PTR;
    typedef typename QuadTreeNode<T>::NODE_PTR NODE_PTR;
    typedef typename QuadTreeSlowNode<T>::NODE_SLOW_PTR NODE_SLOW_PTR;

public:
    QuadTreeSlow(){std::cout<<"Quad tree made"<<std::endl;}
    QuadTreeSlow(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6){
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual void setElements(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6) override{
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual ELEMENTS_PTR getElementsThatOverlap(const AABB<T> &aabb) override{
        ELEMENTS_PTR result;
        //TODO
        return result;
    }
    virtual vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> getAllOverlappingElementsTuples() override{
        vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> overlappingTuples;
        getAllOverlappingElementsTuplesRecursively(overlappingTuples,rootNode);
        return overlappingTuples;
    }
    virtual void reset() override{
        rootNode.reset();
    }
    virtual NODE_PTR getRootNode() override{
        return rootNode;
    }

protected:
    virtual void buildTree(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth, int nodeCapacity){
        this->reset();
        rootNode = makeSubtree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    NODE_SLOW_PTR makeSubtree(const ELEMENTS_PTR &elementsPtrs, const AABB<T> &boundingBox, int levelRemaining, int nodeCapacity){
        if(elementsPtrs.size() == 0) return unique_ptr<QuadTreeSlowNode<T>>(nullptr);
        else{
            NODE_SLOW_PTR currentRootNode(new QuadTreeSlowNode<T>);
            currentRootNode->boundingBox = boundingBox;
            if(elementsPtrs.size() <= nodeCapacity || levelRemaining <= 0){
                currentRootNode->elementsPtr = elementsPtrs;
            }else{
                const array<AABB<T>,4> boundingBoxes = boundingBox.split();
                auto pointsByQuadrant = splitElementsPtrsByQuadrant(elementsPtrs,boundingBoxes);
                for(int i=0;i<4;i++){
                    currentRootNode->children.at(i) = makeSubtree(pointsByQuadrant.at(i),boundingBoxes.at(i),levelRemaining-1,nodeCapacity);
                }
            }
            return currentRootNode;
        }
    }
    inline array<ELEMENTS_PTR,4> splitElementsPtrsByQuadrant(const ELEMENTS_PTR &elementsPtrs,  const array<AABB<T>,4> &boundingBoxes){
        array<ELEMENTS_PTR,4> elementsPtrsByQuadrant;
        for(auto element: elementsPtrs){
            for(int i=0;i<4;i++){
                if(element->aabb.doesOverlap(boundingBoxes.at(i))){
                    elementsPtrsByQuadrant.at(i).push_back(element);
                }
            }
        }
        return elementsPtrsByQuadrant;
    }
    void getAllOverlappingElementsTuplesRecursively(vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> &tuples, const NODE_SLOW_PTR &node){
        auto &elements = node->elementsPtr;
        if(node == nullptr){
            return;
        }
        else if(elements.size()>1){
            for(int i=0;i<elements.size();i++){
                for(int j=i+1;j<elements.size();j++){
                    if(elements.at(i)->doesOverlap(elements.at(j)->aabb)){
                        tuples.push_back(tuple<ELEMENT_PTR,ELEMENT_PTR>(elements.at(i),elements.at(j)));
                    }
                }
            }
        }
        else{
            for(auto child: node->children){
                getAllOverlappingElementsTuplesRecursively(tuples,child);
            }
        }
    }

    shared_ptr<QuadTreeSlowNode<T>> rootNode;
};

#endif // QUADTREESLOW_H
