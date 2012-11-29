#include "Graph.h"

Graph::Graph()
    : last_node_id(0)
    , last_edge_id(0)
{
}

void Graph::addNode(VectorXd position)
{
    Node n;
    n.id = last_node_id;
    n.position = position;
    n.simular_threshold = 0.0;
    n.win_times = 0.0;

    nodes.push_back(n);
    ++last_node_id;
}

void Graph::addEdge(int first_node_id, int second_node_id)
{
    sort(first_node_id, second_node_id);

    Edge edge;
    edge.id = last_edge_id;
    edge.node_ids = pair<int, int>(first_node_id, second_node_id);
    edge.age = 0;

    edges.push_back(edge);
    ++last_edge_id;
}

bool Graph::hasEdge(int first_node_id, int second_node_id)
{
    sort(first_node_id, second_node_id);

    for(int i = 0; i < (int)edges.size(); ++i)
    {
        Edge e = edges.at(i);
        if(e.node_ids.first == first_node_id &&
           e.node_ids.second == second_node_id)
        {
            return true;
        }
    }

    return false;
}

bool Graph::hasEdge(int node_id)
{
    int count = getEdgeCount(node_id);
    if(count == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

Node Graph::getNode(int node_id)
{
    vector<Node>::iterator it;
    for(it = nodes.begin(); it != nodes.end(); ++it)
    {
        if(it->id == node_id)
        {
            return (*it);
        }
    }
}

vector<Node> Graph::getRealtedNodes(int node_id)
{
    vector<Edge> from = getEdgeFrom(node_id);
    vector<Edge> to = getEdgeTo(node_id);

    vector<Node> related;
    for(int i = 0; i < (int)from.size(); ++i)
    {
        Edge e = from.at(i);
        Node n = getNode(e.node_ids.second);
        related.push_back(n);
    }
    for(int i = 0; i < (int)to.size(); ++i)
    {
        Edge e = to.at(i);
        Node n = getNode(e.node_ids.first);
        related.push_back(n);
    }

    return related;
}

vector<Node> Graph::getAllNodes()
{
    return nodes;
}

int Graph::getEdgeCount(int node_id)
{
    // node_idに繋がるエッジの数
    int count = 0;
    vector<Edge>::iterator it = edges.begin();
    while(it != edges.end())
    {
        if((*it).node_ids.first == node_id ||
                (*it).node_ids.second == node_id)
        {
            ++count;
        }
        ++it;
    }

    return count;
}

Edge Graph::getEdgeFromTo(int first_id, int second_id)
{
    sort(first_id, second_id);
    vector<Edge> es = getEdgeFrom(first_id);
    for(int i = 0; i < (int)es.size(); ++i)
    {
        Edge e = es.at(i);
        if(e.node_ids.second == second_id)
        {
            return e;
        }
    }
}

vector<Edge> Graph::getEdgeFrom(int node_id)
{
    vector<Edge> es;
    for(int i = 0; i < (int)edges.size(); ++i)
    {
        Edge e = edges.at(i);
        if(e.node_ids.first == node_id)
        {
            es.push_back(e);
        }
    }
    return es;
}

vector<Edge> Graph::getEdgeTo(int node_id)
{
    vector<Edge> es;
    for(int i = 0; i < (int)edges.size(); ++i)
    {
        Edge e = edges.at(i);
        if(e.node_ids.second == node_id)
        {
            es.push_back(e);
        }
    }
    return es;
}

vector<Edge> Graph::getAllEdges()
{
    return edges;
}

void Graph::setNode(const Node &node)
{
    vector<Node>::iterator it;
    for(it = nodes.begin(); it != nodes.end(); ++it)
    {
        if((*it).id == node.id)
        {
            (*it) = node;
            break;
        }
    }
}

void Graph::setEdge(const Edge &edge)
{
    vector<Edge>::iterator it;
    for(it = edges.begin(); it != edges.end(); ++it)
    {
        if((*it).id == edge.id)
        {
            (*it) = edge;
            break;
        }
    }
}

void Graph::setAllNodes(const vector<Node> &new_nodes)
{
    nodes = new_nodes;
}

void Graph::setAllEdges(const vector<Edge> &new_edges)
{
    edges = new_edges;
}

void Graph::sort(int &a, int &b)
{
    // 大小関係をa < bに
    if(a > b)
    {
        int tmp = b;
        b = a;
        a = tmp;
    }
}
