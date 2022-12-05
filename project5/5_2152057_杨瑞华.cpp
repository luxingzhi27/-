#include<iostream>
using namespace std;
const int MAXSIZE = 50;

template <class T>
class Queue
{
private:
	int maxSize = MAXSIZE;      //�����������
	T* elements;                //�����������
	int rear;                   //��βָ��
	int front;                  //����ָ��
	bool createQueue(int size);   //�������
public:
	Queue();
	Queue(int size);
	~Queue();
	bool isFull() const;           //�ж϶�������
	bool isEmpty() const;          //���пշ�
	void makeEmpty();              //�����ÿ�
	bool enQueue(const T& elem);   //����
	bool deQueue(T& elem);         //����
	T deQueue();
	int count(const T& elem)const;  //���������ĳ���ݸ���
	int getLength() const;          //��ö��г���
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
		rear = (rear + 1) % maxSize;     //ѭ�����У���һ��ģ�������
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
		front = (front + 1) % maxSize;    //ѭ�����У���һ��ģ�������
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
	cout << "������˿�������������1000,��СΪ2���Լ��˿ͱ��" << endl;
	while (1)
	{
		cin >> num;
		if (!cin || num < 1 || num>1000)
		{
			cin.clear();
			cin.ignore(9999, '\n');
			cerr << "������������������" << endl;
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
				cerr << "��Ŵ�������������" << endl;
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
		//�����������������һ�γ�����
		if (oddCusNum.getLength() >= 2)
		{
			cout << oddCusNum.deQueue() << " ";
			cout << oddCusNum.deQueue() << " ";
		}
		else if (oddCusNum.getLength() == 1)
			cout << oddCusNum.deQueue() << " ";
		//ż������һ�γ�һ��
		if (!evenCusNum.isEmpty())
			cout << evenCusNum.deQueue() << " ";
		if (evenCusNum.isEmpty() && oddCusNum.isEmpty())
			break;
	}
	cout << '\b' << endl;
}