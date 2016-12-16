#include "graph.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>

Graph::Graph() {}

Graph::Graph(const int& size)
{
    if(size <= 0)
    {
        qDebug() << "Неверный размер в конструкторе"; // throw
        return;
    }

    for(int i = 0; i < size; ++i)
    {
        int size = _vertices.size();
        _vertices.insert(size, new Vertex());
    }
}

Graph::Graph(const QString& address)
{
    QFile file(address);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen())
    {
        qDebug() << "Неверный адрес файла с матирцей"; //throw
        return;
    }

    QString s = file.readAll();

    Parser parser(s);
    _vertices = parser.create_graph();
}

Graph::Graph(const Graph& other)
{
    _vertices = other._vertices;
}

Graph::~Graph()
{
    for(auto it = _vertices.begin(); it != _vertices.end(); ++it)
    {
        delete *it;
    }
}

size_t Graph::size()
{
    return _vertices.size();
}

Vertex* Graph::get_vertex(int count)
{
    for(auto it = _vertices.begin(); it != _vertices.end(); ++it)
    {
        if((*it)->count() == count)
        {
            return *it;
        }
    }
}

void Graph::print_all_ways(int from, int to)
{
    Vertex* _from = get_vertex(from);
    QQueue<int> way;
    QSet<int> used;

    _print_all_ways(_from, way,used, to);
}

void Graph::_print_all_ways(Vertex* now, QList<int>& way, QSet<int>& used, int to)
{
    if(now->count() == to)
    {
        QString _way;
        for(auto it = way.begin(); it != way.end(); ++it)
        {
            QString s;
            s.setNum(*it);
            _way += s;
        }
        qDebug() << _way;
    }
    else
    {
        for(auto it = now->ribs().begin(); it != now->ribs().end(); ++it)
        {
            QPair<int,int> rib = *it;
            if(!used.contains(rib.first))
            {
                used.insert(rib.first);
                way.push_back(rib.first);

                _print_all_ways(get_vertex(rib.first), way, used, to);

                used.erase( used.find(rib.first) );
                way.pop_back();
            }
        }
    }
}


bool Graph::find_length_of_way(int from, int to)
{
    QMap<int, int> graph; // key - номер вершины, value - на какой итерации он просмотрена

    for(auto it = _vertices.begin(); it != _vertices.end(); ++it)
    {
        graph.insert((*it)->count(), -1);
    }

    graph.find(from).value() = 0;

    bool isOk = false;
    _find_length_of_way(graph, 0, from, to, 1, isOk);

    if(!isOk)
    {
        qDebug() << "\nПуть между этими двумя вершинами не существует.";
    }

    return isOk;
}

void Graph::_find_length_of_way(QMap<int, int>& graph, int length, int index, int to, int count, bool& isOk)
{
    if(index == to)
    {
        qDebug() << "\nПуть найден, его длина = " << length;
        graph.find(index).value() = count;
        isOk = true;
    }

    Vertex* now = get_vertex(index);
    std::priority_queue<QPair<int,int>, std::vector<QPair<int,int>>, Compare> q;
    for(auto it = now->ribs().begin(); it != now->ribs().end(); ++it)
    {
        QPair<int,int> rib = *it;
        auto _it = graph.find(rib.first);
        if(_it.value() == -1)
        {
            _it.value() = count;
            q.push(rib);
        }
    }

    while(!q.empty())
    {
        QPair<int, int> rib = q.top();
        q.pop();
        _find_length_of_way(graph, length+rib.second, rib.first, to, count+1, isOk);
    }
}

void Graph::reset_color()
{
    for(auto it = _vertices.begin(); it != _vertices.end(); ++it)
    {
        (*it)->color() = Vertex::Color::WHITE;
    }
}


void Graph::evacuation(int index)
{
    int count = 0;

    Vertex* now = get_vertex(index);

    while(now->data() > 0)
    {
        _evacuation(now, index, index);
        count++;
    }

    qDebug() << "\nЭвакуация произведена за " << count << " ходов.";
    print();
}

void Graph::_evacuation(Vertex* now, int target, int from)
{
    for(auto it = now->ribs().begin(); it != now->ribs().end(); ++it)
    {
        QPair<int,int> rib = *it;
        if(rib.first != from && rib.first != target)
        {
            Vertex* to = get_vertex(rib.first);

            if(to->data() < now->data() || now->count() == target)
            {
                int size = now->data() > rib.second ? rib.second : now->data();

                to->data() += size;
                now->data() -= size;
                _evacuation(to, target, now->count());
            }
            else
            {
                _evacuation(to, target, now->count());
                continue;
            }
        }
    }
}

void Graph::gathering(int _target)
{
    int count = 0;

    Vertex* target = get_vertex(_target);

    QList<Vertex*> obtainableVertices;
    for(auto it = _vertices.begin(); it != _vertices.end(); ++it)
    {
        Vertex* v = *it;
        if(v->count() != _target)
            if(find_length_of_way(v->count(), _target))
            {
                obtainableVertices.push_back(v);
            }
    }

    while(true)
    {
        bool b = true;
        for(auto it = obtainableVertices.begin(); it != obtainableVertices.end(); ++it)
        {
            if((*it)->data() != 0)
            {
                b = false;
                break;
            }
        }

        if(b) break;
        _gathering(target, _target, _target);
        reset_color();
    }

    _gathering(target, _target, _target);

    qDebug() << "Сбор в вершине " << _target << " произведен.";
    print();
}

void Graph::_gathering(Vertex* now, int target, int from)
{
    now->color() = Vertex::Color::GREY;

    for(auto it = now->ribs().begin(); it != now->ribs().end(); ++it)
    {
        QPair<int,int> rib = *it;
        Vertex* adj = get_vertex(rib.first);

        if(adj->color() == Vertex::Color::WHITE)
        {
            _gathering(adj, target, now->count());
        }
        else if(adj->color() == Vertex::Color::GREY && now->count() != target)
        {
            int size = now->data() > rib.second ? rib.second : now->data();

            adj->data() += size;
            now->data() -= size;
        }
    }

    now->color() = Vertex::Color::BLACK;
}

void Graph::print()
{
    qDebug() << "\n";
    for(auto it = _vertices.begin(); it != _vertices.end(); ++it)
    {
        Vertex* vertex = *it;
        qDebug() << "Вершина №" << vertex->count() << " имеет значение " << vertex->data() << " и инцидентна вершинам:";

        for(auto _it = vertex->ribs().begin(); _it != vertex->ribs().end(); ++_it)
        {
            QPair<int,int> rib = *_it;

            qDebug() << "    " << rib.first << " . Длина ребра = " << rib.second;
        }
    }
}

