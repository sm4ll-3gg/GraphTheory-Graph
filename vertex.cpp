#include "vertex.h"

#include <QDebug>

Vertex::Vertex()
    :_color(WHITE), _count(0), _data(0) {}

Vertex::Vertex(const int& count, const int& data)
    :_color(WHITE), _count(count), _data(data) {}

Vertex::~Vertex()
{
    _count = 0;
    _data = 0;

    _ribs.clear();
}

Vertex::Color& Vertex::color()
{
    return _color;
}

void Vertex::add_rib(int number, int weigth)
{
    if(number < 0 && weigth <= 0)
    {
        qDebug() << "Засунь нормальные значения в add_rib"; // throw
    }

    _ribs.push_back(QPair<int,int>(number, weigth));
}

int& Vertex::data()
{
    return _data;
}

int &Vertex::count()
{
    return _count;
}

const QList<Vertex::Rib>& Vertex::ribs()
{
    return _ribs;
}

