#ifndef VERTEX_H
#define VERTEX_H

#include <ostream>
#include <QPair>
#include <QList>

class Vertex
{
public:
    enum Color { WHITE = 0,
                 GREY  = 1,
                 BLACK = 2};

    // first - номер вершины к которой идет ребро(дуга)
    // second - вес ребра(дуги)
    typedef QPair<int,int> Rib;

public: // methods
    Vertex();
    Vertex(const int& count, const int& data);
    ~Vertex();

    Color&              color();
    int&                data();
    int&                count();

    void                add_rib(int number, int weigth);
    const QList<Rib>&   ribs();

private:
    Color _color;

    int _count; // Номер вершины в графе
    int _data; // Значение вершины

    QList<Rib> _ribs; // Ребра от этой вершины
};



#endif // VERTEX_H
