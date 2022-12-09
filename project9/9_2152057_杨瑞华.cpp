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
    visitStatus *tags; // ���ʱ������
    int *topArray;     // ����������
    vexNode *vertices; // �������
    int vexNum;        // �������
    int edgeNum;       // �ߵ�����
    int vexMaxNum;     // ͼ�����������

public:
    adjDirList();                                                        // Ĭ�Ϲ��캯���������ͼ
    adjDirList(int *_names, int _vexNum, int _vexMaxNum = VEXMAXNUM);    // ������������������ͼ
    ~adjDirList();                                                       // �����������ͷ��ڴ�
    void clearAllEdges();                                                // ���ͼ�����б�
    bool isEmpty() const { return vexNum == 0; };                        // �ж�ͼ�Ƿ�Ϊ��
    int getVexNum() const { return vexNum; };                            // ��ȡ�������
    int getEdgeNum() const { return edgeNum; };                          // ��ȡ�ߵ�����
    int getVexMaxNum() const { return vexMaxNum; };                      // ��ȡͼ�����������
    int getName(int pos) const;                                          // ��ȡָ��λ�õĽ������
    int getPos(int name) const;                                          // ��ȡָ�����ƵĽ��λ��
    int getWeight(int v1, int v2) const;                                 // ��ȡ�ӽ�� v1 ����� v2 �ıߵ�Ȩֵ
    int *getTopArray() const { return topArray; };                       // ��ȡ��������������
    int firstAdjVex(int v) const;                                        // ��ȡ��� v �ĵ�һ���ڽӽ��
    int nextAdjVex(int v, int w) const;                                  // ��ȡ��� v ���ڽӽ�� w ����һ���ڽӽ��
    bool addEdge(int startPos, int endPos, int weight);                  // �ڽ�� startPos �� endPos ֮�����һ��ȨֵΪ weight �ı�
    bool addVex(int name);                                               // ��ͼ�����һ������Ϊ name �Ľ��
    bool removeEdge(int startPos, int endPos);                           // ɾ����� startPos �� endPos ֮��ı�
    bool removeVex(int pos);                                             // ɾ��ͼ��ָ��λ�õĽ��
    friend ostream &operator<<(ostream &out, const adjDirList &l);       // ���������������������ͼ����Ϣ
    int getInDegree(int pos) const { return vertices[pos].inDegree; };   // ��ȡ��� pos �����
    int getOutDegree(int pos) const { return vertices[pos].outDegree; }; // ��ȡ��� pos �ĳ���
    bool topSort();                                                      // ��������
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
    // �жϱߵ���ʼλ�úͽ���λ���Ƿ�Ϸ�
    if (startPos >= 0 && startPos < vexNum && endPos >= 0 && endPos < vexNum && startPos != endPos)
    {
        // ����ʼ�������������һ����㣬��ʾ����ʼ��㵽�������������
        vertices[startPos].firstEdgeNode = new edgeNode(endPos, weight, vertices[startPos].firstEdgeNode);
        // ������ʼ���ĳ���
        vertices[startPos].outDegree++;
        // ���½����������
        vertices[endPos].inDegree++;
        // ���±ߵ�����
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
    // �жϱߵ���ʼλ�úͽ���λ���Ƿ�Ϸ�
    if (startPos >= 0 && startPos < vexNum && endPos >= 0 && endPos < vexNum && startPos != endPos)
    {
        // ������ʼ���������ҵ���������
        edgeNode *p = vertices[startPos].firstEdgeNode, *q;
        while (p && p->adjVexPos != endPos)
        {
            q = p;
            p = p->next;
        }
        // ����ҵ����������㣬ɾ����
        if (p)
        {
            // ���Ҫɾ���Ľ��������ĵ�һ����㣬����������ͷָ��
            if (vertices[startPos].firstEdgeNode == p)
                vertices[startPos].firstEdgeNode = p->next;
            // �������������Ҫɾ��������һ������ next ָ��
            else
                q->next = p->next;
            // ɾ�����
            delete p;
            // ������ʼ���ĳ���
            vertices[startPos].outDegree--;
            // ���½����������
            vertices[endPos].inDegree--;
            // ���±ߵ�����
            edgeNum--;
            return true;
        }
    }
    return false;
}

bool adjDirList::removeVex(int pos)
{
    // �ж�Ҫɾ���Ľ���Ƿ�Ϸ�
    if (pos >= 0 && pos < vexNum)
    {
        int i;
        edgeNode *p, *s, *q;
        // ɾ���ý��ĳ���
        while (vertices[pos].firstEdgeNode)
        {
            p = vertices[pos].firstEdgeNode;
            vertices[pos].firstEdgeNode = p->next;
            // ���¸ý�������ӽ������
            vertices[p->adjVexPos].inDegree--;
            delete p;
        }
        // �������н�㣬ɾ������ָ��ý��ı�
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
                // ����ҵ���ָ��ý��ıߣ���ɾ����
                if (s)
                {
                    // ���Ҫɾ���Ľ��������ĵ�һ����㣬����������ͷָ��
                    if (vertices[i].firstEdgeNode == s)
                        vertices[i].firstEdgeNode = s->next;
                    // �������������Ҫɾ��������һ������ next ָ��
                    else
                        q->next = s->next;
                    // ���¸ý��ĳ���
                    vertices[i].outDegree--;
                    // ɾ�����
                    delete s;
                    // ���±ߵ�����
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
        out << "���㣺\t" << l.vertices[i].name << '\t';
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
    int top = -1, count = 0;         // top Ϊջ��Ԫ�ص�������count ������������������Ѿ������˶���Ԫ��
    int *inDegree = new int[vexNum]; // ��������ʼ�� inDegree ����
    for (int i = 0; i < vexNum; i++)
        inDegree[i] = vertices[i].inDegree;
    for (int i = 0; i < vexNum; i++)
    {
        if (inDegree[i] == 0)
        {
            // �����ǰ��������Ϊ 0��������ջ
            inDegree[i] = top;
            top = i;
        }
    }
    if (topArray)
        delete[] topArray;      // ���ԭ���Ľ������
    topArray = new int[vexNum]; // �����µĽ������
    while (top != -1)           // ֻҪջ��Ϊ�գ���һֱѭ��
    {
        topArray[count++] = getName(top); // ȡ��ջ��Ԫ�أ�����������
        int v = top;
        top = inDegree[top]; // ����ջ��Ԫ��
        for (int i = firstAdjVex(v); i != -1; i = nextAdjVex(v, i))
        {
            if (--inDegree[i] == 0)
            {
                // �����ǰ��������Ϊ 0��������ջ
                inDegree[i] = top;
                top = i;
            }
        }
    }
    delete[] inDegree; // �ͷ� inDegree ������ڴ�
    if (count != vexNum)
        return false; // ����������ĳ��Ȳ�����ͼ�ж����������˵��ͼ���л�����������ʧ��
    else
        return true; // ����������ĳ��ȵ���ͼ�ж��������������������ɹ�
}

class AOE
{
private:
    adjDirList project; // ��Ŀͼ
    int *topArray;      // ��������������
    int time;           // ������ʱ��

public:
    AOE() : topArray(nullptr), time(0){}; // Ĭ�Ϲ��캯��
    ~AOE();                               // �����������ͷ��ڴ�
    void build_AOE();                     // ����AOE����
    void output_AOE();                    // ���AOE����
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
        cout << "���������񽻽ӵ�����(1-100)������������:";
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
        cout << "�������" << count + 1 << "��������Ŀ�ʼ���ӵ�,��ɽ��ӵ�,�Լ������������ʱ��:";
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
        for (int i = 0; i < incidents; i++) // ������������˳�����Ve
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
        for (int i = incidents - 2; i >= 0; i--) // �����������������Vl
        {
            for (int k = project.firstAdjVex(topArray[i] - 1); k != -1; k = project.nextAdjVex(topArray[i] - 1, k))
            {
                int w = project.getWeight(topArray[i] - 1, k);
                if (vl[k] - w < vl[topArray[i] - 1])
                    vl[topArray[i] - 1] = vl[k] - w;
            }
        }

        // ���ʱ�伴Ϊ�����������һ�����ӵ�ĵ����翪ʼʱ��
        time = ve[incidents - 1];
        cout << time << endl; // output the time;

        int cnt = 0;
        for (int i = 0; i < incidents; i++)
        {
            for (int j = project.firstAdjVex(topArray[i] - 1); j != -1; j = project.nextAdjVex(topArray[i] - 1, j))
            {
                int w = project.getWeight(topArray[i] - 1, j);
                // ����ae��al
                ae = ve[topArray[i] - 1];
                al = vl[j] - w;
                if (ae == al) // ���ae==al����˵��Ϊ�ؼ��
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