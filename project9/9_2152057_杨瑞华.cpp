#define VEXMAXNUM 150
#include <iostream>
#include <windows.h>
#include <stdlib.h>
using namespace std;
enum class visitStatus
{
    VISIT,
    UNVISIT
};

class edgeNode
{
public:
    int adjVexPos;
    int weight;
    edgeNode *next;
    edgeNode() : adjVexPos(-1), weight(0), next(nullptr){};
    edgeNode(int _adjVexPos, int _weight, edgeNode *_next = nullptr) : adjVexPos(_adjVexPos), weight(_weight), next(_next){};
};

class vexNode
{
public:
    int name;
    edgeNode *firstEdgeNode;
    int inDegree;
    int outDegree;
    vexNode() : name(-1), inDegree(0), outDegree(0), firstEdgeNode(nullptr){};
    vexNode(int _name, edgeNode *_firstEdgeNode = nullptr) : name(_name), inDegree(0), outDegree(0), firstEdgeNode(_firstEdgeNode){};
};

class adjDirList
{
private:
    visitStatus *tags; // 访问标记数组
    int *topArray;     // 拓扑排序结果
    vexNode *vertices; // 结点数组
    int vexNum;        // 结点数量
    int edgeNum;       // 边的数量
    int vexMaxNum;     // 图的最大结点数量

public:
    adjDirList();                                                        // 默认构造函数，构造空图
    adjDirList(int *_names, int _vexNum, int _vexMaxNum = VEXMAXNUM);    // 构造包含多个结点的有向图
    ~adjDirList();                                                       // 析构函数，释放内存
    void clearAllEdges();                                                // 清除图中所有边
    bool isEmpty() const { return vexNum == 0; };                        // 判断图是否为空
    int getVexNum() const { return vexNum; };                            // 获取结点数量
    int getEdgeNum() const { return edgeNum; };                          // 获取边的数量
    int getVexMaxNum() const { return vexMaxNum; };                      // 获取图的最大结点数量
    int getName(int pos) const;                                          // 获取指定位置的结点名称
    int getPos(int name) const;                                          // 获取指定名称的结点位置
    int getWeight(int v1, int v2) const;                                 // 获取从结点 v1 到结点 v2 的边的权值
    int *getTopArray() const { return topArray; };                       // 获取拓扑排序结果数组
    int firstAdjVex(int v) const;                                        // 获取结点 v 的第一个邻接结点
    int nextAdjVex(int v, int w) const;                                  // 获取结点 v 的邻接结点 w 的下一个邻接结点
    bool addEdge(int startPos, int endPos, int weight);                  // 在结点 startPos 和 endPos 之间添加一条权值为 weight 的边
    bool addVex(int name);                                               // 向图中添加一个名称为 name 的结点
    bool removeEdge(int startPos, int endPos);                           // 删除结点 startPos 和 endPos 之间的边
    bool removeVex(int pos);                                             // 删除图中指定位置的结点
    friend ostream &operator<<(ostream &out, const adjDirList &l);       // 重载输出运算符，用于输出图的信息
    int getInDegree(int pos) const { return vertices[pos].inDegree; };   // 获取结点 pos 的入度
    int getOutDegree(int pos) const { return vertices[pos].outDegree; }; // 获取结点 pos 的出度
    bool topSort();                                                      // 拓扑排序
};

adjDirList::adjDirList() : vexNum(0), edgeNum(0), vexMaxNum(VEXMAXNUM), topArray(nullptr)
{
    tags = new visitStatus[VEXMAXNUM];
    vertices = new vexNode[VEXMAXNUM];
}

adjDirList::adjDirList(int *_names, int _vexNum, int _vexMaxNum) : vexNum(_vexNum), vexMaxNum(_vexMaxNum), edgeNum(0), topArray(nullptr)
{
    tags = new visitStatus[VEXMAXNUM];
    vertices = new vexNode[VEXMAXNUM];
    for (int i = 0; i < _vexNum; i++)
    {
        tags[i] = visitStatus::UNVISIT;
        vertices[i] = _names[i];
    }
}

adjDirList::~adjDirList()
{
    if (tags != nullptr)
        delete[] tags;
    clearAllEdges();
    if (vertices != nullptr)
        delete[] vertices;
    if (topArray != nullptr)
        delete[] topArray;
}

void adjDirList::clearAllEdges()
{
    for (int i = 0; i < vexNum; i++)
    {
        edgeNode *p;
        while (vertices[i].firstEdgeNode)
        {
            p = vertices[i].firstEdgeNode;
            vertices[i].firstEdgeNode = p->next;
            delete p;
        }
    }
    edgeNum = 0;
}

int adjDirList::getName(int pos) const
{
    if (pos >= 0 && pos < vexNum)
        return vertices[pos].name;
    return -1;
}

int adjDirList::getPos(int name) const
{
    for (int i = 0; i < vexNum; i++)
    {
        if (vertices[i].name == name)
            return i;
    }
    return -1;
}

int adjDirList::getWeight(int v1, int v2) const
{
    if (v2 >= 0 && v1 < vexNum && v2 >= 0 && v2 < vexNum)
    {
        edgeNode *p = vertices[v1].firstEdgeNode;
        while (p)
        {
            if (p->adjVexPos == v2)
                return p->weight;
            p = p->next;
        }
    }
    return -1;
}

int adjDirList::firstAdjVex(int v) const
{
    if (v >= 0 && v < vexNum)
    {
        edgeNode *p = vertices[v].firstEdgeNode;
        while (p)
        {
            if (p->adjVexPos >= 0 && p->adjVexPos != v)
                return p->adjVexPos;
            p = p->next;
        }
    }
    return -1;
}

int adjDirList::nextAdjVex(int v, int w) const
{
    if (v >= 0 && v < vexNum && w >= 0 && w < vexNum && v != w)
    {
        edgeNode *p = vertices[v].firstEdgeNode;
        while (p && p->next != nullptr)
        {
            if (p->adjVexPos == w)
                break;
            p = p->next;
        }
        if (p->next != nullptr)
            return p->next->adjVexPos;
    }
    return -1;
}

bool adjDirList::addEdge(int startPos, int endPos, int weight)
{
    // 判断边的起始位置和结束位置是否合法
    if (startPos >= 0 && startPos < vexNum && endPos >= 0 && endPos < vexNum && startPos != endPos)
    {
        // 在起始结点的链表中添加一个结点，表示从起始结点到结束结点的有向边
        vertices[startPos].firstEdgeNode = new edgeNode(endPos, weight, vertices[startPos].firstEdgeNode);
        // 更新起始结点的出度
        vertices[startPos].outDegree++;
        // 更新结束结点的入度
        vertices[endPos].inDegree++;
        // 更新边的数量
        edgeNum++;
        return true;
    }
    return false;
}

bool adjDirList::addVex(int name)
{
    vertices[vexNum].name = name;
    vexNum++;
    return true;
}

bool adjDirList::removeEdge(int startPos, int endPos)
{
    // 判断边的起始位置和结束位置是否合法
    if (startPos >= 0 && startPos < vexNum && endPos >= 0 && endPos < vexNum && startPos != endPos)
    {
        // 遍历起始结点的链表，找到与结束结点
        edgeNode *p = vertices[startPos].firstEdgeNode, *q;
        while (p && p->adjVexPos != endPos)
        {
            q = p;
            p = p->next;
        }
        // 如果找到了与结束结点，删除它
        if (p)
        {
            // 如果要删除的结点是链表的第一个结点，则更新链表的头指针
            if (vertices[startPos].firstEdgeNode == p)
                vertices[startPos].firstEdgeNode = p->next;
            // 否则更新链表中要删除结点的上一个结点的 next 指针
            else
                q->next = p->next;
            // 删除结点
            delete p;
            // 更新起始结点的出度
            vertices[startPos].outDegree--;
            // 更新结束结点的入度
            vertices[endPos].inDegree--;
            // 更新边的数量
            edgeNum--;
            return true;
        }
    }
    return false;
}

bool adjDirList::removeVex(int pos)
{
    // 判断要删除的结点是否合法
    if (pos >= 0 && pos < vexNum)
    {
        int i;
        edgeNode *p, *s, *q;
        // 删除该结点的出边
        while (vertices[pos].firstEdgeNode)
        {
            p = vertices[pos].firstEdgeNode;
            vertices[pos].firstEdgeNode = p->next;
            // 更新该结点所连接结点的入度
            vertices[p->adjVexPos].inDegree--;
            delete p;
        }
        // 遍历所有结点，删除所有指向该结点的边
        for (int i = 0; i < vexNum; i++)
        {
            if (i != pos)
            {
                s = vertices[i].firstEdgeNode;
                while (s && s->adjVexPos != pos)
                {
                    q = s;
                    s = s->next;
                }
                // 如果找到了指向该结点的边，则删除它
                if (s)
                {
                    // 如果要删除的结点是链表的第一个结点，则更新链表的头指针
                    if (vertices[i].firstEdgeNode == s)
                        vertices[i].firstEdgeNode = s->next;
                    // 否则更新链表中要删除结点的上一个结点的 next 指针
                    else
                        q->next = s->next;
                    // 更新该结点的出度
                    vertices[i].outDegree--;
                    // 删除结点
                    delete s;
                    // 更新边的数量
                    edgeNum--;
                }
            }
        }
        vexNum--;
        vertices[pos] = vertices[vexNum];
        vertices[vexNum].name = -1;
        vertices[vexNum].firstEdgeNode = nullptr;
        for (int i = 0; i < vexNum; i++)
        {
            if (i != pos)
            {
                p = vertices[i].firstEdgeNode;
                while (p)
                {
                    if (p->adjVexPos == vexNum)
                        p->adjVexPos = pos;
                    p = p->next;
                }
            }
        }
        return true;
    }
    return false;
}

ostream &operator<<(ostream &out, const adjDirList &l)
{
    for (int i = 0; i < l.vexNum; i++)
    {
        out << "顶点：\t" << l.vertices[i].name << '\t';
        edgeNode *p = l.vertices[i].firstEdgeNode;
        while (p)
        {
            out << "(pos: " << p->adjVexPos << "\tname: " << l.vertices[p->adjVexPos].name << "\tweight: " << p->weight << ")\t";
            p = p->next;
        }
        out << endl;
    }
    return out;
}

bool adjDirList::topSort()
{
    int top = -1, count = 0;         // top 为栈顶元素的索引，count 用来计数结果数组中已经加入了多少元素
    int *inDegree = new int[vexNum]; // 创建并初始化 inDegree 数组
    for (int i = 0; i < vexNum; i++)
        inDegree[i] = vertices[i].inDegree;
    for (int i = 0; i < vexNum; i++)
    {
        if (inDegree[i] == 0)
        {
            // 如果当前顶点的入度为 0，则将其入栈
            inDegree[i] = top;
            top = i;
        }
    }
    if (topArray)
        delete[] topArray;      // 清空原来的结果数组
    topArray = new int[vexNum]; // 创建新的结果数组
    while (top != -1)           // 只要栈不为空，就一直循环
    {
        topArray[count++] = getName(top); // 取出栈顶元素，加入结果数组
        int v = top;
        top = inDegree[top]; // 更新栈顶元素
        for (int i = firstAdjVex(v); i != -1; i = nextAdjVex(v, i))
        {
            if (--inDegree[i] == 0)
            {
                // 如果当前顶点的入度为 0，则将其入栈
                inDegree[i] = top;
                top = i;
            }
        }
    }
    delete[] inDegree; // 释放 inDegree 数组的内存
    if (count != vexNum)
        return false; // 如果结果数组的长度不等于图中顶点的数量，说明图中有环，拓扑排序失败
    else
        return true; // 如果结果数组的长度等于图中顶点的数量，则拓扑排序成功
}

class AOE
{
private:
    adjDirList project; // 项目图
    int *topArray;      // 拓扑排序结果数组
    int time;           // 最短完成时间

public:
    AOE() : topArray(nullptr), time(0){}; // 默认构造函数
    ~AOE();                               // 析构函数，释放内存
    void build_AOE();                     // 建立AOE网络
    void output_AOE();                    // 输出AOE网络
};

AOE::~AOE()
{
    if (topArray != nullptr)
        delete[] topArray;
}

void AOE::build_AOE()
{
    int incidents, missionNum;
    while (1)
    {
        cout << "请输入任务交接点数量(1-100)和子任务数量:";
        cin >> incidents >> missionNum;
        if (cin.fail() || incidents <= 0 || incidents > 100 || missionNum < incidents || missionNum >= incidents * (incidents - 1) / 2)
        {
            cin.clear();
            cin.ignore(9999, '\n');
            cerr << "error,please input again" << endl;
            continue;
        }
        else
            break;
    }
    for (int i = 0; i < incidents; i++)
        project.addVex(i + 1);
    int v1, v2, e, count = 0;
    while (count < missionNum)
    {
        cout << "请输入第" << count + 1 << "个子任务的开始交接点,完成交接点,以及完成任务所需时间:";
        cin >> v1 >> v2 >> e;
        if (v1 <= 0 || v1 > incidents || v2 <= 0 || v2 > incidents || e <= 0 || cin.fail())
        {
            cin.clear();
            cin.ignore(9999, '\n');
            cerr << "error,please input again" << endl;
            continue;
        }
        else
        {
            count++;
            project.addEdge(v1 - 1, v2 - 1, e);
        }
    }
}

void AOE::output_AOE()
{
    if (!project.topSort()) // if exisits directed rings, cout 0
        cout << 0 << endl;
    else
    {
        int incidents = project.getVexNum(); // get the incidents nums

        topArray = new int[incidents]; // get the array of topsort
        int *tmp = project.getTopArray();
        for (int i = 0; i < incidents; i++)
            topArray[i] = tmp[i];

        int *ve = new int[incidents];
        int *vl = new int[incidents];
        int ae, al;

        for (int i = 0; i < incidents; i++)
            ve[i] = 0;
        for (int i = 0; i < incidents; i++) // 按照拓扑排序顺序计算Ve
        {
            for (int k = project.firstAdjVex(topArray[i] - 1); k != -1; k = project.nextAdjVex(topArray[i] - 1, k))
            {
                int w = project.getWeight(topArray[i] - 1, k);
                if (ve[topArray[i] - 1] + w > ve[k])
                    ve[k] = ve[topArray[i] - 1] + w;
            }
        }

        for (int i = 0; i < incidents; i++)
            vl[i] = INT32_MAX;
        vl[incidents - 1] = ve[incidents - 1];
        for (int i = incidents - 2; i >= 0; i--) // 按照逆拓扑排序计算Vl
        {
            for (int k = project.firstAdjVex(topArray[i] - 1); k != -1; k = project.nextAdjVex(topArray[i] - 1, k))
            {
                int w = project.getWeight(topArray[i] - 1, k);
                if (vl[k] - w < vl[topArray[i] - 1])
                    vl[topArray[i] - 1] = vl[k] - w;
            }
        }

        // 最短时间即为拓扑排序最后一个交接点的的最早开始时间
        time = ve[incidents - 1];
        cout << time << endl; // output the time;

        int cnt = 0;
        for (int i = 0; i < incidents; i++)
        {
            for (int j = project.firstAdjVex(topArray[i] - 1); j != -1; j = project.nextAdjVex(topArray[i] - 1, j))
            {
                int w = project.getWeight(topArray[i] - 1, j);
                // 计算ae和al
                ae = ve[topArray[i] - 1];
                al = vl[j] - w;
                if (ae == al) // 如果ae==al，则说明为关键活动
                {
                    cout << topArray[i] << "->" << j + 1 << endl;
                    cnt++;
                }
            }
        }
        cout << endl;
    }
}

int main()
{
    AOE net;
    net.build_AOE();
    net.output_AOE();
    system("pause");
    return 0;
}