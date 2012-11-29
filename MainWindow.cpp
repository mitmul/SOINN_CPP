#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Learn_clicked()
{
    int scale = 3;
    Mat input(scale * 100, scale * 100, CV_8UC1);
    input = Mat::zeros(input.size(), input.type());
    circle(input, Point(scale * 30, scale * 30), 5 * scale, Scalar(255), 3, CV_AA);
    circle(input, Point(scale * 30, scale * 30), 20 * scale, Scalar(255), 3, CV_AA);
    rectangle(input, Point(scale * 50, scale * 50), Point(scale * 70, scale * 80), Scalar(255), 5, CV_AA);

    SOINN soinn(3000, 30);
    for(int learn_num = 0; learn_num < 10; ++learn_num)
    {
        // ノイズ生成
        Mat noize(input.size(), input.type());
        noize = Mat::zeros(input.size(), input.type());
        randn(noize, Scalar::all(255), Scalar::all(5E11));

        stringstream ss;
        ss << "noize" << learn_num << ".png";
        imwrite(ss.str(), noize);

        // ノイズと合成した入力画像
        Mat noized_input(input.size(), input.type());
        noized_input = Mat::zeros(input.size(), input.type());
        bitwise_xor(input, noize, noized_input);

        ss.str("");
        ss << "input" << learn_num << ".png";
        imwrite(ss.str(), noized_input);

        // 入力ノード
        vector<Node> input_nodes;
        for(int i = 0; i < (int)input.rows; ++i)
        {
            for(int j = 0; j < input.cols; ++j)
            {
                if(noized_input.at<uchar>(i, j) == 255)
                {
                    Node new_node;
                    new_node.position = VectorXd(2);
                    new_node.position << i, j;
                    input_nodes.push_back(new_node);
                }
            }
        }
        random_shuffle(input_nodes.begin(), input_nodes.end());

        // いざ学習
        cout << "all inputs:" << input_nodes.size() << endl;

        clock_t start, end;
        start = clock();
        soinn.learn(input_nodes);
        end = clock();

        cout << "learning finished! time=" << (double)(end-start) / CLOCKS_PER_SEC << endl;

        // 結果画像保存
        Mat result(input.size(), input.type());
        result = Mat::zeros(result.size(), result.type());

        vector<Node> learned_result = soinn.getLearnedNode();
        for(int i = 0; i < (int)learned_result.size(); ++i)
        {
            Node r = learned_result.at(i);
            if(!isnan(r.position[0]) && !isnan(r.position[1])
               && !isinf(r.position[0]) && !isinf(r.position[1])
               && result.rows > (int)r.position[0]
               && result.cols > (int)r.position[1])
                result.at<uchar>((int)r.position[0], (int)r.position[1]) = 255;
        }

        ss.str("");
        ss << "result" << learn_num << ".png";
        imwrite(ss.str(), result);
    }
}
