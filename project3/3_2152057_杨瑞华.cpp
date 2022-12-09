#include <iostream>
#include <Windows.h>
using namespace std;

#define ADD_LENGTH 50 // the vector adding capacity

template <class T>
class Vector
{
private:
    T* elements = nullptr;
    int maxSize = 0;
    int length = maxSize;
    void setMaxSize(int size) { maxSize = size; };
    void overSize();

public:
    Vector() : maxSize(0), length(0), elements(nullptr) {};
    Vector(int size) : maxSize(size), length(size)
    {
        if (size > 0)
        {
            elements = new T[size];
            if (!elements)
                std::cerr << "wrong memeory allocation!\n";
        }
        else
            elements = nullptr;
    };
    Vector(int size, T elem) : maxSize(size), length(size)
    {
        if (size > 0)
        {
            elements = new T[size];
            if (!elements)
                std::cerr << "wrong memeory allocation!\n";
            for (int i = 0; i < size; i++)
                elements[i] = elem;
        }
        else
            elements = nullptr;
    };
    Vector(const Vector<T>& v) : elements(v.elements), maxSize(v.maxSize), length(v.length) {};
    ~Vector() { delete[] elements; };
    void pushBack(const T& elem);
    int getLength() const { return length; };
    void insert(int i, const T& elem);
    T remove(int i);
    void reMove(const T& aim);
    T minElem();
    T maxElem();
    T& operator[](int i) const;
    int find(const T& elem) const;
    T sum();
    void makeEmpty();
};

template <class T>
class priorityQueue
{
private:
    int size;				// the size of the queue
    Vector<T> heap;			// to store the data
    void filterUp(int i);	// to maintain the minHeap from bottle to top
    void filterDown(int i); // to maintain the minHeap from top to bottle
    T popMin();				// pop the minimal elem
    int parent(int i);		// return the position of parent
    int left(int i);		// return the position of left child
    int right(int i);		// return the posision of right child
    void swap(T& a, T& b);	// swap the value of a,b
public:
    priorityQueue();
    ~priorityQueue();
    bool empty();	   // judge whether the queue is empty
    T pop();		   // out of the queue
    void push(T elem); // into the queue
};

template <class T>
priorityQueue<T>::priorityQueue()
{
    size = 0;
}

template <class T>
priorityQueue<T>::~priorityQueue()
{
}

template <class T>
void priorityQueue<T>::swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template <class T>
int priorityQueue<T>::parent(int i)
{
    return (i - 1) / 2;
}

template <class T>
int priorityQueue<T>::left(int i)
{
    return 2 * i + 1;
}

template <class T>
int priorityQueue<T>::right(int i)
{
    return 2 * i + 2;
}

template <class T>
bool priorityQueue<T>::empty()
{
    return size == 0;
}

template <class T>
T priorityQueue<T>::pop()
{
    return popMin();
}

template <class T>
void priorityQueue<T>::push(T elem)
{
    heap.pushBack(elem);
    size = heap.getLength();
    filterUp(0);
}

template <class T>
void priorityQueue<T>::filterUp(int i)
{
    if (heap.getLength())
    {
        int k = size - 1;
        int j = parent(k);
        T temp = heap[k];
        while (k > i)
        {
            if (heap[k] >= heap[j])
                break;
            else
            {
                heap[k] = heap[j];
                k = j;
                j = parent(j);
            }
            heap[k] = temp;
        }
    }
}

template <class T>
inline void priorityQueue<T>::filterDown(int i)
{
    if (heap.getLength())
    {
        int j = left(i);
        T temp = heap[i];
        while (j <= size - 1)
        {
            if (j < size - 1 && heap[j] > heap[j + 1])
                j++;
            if (temp <= heap[j])
                break;
            else
            {
                heap[i] = heap[j];
                i = j;
                j = 2 * j + 1;
            }
            heap[i] = temp;
        }
    }
}

template <class T>
T priorityQueue<T>::popMin()
{
    if (empty())
    {
        std::cerr << "the queue is empty!\n";
    }
    else
    {
        T top = heap[0];
        heap[0] = heap.remove(size);
        size = heap.getLength();
        filterDown(0);
        return top;
    }
}

template <class T>
inline void Vector<T>::overSize()
{
    maxSize += ADD_LENGTH;
    T* newArray = new T[maxSize];
    if (!newArray)
        std::cerr << "wrong memory allocation!\n";
    for (int i = 0; i < maxSize - ADD_LENGTH; i++)
        newArray[i] = elements[i];
    delete[] elements;
    elements = newArray;
}

template <class T>
inline void Vector<T>::pushBack(const T& elem)
{
    if (elements == nullptr)
    {
        elements = new T;
        maxSize = 1;
        length = maxSize;
        elements[0] = elem;
    }
    else
    {
        if (length >= maxSize)
            overSize();
        elements[length] = elem;
        length++;
    }
}

template <class T>
inline void Vector<T>::insert(int i, const T& elem)
{
    if (elements == nullptr)
    {
        elements = new T;
        maxSize = 1;
        length = maxSize;
        elements[0] = elem;
    }
    else
    {
        if (length >= maxSize)
            overSize();
        length++;
        for (int k = 0; k < length - i; k++)
            elements[length - k - 1] = elements[length - k - 2];
        elements[i - 1] = elem;
    }
}

template <class T>
inline T Vector<T>::remove(int i)
{
    T x;
    length--;
    if (i > length)
        x = elements[length];
    else
    {
        x = elements[i - 1];
        for (int k = i - 1; k < length; k++)
            elements[k] = elements[k + 1];
    }
    return x;
}

template <class T>
inline void Vector<T>::reMove(const T& aim)
{
    int findResult = find(aim);
    bool flag = findResult > -1 ? true : false;
    if (flag)
        remove(findResult + 1);
    else
        std::cerr << "no remove target!\n";
}

template <class T>
inline T Vector<T>::minElem()
{
    T min = elements[0];
    for (int i = 0; i < length; i++)
    {
        if (elements[i] < min)
            min = elements[i];
    }
    return min;
}

template <class T>
inline T Vector<T>::maxElem()
{
    T max = elements[0];
    for (int i = 0; i < length; i++)
        if (elements[i] > max)
            max = elements[i];
    return max;
}

template <class T>
inline T& Vector<T>::operator[](int i) const
{
    return elements[i];
}

template <class T>
inline int Vector<T>::find(const T& aim) const
{
    int pos = -1;
    for (int i = 0; i < length; i++)
    {
        if (elements[i] == aim)
        {
            pos = i;
            break;
        }
    }
    return pos;
}

template <class T>
inline T Vector<T>::sum()
{
    T sum = 0;
    for (int i = 0; i < length; i++)
        sum += elements[i];
    return sum;
}

template <class T>
void Vector<T>::makeEmpty()
{
    length = 0;
}

// A*�㷨���״̬ö����
enum class status
{
    OPEN,
    CLOSED,
    UNVISIT
};

//������ö����
enum class category
{
    WALL,
    PATH
};

class grid //�Թ������
{
private:
    int row;				//��
    int col;				//��
    int F = 0;				// A*�㷨����ֵ��F=G+H
    int G = 0;				//��㵽�õ���ƶ�����
    int H = 0;				//�õ㵽�յ�Ĺ���ɱ�
    grid* parent = nullptr; //�ø��ĸ��׸��Ŀ¼
public:
    grid() : row(0), col(0) {};
    grid(int r, int c) : row(r), col(c) {};
    void set_rc(int r, int c) { row = r, col = c; };
    int getF() const { return F; };
    int getG() const { return G; };
    int getH() const { return H; };
    int getRow() const { return row; };
    int getCol() const { return col; };
    grid* getParent() const { return parent; };
    void setF(int f) { F = f; };
    void setG(int g) { G = g; };
    void setH(int h) { H = h; };
    void setParent(grid* p) { parent = p; };
    friend ostream& operator<<(ostream& out, grid a);
    bool operator<=(const grid& other) const;
    bool operator>=(const grid& other) const;
    bool operator>(const grid& other) const;
    bool operator<(const grid& other) const;
    void operator=(const grid& other);
};

//�Թ���
class maze
{
private:
    int row;		//����
    int col;		//����
    grid** m_maze;	//�洢�Թ��Ķ�άָ��
    status** tags;	//�洢�Թ����A*�㷨״̬
    category** cgy; //�洢�Թ�������

    /*�������ܣ�����A*�㷨Gֵ
      �����������ǰ����У���ǰ����У���ʼ����У���ʼ�����
      ����ֵ��  ����õ���G*/
    int calculateG(int c_row, int c_col, int s_row, int s_col);

    /*�������ܣ�����A*�㷨Hֵ
      �����������ǰ����У���ǰ����У��յ��У��յ���
      ����ֵ��  ����õ���Hֵ*/
    int calculateH(int c_row, int c_col, int a_row, int a_col);

    /*�������ܣ�����A*�㷨Fֵ
      �����������ǰ����У���ǰ�����
      ����ֵ��  ����õ���Fֵ*/
    int calculateF(int c_row, int c_col);

    /*�������ܣ���������openList
      ���������Ҫ�����openList�ĸ���������*/
    void addOpenList(int c_row, int c_col);

    /*�������ܣ���������closedList
      ���������Ҫ�����closedList�ĸ���������*/
    void addClosedList(int c_row, int c_col);

    /*�������ܣ�A*�㷨�Ե�ǰ������ڴ���
      ���������status:ָ�����ڸ����Ե�ǰ���λ��
              ��ǰ����������
              �յ��������
              openList��С��*/
    void dealAdjGrid(int status, int s_row, int s_col, int a_row, int a_col, priorityQueue<grid>& openQueue);

public:
    maze(int _row = 0, int _col = 0, category** _cgy = nullptr);
    ~maze();

    /*�������ܣ���A*�㷨�����յ�
      �����������ʼ��������꣬�յ��������*/
    void A_star(int s_row, int s_col, int a_row, int a_col);
    /*�������ܣ�����Թ�·��
      ����������յ�����꣬�յ�������*/
    void outPath(int a_row, int a_col);
    category** getCgy() const { return cgy; };
};

maze::maze(int _row, int _col, category** _cgy) : row(_row), col(_col)
{
    if ((row || col) && (row != 1 && col != 1))
    {
        m_maze = new grid * [row];
        for (int i = 0; i < row; ++i)
            m_maze[i] = new grid[col];

        tags = new status * [row];
        for (int i = 0; i < row; ++i)
            tags[i] = new status[col];

        cgy = new category * [row];
        for (int i = 0; i < row; ++i)
            cgy[i] = new category[col];

        //�Թ���ʼ��
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j)
            {
                m_maze[i][j].set_rc(i + 1, j + 1);
                tags[i][j] = status::UNVISIT;
                if (_cgy)
                    cgy[i][j] = _cgy[i][j];
            }
    }
}

maze::~maze()
{
    if (m_maze)
    {
        for (int i = 0; i < row; ++i)
            delete[] m_maze[i];
    }
    if (tags)
    {
        for (int i = 0; i < row; ++i)
            delete[] tags[i];
    }
    if (cgy)
    {
        for (int i = 0; i < row; ++i)
            delete[] cgy[i];
    }
}

int maze::calculateG(int c_row, int c_col, int s_row, int s_col)
{
    //�ٶ������������ƶ����۾�Ϊ10
    return m_maze[s_row - 1][s_col - 1].getG() + 10;
}

int maze::calculateH(int c_row, int c_col, int a_row, int a_col)
{
    //���øø�㵽�յ��ֱ�߾����ƽ��ΪHֵ
    return (a_row - c_row) * (a_row - c_row) + (a_col - c_col) * (a_col - c_col);
}

int maze::calculateF(int c_row, int c_col)
{
    // F=G+H
    return m_maze[c_row - 1][c_col - 1].getG() + m_maze[c_row - 1][c_col - 1].getH();
}

void maze::addOpenList(int c_row, int c_col)
{
    tags[c_row - 1][c_col - 1] = status::OPEN;
}

void maze::addClosedList(int c_row, int c_col)
{
    tags[c_row - 1][c_col - 1] = status::CLOSED;
}

void maze::dealAdjGrid(int status, int s_row, int s_col, int a_row, int a_col, priorityQueue<grid>& openQueue)
{
    //����status�������ڸ���������
    int adjRow = s_row, adjCol = s_col;
    if (status == 1)
        adjRow = s_row - 1;
    else if (status == 2)
        adjCol = s_col - 1;
    else if (status == 3)
        adjRow = s_row + 1;
    else if (status == 4)
        adjCol = s_col + 1;
    else
    {
        cerr << "����ķ���״̬" << endl;
        return;
    }

    //����ǽ���Ҳ���closedList��
    if (cgy[adjRow - 1][adjCol - 1] != category::WALL && tags[adjRow - 1][adjCol - 1] != status::CLOSED)
    {
        int lastG = m_maze[adjRow - 1][adjCol - 1].getG();      //�洢֮ǰ�Ѽ���Gֵ
        m_maze[adjRow - 1][adjCol - 1].setG(calculateG(adjRow, adjCol, s_row, s_col));
        if (tags[adjRow - 1][adjCol - 1] == status::OPEN)       //������ڵ�����openList��
        {
            if (m_maze[adjRow - 1][adjCol - 1].getG() < lastG)     //�������GֵС��ԭ��Gֵ
            {
                m_maze[adjRow - 1][adjCol - 1].setParent(&m_maze[s_row - 1][s_col - 1]);    //�������ڵ㸸��Ϊ��ǰ�ڵ�
                openQueue.push(m_maze[adjRow - 1][adjCol - 1]);   //������С��
            }
            else
                m_maze[adjRow - 1][adjCol - 1].setG(lastG);     //������openList�������ϴ�Gֵ
        }

        //����H��F
        m_maze[adjRow - 1][adjCol - 1].setH(calculateH(adjRow, adjCol, a_row, a_col));
        m_maze[adjRow - 1][adjCol - 1].setF(calculateF(adjRow, adjCol));
        if (tags[adjRow - 1][adjCol - 1] != status::OPEN)   //������openList������뵽openList��
        {
            addOpenList(adjRow, adjCol);
            m_maze[adjRow - 1][adjCol - 1].setParent(&m_maze[s_row - 1][s_col - 1]);
            openQueue.push(m_maze[adjRow - 1][adjCol - 1]);
        }
    }
}

void maze::A_star(int s_row, int s_col, int a_row, int a_col)
{
    priorityQueue<grid> openQueue;
    while (1)
    {
        addClosedList(s_row, s_col);
        if (tags[a_row - 1][a_col - 1] == status::CLOSED)
            break;
        dealAdjGrid(1, s_row, s_col, a_row, a_col, openQueue);
        dealAdjGrid(2, s_row, s_col, a_row, a_col, openQueue);
        dealAdjGrid(3, s_row, s_col, a_row, a_col, openQueue);
        dealAdjGrid(4, s_row, s_col, a_row, a_col, openQueue);
        grid best = openQueue.pop();
        s_row = best.getRow();
        s_col = best.getCol();
    }
}

void maze::outPath(int a_row, int a_col)
{
    cout << endl
         << "�Թ�·��:" << endl;
    Vector<grid> path;

    while (1)
    {
        //���յ������Ѱ�Ҹ��׸�㣬һֱ�ҵ���ʼ�㼴Ϊ���·��
        grid* p = m_maze[a_row - 1][a_col - 1].getParent();
        path.pushBack(m_maze[a_row - 1][a_col - 1]);
        if (p == nullptr)
            break;
        a_row = p->getRow();
        a_col = p->getCol();
    }

    int len = path.getLength();
    //�������
    for (int i = len; i > 0; --i)
    {
        if (i == 1)
            cout << path[i - 1];
        else
            cout << path[i - 1] << " ---> ";
    }
    cout << endl;
}

ostream& operator<<(ostream& out, grid a)
{
    out << "(" << a.row << "," << a.col << ")";
    return out;
}

bool grid::operator<=(const grid& other) const
{
    return F <= other.F;
}

bool grid::operator<(const grid& other) const
{
    return F < other.F;
}

bool grid::operator>(const grid& other) const
{
    return F > other.F;
}

bool grid::operator>=(const grid& other) const
{
    return F >= other.F;
}

void grid::operator=(const grid& other)
{
    row = other.row;
    col = other.col;
    F = other.F;
    G = other.G;
    H = other.H;
    parent = other.parent;
}

int main()
{
    category** c;
    c = new category * [7];
    for (int i = 0; i < 7; ++i)
        c[i] = new category[7];
    int map[7][7] = { 0, 0, 0, 0, 0, 0, 0,
                      0, 1, 0, 1, 0, 0, 0,
                      0, 1, 1, 1, 1, 1, 0,
                      0, 0, 1, 1, 1, 0, 0,
                      0, 1, 1, 1, 1, 0, 0,
                      0, 1, 1, 0, 1, 1, 0,
                      0, 0, 0, 0, 0, 0, 0 };
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j)
        {
            if (map[i][j] == 0)
                c[i][j] = category::WALL;
            else if (map[i][j] == 1)
                c[i][j] = category::PATH;
        }
    maze m(7, 7, c);
    cout << "�Թ���ͼ:" << endl;
    cout << "        ";
    for (int i = 0; i < 7; i++)
    {
        cout << i + 1 << "��\t";
    }
    cout << endl;
    for (int i = 0; i < 7; i++)
    {
        cout << i + 1 << "��\t";
        for (int j = 0; j < 7; j++)
        {
            if (m.getCgy()[i][j] == category::WALL)
                cout << "#";
            else
                cout << "0";
            cout << '\t';
        }
        cout << endl;
    }
    m.A_star(2, 2, 6, 6);
    m.outPath(6, 6);
    cout << endl;

    if(c)
    {
        for (int i = 0; i < 7; ++i)
            delete c[i];
    }

    system("pause");
    return 0;
}