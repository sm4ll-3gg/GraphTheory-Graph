#include "graph.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Graph graph("matrix");

    graph.find_length_of_way(1,2);
    graph.print();

    graph.evacuation(1);
    graph.gathering(1);

//    graph.print_all_ways(1,4);
//    qDebug() << "";
//    graph.print_all_ways(4,1);

    qDebug() << "\nThe End!";
    //return a.exec();
}
