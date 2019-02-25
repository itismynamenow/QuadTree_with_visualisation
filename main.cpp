#include <QApplication>
#include <QWidget>

#include "main_window.h"
#include "quad_tree_benchmark.h"


using namespace std;

typedef double NUM;

int main(int argc, char *argv[])
{
    bool benchmark = false;
    if(benchmark){
        QuadTree<NUM> *quadTree;
        quadTree = new QuadTreeModerate<NUM>();
                QuadTreeBenchmark<NUM>().testQuadTree(
                            quadTree,
                            3,
                            900,
                            {
                                QUAD_TREE_BENCHMARK_TYPE::SET_ELEMENTS,
                                QUAD_TREE_BENCHMARK_TYPE::GET_ALL_OVERLAPPING_TUPLES,
                                QUAD_TREE_BENCHMARK_TYPE::GET_OVERLAPPING_ELEMENTS},
                            10,
                            10,
                            AABB<NUM>(0,0,333,333)
                        );
        delete quadTree;
        quadTree = new QuadTreeFast<NUM>();
                QuadTreeBenchmark<NUM>().testQuadTree(
                            quadTree,
                            3,
                            900,
                            {
                                QUAD_TREE_BENCHMARK_TYPE::SET_ELEMENTS,
                                QUAD_TREE_BENCHMARK_TYPE::GET_ALL_OVERLAPPING_TUPLES,
                                QUAD_TREE_BENCHMARK_TYPE::GET_OVERLAPPING_ELEMENTS},
                            10,
                            10,
                            AABB<NUM>(0,0,333,333)
                        );
    }else{
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}
