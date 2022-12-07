#define VEX_MAX_NUM 50
#define INFINITY 100
#include <iostream>
using namespace std;

enum class visitStatus
{
    VISIT,
    UNVISIT
};

class adjMatrix
{
public:
    adjMatrix(int vexMaxNum = VEX_MAX_NUM, int infinity = INFINITY);
    adjMatrix(char *vexs, int vexNum, int vexMaxNum = VEX_MAX_NUM, int infinity = INFINITY);
    void insertArc(int v1, int v2, int weight);
    ~adjMatrix();
    int getVexNum() const { return _vexNum; };
    int getArcNum() const { return _arcNum; };
    int firstAdjVex(int v);
    int nextAdjVex(int v, int w);
    int getWeight(int v1, int v2) const { return arcs[v1][v2]; };
    int getInfinity() const { return _infinity; };
    int getPos(char v) const;
    char getName(int pos) const;
    char *getVertices() const { return vertices; };
    int **getArcs() const { return arcs; };
    friend ostream &operator<<(ostream &out, const adjMatrix &g);

private:
    char *vertices;
    int _vexNum;
    int _vexMaxNum;
    int _arcNum;
    int _infinity;
    visitStatus *tag;
    int **arcs;
};

adjMatrix::adjMatrix(int vexMaxNum, int infinity) : _vexNum(0), _arcNum(0), _infinity(infinity), _vexMaxNum(vexMaxNum)
{
    vertices = new char[_vexNum];
    tag = new visitStatus[_vexNum];
    arcs = new int *[_vexNum];
    for (int i = 0; i < _vexNum; i++)
        arcs[i] = new int[_vexNum];
}

adjMatrix::adjMatrix(char *vexs, int vexNum, int vexMaxNum, int infinity) : _vexNum(vexNum), _vexMaxNum(vexMaxNum), _arcNum(0), _infinity(infinity)
{
    vertices = new char[_vexNum];
    tag = new visitStatus[_vexNum];
    arcs = new int *[_vexNum];
    for (int i = 0; i < _vexNum; i++)
        arcs[i] = new int[_vexNum];
    for (int i = 0; i < _vexNum; i++)
    {
        vertices[i] = vexs[i];
        tag[i] = visitStatus::UNVISIT;
        for (int j = 0; j < _vexNum; j++)
            arcs[i][j] = infinity;
    }
}

adjMatrix::~adjMatrix()
{
    if (vertices != nullptr)
        delete[] vertices;
    if (tag != nullptr)
        delete[] tag;
    if (arcs != nullptr)
        delete arcs;
}

void adjMatrix::insertArc(int v1, int v2, int weight)
{
    int choice = 0;
    if (arcs[v1][v2] != _infinity)
    {
        cout << "the arc already exists, do you want to modify the weight of it?(1 for Yes, 2 for No)" << endl;
        while (1)
        {
            cout << "please enter the number:";
            cin >> choice;
            if (cin.fail() || (choice != 1 && choice != 2))
            {
                cin.clear();
                cin.ignore(9999, '\n');
                cout << "invalid input!!! input it again!!!" << endl;
                continue;
            }
            else
                break;
        }
    }
    if (choice == 0 || choice == 1)
    {
        arcs[v1][v2] = weight;
        arcs[v2][v1] = weight;
        if (choice == 0)
            _arcNum++;
    }
    else if (choice == 2)
        return;
}

int adjMatrix::firstAdjVex(int v) // 寻找相邻点
{
    if (!(v < 0 || v >= _vexNum))
    {
        for (int i = 0; i < _vexNum; i++)
        {
            if (arcs[v][i] >= 0 && arcs[v][i] != _infinity)
                return i;
        }
    }
    return -1;
}

int adjMatrix::nextAdjVex(int v, int w) // 寻找相对w的相邻点
{
    if (v >= 0 && v < _vexNum && w >= 0 && w < _vexNum)
    {
        for (int i = w + 1; i < _vexNum; i++)
        {
            if (arcs[v][i] >= 0 && arcs[v][i] != _infinity)
                return i;
        }
    }
    return -1;
}

ostream &operator<<(ostream &out, const adjMatrix &g)
{
    for (int i = 0; i < g._vexNum; i++)
    {
        out << "[";
        for (int j = 0; j < g._vexNum; j++)
        {
            if (g.arcs[i][j] != g._infinity)
                out << g.arcs[i][j];
            else
                out << "∞";
            out << " ";
        }
        out << '\b';
        out << "]" << endl;
    }
    return out;
}

int adjMatrix::getPos(char v) const
{
    for (int i = 0; i < _vexNum; i++)
    {
        if (vertices[i] == v)
            return i;
    }
    return -1;
}

char adjMatrix::getName(int pos) const
{
    if (pos >= 0 && pos < _vexNum)
        return vertices[pos];
    return '#';
}

class primNode
{
public:
    int lowCost;
    int nearVex;
};

class minSpanTreePrim
{
public:
    static adjMatrix *getMinSpanTree(adjMatrix *net, int startVex)
    {
        int vexNum = net->getVexNum(), i, j, k;
        primNode *vexs;
        vexs = new primNode[vexNum];
        adjMatrix *minSpanTree = new adjMatrix(net->getVertices(), vexNum);
        vexs[startVex].lowCost = 0;
        vexs[startVex].nearVex = -1;
        for (i = 0; i < vexNum; i++)
            if (startVex != i)
            {
                vexs[i].lowCost = net->getWeight(startVex, i);
                vexs[i].nearVex = startVex;
            }
        for (i = 1; i < vexNum; i++)
        {
            int minWeight = net->getInfinity(), minOrder;
            for (j = 0; j < vexNum; j++)
                if (vexs[j].lowCost != 0 && vexs[j].lowCost < minWeight)
                {
                    minWeight = vexs[j].lowCost;
                    minOrder = j;
                }
            if (minWeight == net->getInfinity())
            {
                cerr << "the graph is not a connectivity graph, unable to generate a minimal spanning tree!" << endl;
                delete minSpanTree;
                minSpanTree = nullptr;
                break;
            }
            else
            {
                vexs[minOrder].lowCost = 0;
                minSpanTree->insertArc(vexs[minOrder].nearVex, minOrder, minWeight);
                for (k = net->firstAdjVex(minOrder); k != -1; k = net->nextAdjVex(minOrder, k))
                    if (vexs[k].lowCost != 0 && net->getWeight(minOrder, k) < vexs[k].lowCost)
                    {
                        vexs[k].lowCost = net->getWeight(minOrder, k);
                        vexs[k].nearVex = minOrder;
                    }
            }
        }
        delete[] vexs;
        return minSpanTree;
    }
};

class powerGrid
{
private:
    adjMatrix *net;
    adjMatrix *minSpanTree;

public:
    powerGrid() : net(nullptr), minSpanTree(nullptr){};
    ~powerGrid();
    void buildVertex();
    void buildEdge();
    void buildMinSpanTree();
    void showMinSpanTree();
};

powerGrid::~powerGrid()
{
    if (net != nullptr)
    {
        delete net;
        net = nullptr;
    }
    if (minSpanTree != nullptr)
    {
        delete minSpanTree;
        minSpanTree = nullptr;
    }
}

void powerGrid::buildVertex()
{
    if (net == nullptr)
    {
        int n;
        while (1)
        {
            cout << "请输入顶点的个数：";
            cin >> n;
            if (cin.fail() || n <= 0)
            {
                cin.clear();
                cin.ignore(99999, '\n');
                cout << "error" << endl;
            }
            else
                break;
        }
        cout << "请依次输入各顶点的名称：" << endl;
        char vexs[VEX_MAX_NUM];
        vexs[n] = '\0';
        for (int i = 0; i < n; i++)
            cin >> vexs[i];
        cin.ignore(9999, '\n');
        net = new adjMatrix(vexs, n);
    }
    else
        cerr << "the powerGrid's vertices have already been created, do not create it again" << endl;
}

void powerGrid::buildEdge()
{
    while (1)
    {
        char v1, v2;
        int weight;
        cout << "请输入两个顶点及边：";
        cin >> v1 >> v2 >> weight;
        if (v1 == '?' || v2 == '?' || weight == 0)
        {
            cin.ignore(9999, '\n');
            break;
        }
        else if (cin.fail() || weight < 0 || v1 == v2)
        {
            cerr << "invalid input!!!\nplease input again!!!" << endl;
            cin.ignore(99999, '\n');
            continue;
        }
        else
        {
            int pos1 = net->getPos(v1);
            int pos2 = net->getPos(v2);
            if (pos1 == -1 || pos2 == -1)
            {
                if (pos1 == -1)
                    cerr << "there is no vertex called " << v1 << endl;
                if (pos2 == -1)
                    cerr << "there is no vertex called " << v2 << endl;
                cerr << "please input again!" << endl;
                continue;
            }
            net->insertArc(pos1, pos2, weight);
        }
    }
}

void powerGrid::buildMinSpanTree()
{
    char startVex;
    int beginPos = 0;
    while (1)
    {
        cout << "请输入起始顶点：";
        cin >> startVex;
        cin.ignore(99999, '\n');
        if (beginPos = net->getPos(startVex) < 0)
        {
            cout << "there is no vertex called " << startVex << endl;
            continue;
        }
        else
            break;
    }
    minSpanTree = minSpanTreePrim::getMinSpanTree(net, beginPos);
    if (minSpanTree != nullptr)
        cout << "生成prim最小生成树!" << endl;
}

void powerGrid::showMinSpanTree()
{
    if (minSpanTree != nullptr)
    {
        char *vexs = minSpanTree->getVertices();
        int **arcs = minSpanTree->getArcs();
        int vexNum = minSpanTree->getVexNum();
        cout << "最小生成树各个顶点以及与其相邻的边如下：" << endl;
        for (int i = 0; i < vexNum; i++)
        {
            char v = minSpanTree->getName(i);
            cout << "顶点：" << v << '\t';
            for (int j = 0; j < vexNum; j++)
            {
                if (arcs[i][j] != minSpanTree->getInfinity())
                    cout << v << "-<" << arcs[i][j] << ">-" << minSpanTree->getName(j) << '\t';
            }
            cout << endl;
        }
    }
    else
        cerr << "the minSpanTree hasn't been generated yet" << endl;
}

int main()
{
    cout << "**\t电网造价模拟系统\t**\n"
         << "==================================\n"
         << "**\tA --- 创建电网顶点\t**\n"
         << "**\tB --- 添加电网的边\t**\n"
         << "**\tC --- 构造最小生成树\t**\n"
         << "**\tD --- 显示最小生成树\t**\n"
         << "**\tE --- 退出    程序\t**\n"
         << "==================================\n";
    powerGrid powGid;
    while (1)
    {
        cout << endl
             << "请选择操作：";
        char choice;
        cin >> choice;
        switch (choice)
        {
        case 'A':
        case 'a':
            powGid.buildVertex();
            break;
        case 'B':
        case 'b':
            powGid.buildEdge();
            break;
        case 'C':
        case 'c':
            powGid.buildMinSpanTree();
            break;
        case 'D':
        case 'd':
            powGid.showMinSpanTree();
            break;
        case 'E':
        case 'e':
            system("pause");
            return 0;
        default:
            break;
        }
    }
}