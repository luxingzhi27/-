#include<iostream>
using namespace std;
const int MAXSIZE = 50;

template <class T>
class Queue
{
private:
	int maxSize = MAXSIZE;      //队列最大容量
	T* elements;                //储存队列数据
	int rear;                   //队尾指针
	int front;                  //队首指针
	bool createQueue(int size);   //创造队列
public:
	Queue();
	Queue(int size);
	~Queue();
	bool isFull() const;           //判断队列满否
	bool isEmpty() const;          //队列空否
	void makeEmpty();              //队列置空
	bool enQueue(const T& elem);   //进队
	bool deQueue(T& elem);         //出队
	T deQueue();
	int count(const T& elem)const;  //计算队列中某数据个数
	int getLength() const;          //获得队列长度
};

template<class T>
inline bool Queue<T>::createQueue(int size)
{
	elements = new T[size];
	if (!elements)
	{
		cerr << "Error allocating memory!" << endl;
		return false;
	}
	return true;
}

template<class T>
inline Queue<T>::Queue() :rear(0), front(0)
{
	createQueue(maxSize);
}

template<class T>
inline Queue<T>::Queue(int size) :rear(0), front(0)
{
	maxSize = size;
	createQueue(maxSize);
}

template<class T>
inline Queue<T>::~Queue()
{
	delete[]elements;
}

template<class T>
inline bool Queue<T>::isFull() const
{
	return (rear + 1) % maxSize == front;
}

template<class T>
inline bool Queue<T>::isEmpty() const
{
	return rear == front;
}

template<class T>
inline void Queue<T>::makeEmpty()
{
	rear = 0;
	front = 0;
}

template<class T>
inline bool Queue<T>::enQueue(const T& elem)
{
	if (isFull())
		return false;
	else
	{
		elements[rear] = elem;
		rear = (rear + 1) % maxSize;     //循环队列，加一后模最大容量
		return true;
	}
}

template<class T>
inline bool Queue<T>::deQueue(T& elem)
{
	if (isEmpty())
		return false;
	else
	{
		elem = elements[front];
		front = (front + 1) % maxSize;    //循环队列，加一后模最大容量
		return true;
	}
}

template<class T>
inline T Queue<T>::deQueue()
{
	if (isEmpty())
		cerr << "queue is empty!" << endl;
	else
	{
		T tmp = elements[front];
		front = (front + 1) % maxSize;
		return tmp;
	}
}

template<class T>
inline int Queue<T>::count(const T& elem) const
{
	int cnt = 0;
	for (int i = front;;)
	{
		if (i == rear)
			break;
		if (elements[i] == elem)
			cnt++;
		i = (i + 1) % maxSize;
	}
	return cnt;
}

template<class T>
inline int Queue<T>::getLength() const
{
	if (rear >= front)
		return rear - front;
	else
		return maxSize - (front - rear);
}

int main()
{
	int num = 0;
	cout << "请输入顾客总数（不超过1000,最小为2）以及顾客编号" << endl;
	while (1)
	{
		cin >> num;
		if (!cin || num < 1 || num>1000)
		{
			cin.clear();
			cin.ignore(9999, '\n');
			cerr << "人数错误，请重新输入" << endl;
		}
		else
			break;
	}
	Queue<int> evenCusNum(num + 1);
	Queue<int> oddCusNum(num + 1);
	for (int i = 0; i < num; i++)
	{
		int tmp = 0;
		while (1)
		{
			cin >> tmp;
			if (!cin || tmp < 1)
			{
				cin.clear();
				cin.ignore(9999, '\n');
				cerr << "编号错误，请重新输入" << endl;
			}
			else
				break;
		}
		if (tmp % 2 == 0)
			evenCusNum.enQueue(tmp);
		else
			oddCusNum.enQueue(tmp);
	}
	while (1)
	{
		//奇数队列有则输出，一次出两个
		if (oddCusNum.getLength() >= 2)
		{
			cout << oddCusNum.deQueue() << " ";
			cout << oddCusNum.deQueue() << " ";
		}
		else if (oddCusNum.getLength() == 1)
			cout << oddCusNum.deQueue() << " ";
		//偶数队列一次出一个
		if (!evenCusNum.isEmpty())
			cout << evenCusNum.deQueue() << " ";
		if (evenCusNum.isEmpty() && oddCusNum.isEmpty())
			break;
	}
	cout << '\b' << endl;
}