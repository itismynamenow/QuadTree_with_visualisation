//#include "mainwindow.h"

//MainWindow::MainWindow()
//{
//    lineEdit.setValidator(&inputValidator);

//    mainLayout.addWidget(&qWidget,0,0);
//    inputPanelLayout.addWidget(&clearButton);
//    inputPanelLayout.addWidget(&randomButton);
//    inputPanelLayout.addWidget(&lineEdit);
//    mainLayout.addItem(&inputPanelLayout,1,0);
//    setLayout(&mainLayout);
//    connect(&clearButton, SIGNAL(clicked()), &qWidget, SLOT(reset()));
//    connect(&randomButton, SIGNAL(clicked()), this, SLOT(setRandomData()));
//    srand( time( 0 ) );
//}

//void MainWindow::setRandomData()
//{
//    vector<QPoint> points;
//    for(int i=0;i<lineEdit.text().toInt();i++){
//        QPoint point(rand()%800,rand()%800);
//        points.push_back(point);
//    }
//    qWidget.setPoints(points);
//}

