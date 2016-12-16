#include "parser.h"

#include <QFile>
#include <QDebug>

Parser::Parser()
    :condition(NONE), list("") {}

Parser::Parser(const QString& _list)
{
    condition = NONE;
    list = _list;
}

QList<Vertex*> Parser::create_graph()
{
    QList<Vertex*> vertices;
    parse_list(vertices);

    return vertices;
}

void Parser::parse_list(QList<Vertex*>& vertices)
{
    QString value = "";

    int from = 0;

    auto it = list.begin();
    while(it != list.end())
    {
        if(condition == ERROR)
            return;

        if(condition == VERTICES)
        {
            if(*it == ',' || *it == ']')
            {
                int number = value.toInt();

                QTextStream in(stdin);

                qDebug() << "Введите значение " << number << "-й вершины";
                int data;
                in >> data;

                Vertex* vertex = new Vertex(number, data);
                vertices.insert(number, vertex);

                value = "";
            }
            else
                value += *it;
        }
        else if(condition == ADJACENCY)
        {
            if(*it == ',')
            {
                from = value.toInt();
                value = "";
            }
            else if(*it == ')')
            {
                int to = value.toInt(); // Номер вершины, инцидентной вершине с индеком index

                QTextStream in(stdin);

                qDebug() << "Введите длину ребра между вершинами " << from << " и " << to;
                int width;
                in >> width;

                for(auto it = vertices.begin(); it != vertices.end(); ++it)
                {
                    if((*it)->count() == from)
                        (*it)->add_rib(to, width);
                }
                from = -1;
                value = "";
            }
            else
                value += *it;
        }

        change_condition(*it);

        ++it;
    }

    QTextStream str(stdout);
}

void Parser::change_condition(const QChar& c)
{
    if(c.isNumber() || c == ',' || c == ' ')
        return;
    else if(c == '[' && condition == NONE)
        condition = VERTICES;
    else if(c == ']' && condition == VERTICES)
        condition = NONE;
    else if(c == '{' && condition == NONE)
        condition = ADJACENCY_LIST;
    else if(c == '}' && condition == ADJACENCY_LIST)
        condition = NONE;
    else if(c == '(' && condition == ADJACENCY_LIST)
        condition = ADJACENCY;
    else if(c == ')' && condition == ADJACENCY)
        condition = ADJACENCY_LIST;
    else
    {
        qDebug() << "Некорректное описание графа.";
        condition = ERROR;
    }

}
