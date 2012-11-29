#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <map>
#include <vector>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

class Node
{
public:
    int         id;                     // ノード番号
    VectorXd    position;               // ノードの位置ベクトル
    double      simular_threshold;      // 類似度閾値
    int         win_times;              // 勝者ノードに選択された回数

};

class Edge
{
public:
    int                 id;             // エッジ番号
    pair<int, int>      node_ids;       // 始点と終点のノードID
    int                 age;            // エッジの年齢
};

class Graph
{
public:
    Graph();

    void addNode(VectorXd position);
    void addEdge(int first_node_id, int second_node_id);

    bool hasEdge(int first_node_id, int second_node_id);
    bool hasEdge(int node_id);

    Node getNode(int node_id);
    vector<Node> getRealtedNodes(int node_id);
    vector<Node> getAllNodes();
    int getEdgeCount(int node_id);
    Edge getEdgeFromTo(int first_id, int second_id);
    vector<Edge> getEdgeFrom(int node_id);
    vector<Edge> getEdgeTo(int node_id);
    vector<Edge> getAllEdges();

    void setNode(const Node &node);
    void setEdge(const Edge &edge);
    void setAllNodes(const vector<Node> &new_nodes);
    void setAllEdges(const vector<Edge> &new_edges);

private:
    vector<Edge> edges;
    vector<Node> nodes;

    int last_node_id;
    int last_edge_id;

    void sort(int &a, int &b);

};

#endif // GRAPH_H
