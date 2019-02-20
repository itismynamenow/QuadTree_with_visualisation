#include <QApplication>
#include <QWidget>

//#include "qtreevisualisationwidget.h"
//#include "quadtree.h"
#include "mainwindow.h"

/*
 *  QuadTree needs:
 *      Nodes
 *      Input type (an interface including AABB?)
 *      Some way to return info
 *      Type for AABB axis alined bouning box -> created AABB template class
 *      Enum for 4 quadrants -> dones
 *  Things to decide:
 *      Precision used to store AABB coordinates -> used template
 *      Does tree hold copy of values or just pointers to values that exist separatly
 *          - Keeping copy doesn't allow to return info about overlaping objects without using indices
 *          - Keeping ordinary pointers is not safe
 *          - Shared pointers are a bit slower -> done
 *      Is QuadTree class just an interface
 *          - If it is an interface then there is no need to create new project each time some I make new version of tree.
 *             However how often new version has same API? Well, one day I will learn to make proper interfaces that stay same
 * `            -> made QuadTree as an abstract class
 *      Where to create and hold data structure responsible for quad tree elements
 *          - Inside visualisation widget -> feels wrong cuz of strong coupling. However
 *              this widget is algorithm specific and therefore is not reusable.
 *          - Make small stuct responsible for this and place it into this header ->
 *              Looks as most resonable thing to do right now
 *      How quad tree and element holder should related to each other
 *          - They can be just 2 separate objects -> ugly cuz what if it is needed not only perform quad tree related operations
 *              but also something like nearest neighbor etc. -> Then there would be multiple objects interacting with element holder ->
 *              Considering that element holder is specific to element type all objects can be moved in it
 *          - Quad tree can be member of element holder -> less ugly
 *          - Quad tree be turned into data structure that actually stores all data on its own ->
 *              guess it depends on goald of quad tree. Quad tree needs to work with arbitrary object
 *              that inherit QuadTreeElement so there is no way to store this objects inside
 *      Purpose of quad tree
 *          - Finding all overlaping objects
 *          - Returning all objects in range
 *          - Doing all above on arbitrary objects that inherit QuadTreeElement
 *      Where quad tree and element holder should be located
 */
using namespace std;

int main(int argc, char *argv[])
{
//    auto treeElementPtr0 = std::make_shared<QuadTreeElement<float>>(QuadTreeElement<float>(AABB<float>(755,755,788,788)));
//    auto treeElementPtr1 = std::make_shared<QuadTreeElement<float>>(QuadTreeElement<float>(AABB<float>(1,1,2,3)));
//    auto treeElementPtr2 = std::make_shared<QuadTreeElement<float>>(QuadTreeElement<float>(AABB<float>(1,0,2,4)));
//    auto treeElementPtr3 = std::make_shared<QuadTreeElement<float>>(QuadTreeElement<float>(AABB<float>(5,5,2,2)));

//    vector<shared_ptr<QuadTreeElement<float>>> treeElementsVector{treeElementPtr0,treeElementPtr1,treeElementPtr2,treeElementPtr3};

//    QuadTreeSlow<float> quadTree(treeElementsVector,AABB<float>(0,0,799,799),6,2);

//    quadTree.printElementsQuads();

//    MyCustomElementsHolder holder;
//    holder.addElement(AABB<int>(755,755,788,788),Qt::yellow);
//    holder.addElement(AABB<int>(756,756,789,789),Qt::yellow);
//    holder.addElement(AABB<int>(155,155,788,788),Qt::yellow);
//    holder.addElement(AABB<int>(122,122,123,123),Qt::yellow);

//    auto overlappingObject = holder.getOverlappingObjects();
//    for(auto &object: overlappingObject){
//        object->aabb.print();
//    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}
