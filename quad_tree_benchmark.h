#ifndef QUAD_TREE_BENCHMARK_H
#define QUAD_TREE_BENCHMARK_H
#include <ctime>
#include <chrono>

#include "quad_tree.h"
#include "quad_tree_slow.h"
#include "quad_tree_moderate.h"

template <class T>
class QuadTreeDataGenerator{
public:
    typedef typename QuadTree<T>::ELEMENTS_PTR ELEMENTS_PTR;
    typedef typename QuadTree<T>::ELEMENT_PTR ELEMENT_PTR;

public:
    QuadTreeDataGenerator(){
        srand(time(0));
    }
    ELEMENTS_PTR makeElements(int numberOfElements,
                              const AABB<T> &boundingBox = AABB<T>(0,0,799,799),
                              T minSize = 10,
                              T maxSize = 50)
    {
        ELEMENTS_PTR elements(numberOfElements);
        for(int i=0;i<numberOfElements;i++){
            T boundingBoxWidth = boundingBox.xMax - boundingBox.xMin;
            T boundingBoxHeight = boundingBox.yMax - boundingBox.yMin;
            int x = boundingBox.xMin + rand()%boundingBoxWidth;
            int y = boundingBox.yMin + rand()%boundingBoxHeight;
            elements.at(i) = std::make_shared<QuadTreeElement<T>>(QuadTreeElement<T>(AABB<T>(x,y,x+boundingBoxWidth,y+boundingBoxHeight)));
        }
        return  elements;
    }
};

template <class T>
class QuadTreeBenchmark{
public:
    typedef typename QuadTree<T>::ELEMENTS_PTR ELEMENTS_PTR;
    typedef typename QuadTree<T>::ELEMENT_PTR ELEMENT_PTR;

public:
    void testQuadTree(QuadTree<T>* quadTree,
                      int numberOfTests,
                      int numberOfElements,
                      int treeDepth = 6,
                      int maxElementsPerBox = 6,
                      const AABB<T> &boundingBox = AABB<T>(0,0,799,799),
                      T minSize = 10,
                      T maxSize = 50 ) const
    {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        for(int i=0;i<numberOfTests;i++){
            ELEMENTS_PTR elements = QuadTreeDataGenerator<T>().makeElements(
                        numberOfElements,
                        boundingBox,
                        minSize,
                        maxSize);
            quadTree->setElements(elements,boundingBox, treeDepth, maxElementsPerBox);
//            volatile int overlappingElementsNum = quadTree->getAllOverlappingElements().size();
            volatile int overlappingElementTuplesNum = quadTree->getAllOverlappingElementTuples().size();
        }
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

        std::string typeName = typeid(*quadTree).name();
        std::cout<<"Performing "<<numberOfTests<<" test on '"<<typeName<<"' with "<<numberOfElements<<"elements took "<<duration<<" miliseconds"<<std::endl;
        std::cout<<"Average: "<<duration/(double) numberOfTests<<" miliseconds per single test"<<std::endl;
    }
};

#endif // QUAD_TREE_BENCHMARK_H
