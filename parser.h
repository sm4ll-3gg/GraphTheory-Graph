#ifndef PARSER_H
#define PARSER_H

#include "graph.h"

#include <QString>
#include <QMap>

class Graph;

class Parser
{
    enum Condition {NONE            = 0,
                    VERTICES        = 1,  //Список всех вершин графа
                    ADJACENCY       = 2,  //Две конкретные смежные вершины
                    ADJACENCY_LIST  = 3,  //Список всех смежных вершин
                    ERROR           = 4};

    Condition   condition;
    QString     list; // список смежности
public:
    Parser();
    Parser(const QString& _list);

    QList<Vertex*>      create_graph();

private:
    void                parse_list(QList<Vertex*>& graph);
    void                change_condition(const QChar& c);

};

#endif // PARSER_H
