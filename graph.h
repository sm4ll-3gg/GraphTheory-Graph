#ifndef GRAPH_H
#define GRAPH_H

#include "vertex.h"
#include "parser.h"

#include <queue>
#include <QString>
#include <QMap>
#include <QSet>
#include <QQueue>

class Parser;

class Graph
{
public:
    Graph();
    Graph(const int& size); // Количество вершин
    Graph(const QString& address); // Создание графа из файла
    Graph(const Graph& other);
    ~Graph();

    size_t          size();

    Vertex*         get_vertex(int count);
    void            remove_vertex(int count);

    void            print_all_ways(int from, int to);

    bool            find_length_of_way(int from, int to); // номера вершин
    void            evacuation(int from);
    void            gathering(int target);

    void            print();

private: //methods
    struct Compare
    {
        bool operator()(const QPair<int, int>& p1, const QPair<int, int>& p2) {
            return p1.second > p2.second;
        }
    };

    void            _print_all_ways(Vertex* now, QList<int>& way, QSet<int>& used, int to);

    void            _find_length_of_way(QMap<int, int>& graph, int length, int index, int to, int count, bool& isOk);

    void            reset_color();

    void            _evacuation(Vertex* now, int target, int from);
    void            _gathering(Vertex* now, int target, int from);
private:
    // key - номер вершины
    QList<Vertex*>   _vertices; // Все вершины графа и связи между ними
};




#endif // GRAPH_H
