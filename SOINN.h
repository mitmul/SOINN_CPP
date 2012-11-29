#ifndef SOINN_H
#define SOINN_H

#include <Eigen/Eigen>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <opencv.hpp>
#include "Graph.h"

using namespace std;
using namespace Eigen;
using namespace cv;

class SOINN
{
public:
    SOINN(int node_age, double edge_age);

    void learn(vector<Node> inputs);

    vector<Node> getLearnedNode();

private:
    bool is_first_learning;     // 初回の学習かどうか
    int node_erase_age;         // ノード削除パラメータ
    double edge_erase_age;      // エッジ削除パラメータ
    vector<Node> input_nodes;   // 入力パターン
    vector<Edge> erased_edges;  // 削除されたエッジ

    Graph soinn;

    void initialize();

    vector<Node> getWinnerNodes(const Node &input_node);
    double getSimularThreshold(const Node &target_node);
    double getNodeDistance(const Node &first_node, const Node &second_node);
    multimap<double, int> getSortedDistancesNodeNumbers(const Node &input_node, vector<Node> &compared_nodes);

    void setEdgeAge(int first, int second, int age);
    void incAllEdgeAge(int node_id);

    void updatePosition(Node &node, const Node &input, double weight, int win_times);
    void updatePositionRelated(const Node &node, const Node &input, double weight);

    void eraseOldEdges();
    void eraseIndependentNode(int node_id);
    void eraseNoizyNode();

    void drawGraph();
};

#endif // SOINN_H
