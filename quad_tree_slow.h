#ifndef QUADTREESLOW_H
#define QUADTREESLOW_H
#include <quad_tree.h>
template <class T>
class QuadTreeSlow: public QuadTree<T>{

    typedef unique_ptr<QuadTreeSlowNode<T>> NODE_U_PTR;
    typedef vector<shared_ptr<QuadTreeElement<T>>> ELEMENTS_S_PRT;
    typedef shared_ptr<QuadTreeElement<T>> ELEMENT_S_PRT;

public:
    QuadTreeSlow(){std::cout<<"Quad tree made"<<std::endl;}
    QuadTreeSlow(const ELEMENTS_S_PRT &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6){
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual void setElements(const ELEMENTS_S_PRT &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6) override{
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual ELEMENTS_S_PRT getElementsThatOverlap(const AABB<T> &aabb) override{
        ELEMENTS_S_PRT result;
        //TODO
        return result;
    }
    virtual vector<tuple<ELEMENT_S_PRT,ELEMENT_S_PRT>> getAllOverlappingElementsTuples() override{
        vector<tuple<ELEMENT_S_PRT,ELEMENT_S_PRT>> overlappingTuples;
        getAllOverlappingElementsTuplesRecursively(overlappingTuples,rootNode);
        return overlappingTuples;
    }
    virtual void reset() override{
        this->quadTreeElementsPtrs.clear();
        rootNode.reset();
    }
    virtual NODE_U_PTR &getRootNode() override{
        return rootNode;
    }

protected:
    virtual void buildTree(const ELEMENTS_S_PRT &inputElementsPtrs, const AABB<T> &boundingBox, int depth, int nodeCapacity){
        this->reset();
        this->quadTreeElementsPtrs = inputElementsPtrs;
        rootNode = makeSubtree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    NODE_U_PTR makeSubtree(const ELEMENTS_S_PRT &elementsPtrs, const AABB<T> &boundingBox, int levelRemaining, int nodeCapacity){
        if(elementsPtrs.size() == 0) return unique_ptr<QuadTreeSlowNode<T>>(nullptr);
        else{
            NODE_U_PTR currentRootNode(new QuadTreeSlowNode<T>);
            currentRootNode->boundingBox = boundingBox;
            if(elementsPtrs.size() <= nodeCapacity || levelRemaining <= 0){
                currentRootNode->quadTreeElementsPtrs = elementsPtrs;
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
    inline array<ELEMENTS_S_PRT,4> splitElementsPtrsByQuadrant(const ELEMENTS_S_PRT &elementsPtrs,  const array<AABB<T>,4> &boundingBoxes){
        array<ELEMENTS_S_PRT,4> elementsPtrsByQuadrant;
        for(auto element: elementsPtrs){
            for(int i=0;i<4;i++){
                if(element->aabb.doesOverlap(boundingBoxes.at(i))){
                    elementsPtrsByQuadrant.at(i).push_back(element);
                }
            }
        }
        return elementsPtrsByQuadrant;
    }
    void getAllOverlappingElementsTuplesRecursively(vector<tuple<ELEMENT_S_PRT,ELEMENT_S_PRT>> &tuples, const NODE_U_PTR &node){
        auto &elements = node->quadTreeElementsPtrs;
        if(node == nullptr){
            return;
        }
        else if(elements.size()>1){
            for(int i=0;i<elements.size();i++){
                for(int j=i+1;j<elements.size();j++){
                    if(elements.at(i)->doesOverlap(elements.at(j)->aabb)){
                        tuples.push_back(tuple<ELEMENT_S_PRT,ELEMENT_S_PRT>(elements.at(i),elements.at(j)));
                    }
                }
            }
        }
        else{
            for(auto &child: node->children){
                getAllOverlappingElementsTuplesRecursively(tuples,child);
            }
        }
    }

    NODE_U_PTR rootNode;
};

#endif // QUADTREESLOW_H
