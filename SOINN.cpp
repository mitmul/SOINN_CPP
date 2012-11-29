#include "SOINN.h"

SOINN::SOINN(int node_age, double edge_age)
    : is_first_learning(true)
    , node_erase_age(node_age)
    , edge_erase_age(edge_age)
{
    srand((unsigned)time(NULL));
}

void SOINN::learn(vector<Node> inputs)
{
    input_nodes = inputs;

    initialize();

    // 入力パターンを学習
    for(int i = 0; i < (int)input_nodes.size(); ++i)
    {
        Node input = input_nodes.at(i);

        // 勝者ノードを探す
        vector<Node> winner_nodes = getWinnerNodes(input);

        Node first_winner = winner_nodes.at(0);         // 勝者ノード
        Node second_winner = winner_nodes.at(1);        // 第二勝者ノード

        double first_simular = getSimularThreshold(first_winner);
        double second_simular = getSimularThreshold(second_winner);

        double input_first_dist = getNodeDistance(input, first_winner);
        double input_second_dist = getNodeDistance(input, second_winner);

        // 勝者ノードもしくは第二勝者ノードの類似度閾値の外側なら
        if(input_first_dist > first_simular ||
           input_second_dist > second_simular)
        {
            // 新規ノードとして追加
            soinn.addNode(input.position);
        }

        // 勝者・第二勝者ノードいずれかの類似度閾値の内側の場合
        if(input_first_dist <= first_simular &&
           input_second_dist <= second_simular)
        {
            // 勝者ノードと第二勝者ノードの間にエッジが無いなら
            if(!soinn.hasEdge(first_winner.id, second_winner.id))
            {
                // エッジを追加
                soinn.addEdge(first_winner.id, second_winner.id);
            }

            // エッジの年齢を0にリセット
            setEdgeAge(first_winner.id, second_winner.id, 0);

            // 勝者ノードにつながる全エッジの年齢をインクリメント
            incAllEdgeAge(first_winner.id);

            // 位置ベクトルの更新
            updatePosition(first_winner, input, 1.0, first_winner.win_times);

            // 勝者ノードの関連ノードの位置ベクトルの更新
            updatePositionRelated(first_winner, input, 1.0 / 100.0);

            // 年老いたエッジを削除
            eraseOldEdges();

            // 消されたエッジでつながれていたノードを調べる
            for(int j = 0; j < (int)erased_edges.size(); ++j)
            {
                Edge e = erased_edges.at(j);

                // 関連ノードがなければ削除
                eraseIndependentNode(e.node_ids.first);
                eraseIndependentNode(e.node_ids.second);
            }
        }

        // 入力パターン数がnode_erase_ageの倍数
        if((i + 1) % node_erase_age == 0 ||
           (i + 1) == (int)input_nodes.size())
        {
            eraseNoizyNode();
        }
    }
    drawGraph();
}

vector<Node> SOINN::getLearnedNode()
{
    return soinn.getAllNodes();
}

void SOINN::initialize()
{
    // 初回の学習である
    if(is_first_learning)
    {
        // 学習データから2つ適当に選んで追加
        for(int i = 0; i < 2; ++i)
        {
            int random_num = rand() % input_nodes.size();
            soinn.addNode(input_nodes.at(random_num).position);

            cout << "random" << i << " : " << random_num << endl;
        }
        is_first_learning = false;
    }
}

vector<Node> SOINN::getWinnerNodes(const Node &input_node)
{
    // 距離でソートされたノード番号のセット
    vector<Node> nodes = soinn.getAllNodes();
    multimap<double, int> node_distances = getSortedDistancesNodeNumbers(input_node, nodes);

    // 勝者ノードと第二勝者ノードを返す
    vector<Node> winners;
    multimap<double, int>::iterator it = node_distances.begin();

    // 勝者ノード
    Node win1 = soinn.getNode(it->second);
    ++win1.win_times;
    soinn.setNode(win1);
    winners.push_back(win1);

    ++it;

    // 第二勝者ノード
    Node win2 = soinn.getNode((*it).second);
    winners.push_back(win2);

    return winners;
}

double SOINN::getSimularThreshold(const Node &target_node)
{
    // target_nodeの類似度閾値を計算

    // 連結ノードがある場合は
    if(soinn.getEdgeCount(target_node.id) != 0)
    {
        // 連結ノード集合
        vector<Node> related_nodes = soinn.getRealtedNodes(target_node.id);

        // 連結ノード集合のうち最も遠いノードまでの距離を返す
        if(related_nodes.size() == 1)
        {
            return getNodeDistance(related_nodes.at(0), target_node);
        }
        else
        {
            multimap<double, int> node_distances = getSortedDistancesNodeNumbers(target_node, related_nodes);
            return (*node_distances.end()).first;
        }
    }

    // 連結ノードがない場合は
    else
    {
        // target_node以外のノードの集合
        vector<Node> nodes = soinn.getAllNodes();
        vector<Node> except_nodes;
        for(int i = 0; i < (int)nodes.size(); ++i)
        {
            Node n = nodes.at(i);
            if(n.id != target_node.id)
            {
                except_nodes.push_back(n);
            }
        }

        // target_node以外のノードのうち最も近いノードまでの距離を返す
        multimap<double, int> node_distances = getSortedDistancesNodeNumbers(target_node, except_nodes);
        return (*node_distances.begin()).first;
    }
}

double SOINN::getNodeDistance(const Node &first_node, const Node &second_node)
{
    return sqrt(pow(first_node.position[0] - second_node.position[0], 2.0) +
                pow(first_node.position[1] - second_node.position[1], 2.0));
}

multimap<double, int> SOINN::getSortedDistancesNodeNumbers(const Node &input_node, vector<Node> &compared_nodes)
{
    // 距離でソートされたノード番号のセット
    multimap<double, int> node_distances;
    vector<Node>::iterator it = compared_nodes.begin();
    while(it != compared_nodes.end())
    {
        double d = getNodeDistance((*it), input_node);
        node_distances.insert(pair<double, int>(d, it->id));

        ++it;
    }

    return node_distances;
}

void SOINN::setEdgeAge(int first, int second, int age)
{
    Edge e = soinn.getEdgeFromTo(first, second);
    e.age = age;
    soinn.setEdge(e);
}

void SOINN::incAllEdgeAge(int node_id)
{
    // node_idにつながる全エッジの年齢に1を足す
    vector<Edge> from = soinn.getEdgeFrom(node_id);
    vector<Edge> to = soinn.getEdgeTo(node_id);

    for(int i = 0; i < (int)from.size(); ++i)
    {
        Edge e = from.at(i);
        ++e.age;
        soinn.setEdge(e);
    }
    for(int i = 0; i < (int)to.size(); ++i)
    {
        Edge e = to.at(i);
        ++e.age;
        soinn.setEdge(e);
    }
}

void SOINN::updatePosition(Node &node, const Node &input, double weight, int win_times)
{
    // nodeの位置ベクトル更新
    double epsilon = 1.0 / (double)win_times;
    VectorXd vec = input.position - node.position;
    node.position += epsilon * weight * vec;

    soinn.setNode(node);
}

void SOINN::updatePositionRelated(const Node &node, const Node &input, double weight)
{
    // nodeに関連する全てのノード
    vector<Node> related = soinn.getRealtedNodes(node.id);
    for(int i = 0; i < (int)related.size(); ++i)
    {
        Node n = related.at(i);
        updatePosition(n, input, weight, node.win_times);
    }
}

void SOINN::eraseOldEdges()
{
    erased_edges.clear();

    vector<Edge> edges = soinn.getAllEdges();
    vector<Edge>::iterator it = edges.begin();
    while(it != edges.end())
    {
        if((*it).age > edge_erase_age)
        {
            erased_edges.push_back((*it));
            it = edges.erase(it);
        }
        else
        {
            ++it;
        }
    }

    soinn.setAllEdges(edges);
}

void SOINN::eraseIndependentNode(int node_id)
{
    // 関連ノードを持っていなければ消す
    if(soinn.getEdgeCount(node_id) == 0)
    {
        vector<Node> nodes = soinn.getAllNodes();
        vector<Node>::iterator it = nodes.begin();
        it = nodes.erase(it + node_id);
        soinn.setAllNodes(nodes);

        // そのノードが持っていたエッジを消す
        vector<Edge> edges = soinn.getAllEdges();
        vector<Edge>::iterator eit = edges.begin();
        while(eit != edges.end())
        {
            if((*eit).node_ids.first == node_id ||
               (*eit).node_ids.second == node_id)
            {
                eit = edges.erase(eit);
            }
            else
            {
                ++eit;
            }
        }

        soinn.setAllEdges(edges);
    }
}

void SOINN::eraseNoizyNode()
{
    vector<Node> nodes = soinn.getAllNodes();
    vector<Node>::iterator node_it = nodes.begin();

    while(node_it != nodes.end())
    {
        // 関連ノード数が1未満なら
        if(soinn.getEdgeCount(node_it->id) <= 1)
        {
            // そのエッジを消す
            vector<Edge> edges = soinn.getAllEdges();
            vector<Edge>::iterator edge_it = edges.begin();
            while(edge_it != edges.end())
            {
                // 現在のノードが端点にあるなら
                if(edge_it->node_ids.first == node_it->id ||
                   edge_it->node_ids.second == node_it->id)
                {
                    edge_it = edges.erase(edge_it);
                }
                else
                {
                    ++edge_it;
                }
            }
            soinn.setAllEdges(edges);

            // ノードを消す
            node_it = nodes.erase(node_it);
        }
        else
        {
            ++node_it;
        }
    }

    soinn.setAllNodes(nodes);
}

void SOINN::drawGraph()
{
    Mat graph(300, 300, CV_8UC3);
    graph = Mat::zeros(graph.size(), graph.type());

    vector<Node> nodes = soinn.getAllNodes();
    for(int i = 0; i < (int)nodes.size(); ++i)
    {
        Node n = nodes.at(i);
        circle(graph, Point((int)n.position[1], (int)n.position[0]), 1, Scalar(255, 255, 255), -1);
    }

    vector<Edge> edges = soinn.getAllEdges();
    for(int i = 0; i < (int)edges.size(); ++i)
    {
        Edge e = edges.at(i);
        Node sn = soinn.getNode(e.node_ids.first);
        Node en = soinn.getNode(e.node_ids.second);
        line(graph,
             Point((int)sn.position[1], (int)sn.position[0]),
             Point((int)en.position[1], (int)en.position[0]),
             Scalar(180, 180, i % 5 * 50), 1, CV_AA);
    }

    imwrite("graph.png", graph);
}
