#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<assert.h>
using namespace std;

const int addCapacity = 20;
const int maxCapacity = 50;

template<class T>
class Stack
{
private:
	T* elements;         //����ջԪ��
	int top;             //ջ��Ԫ��ָ��
	int maxSize;         //ջ���������
	void overCapacity(); //ջ��ʱ���ݲ���
public:
	Stack(int size = maxCapacity);
	~Stack();
	bool pop(T& tmp);    //��ջ
	void push(T elem);   //��ջ
	int getSize();       //�õ�ջ�Ĵ�С
	bool isEmpty();      //�ж�ջ�շ�
	bool isFull();       //�ж�ջ����
	T getTop();          //��ȡջ��Ԫ��
	bool reverse();      //��ջ��ת
};

template<class T>
inline void Stack<T>::overCapacity()
{
	//����������ʱ��������
	T* newArray = new T[maxCapacity + addCapacity];
	assert(newArray != nullptr);
	for (int i = 0; i <= top; i++)
		newArray[i] = elements[i];
	maxSize = maxCapacity + addCapacity;
	delete[]elements;
	elements = newArray;
}

template<class T>
inline Stack<T>::Stack(int size) :maxSize(size), top(-1)
{
	elements = new T[maxSize];
	assert(elements != nullptr);
}

template<class T>
inline Stack<T>::~Stack()
{
	delete[]elements;
}

template<class T>
inline bool Stack<T>::pop(T& tmp)
{
	if (!isEmpty())       //���ջ���գ��ȳ�ջ��ջ��ָ�����Լ�
	{
		tmp = elements[top--];
		return true;
	}
	else                   //ջ�գ�����false
		return false;
}

template<class T>
inline void Stack<T>::push(T elem)
{
	if (isFull())              //���ջ��������
		overCapacity();
	elements[++top] = elem;   //ջ��ָ����+1Ȼ��Ԫ�ؽ�ջ
}

template<class T>
inline int Stack<T>::getSize()
{
	return top + 1;
}

template<class T>
inline bool Stack<T>::isEmpty()
{
	return top > -1 ? false : true;
}

template<class T>
inline bool Stack<T>::isFull()
{
	return top + 1 >= maxSize ? true : false;
}

template<class T>
inline T Stack<T>::getTop()
{
	return elements[top];
}

template<class T>
inline bool Stack<T>::reverse()
{
	if (isEmpty())
		return false;
	else
	{
		T* tmp = new T[maxSize];
		assert(tmp != nullptr);
		char x = '0';
		int cnt = 0;
		while (!isEmpty())
		{
			pop(x);
			tmp[cnt] = x;
			cnt++;
		}
		delete[]elements;
		elements = tmp;
		top = cnt - 1;
		return true;
	}
}

int isp(char x)      //ջ�����ȼ�
{
	int i = -1;
	switch (x)
	{
	case '(':
		i = 1;
		break;
	case')':
		i = 6;
		break;
	case'*':
		i = 5;
		break;
	case'/':
		i = 5;
		break;
	case'+':
		i = 3;
		break;
	case'-':
		i = 3;
		break;
	case'#':
		i = 0;
		break;
	}
	return i;
}

int icp(char ch)   //ջ�����ȼ�
{
	int i = -1;
	switch (ch)
	{
	case '(':
		i = 6;
		break;
	case')':
		i = 1;
		break;
	case'*':
		i = 4;
		break;
	case'/':
		i = 4;
		break;
	case'+':
		i = 2;
		break;
	case'-':
		i = 2;
		break;
	case'#':
		i = 0;
		break;
	}
	return i;
}

bool isNum(const char* ch)     //�Ƿ�������
{
	if (strlen(ch) == 1 && (ch[0] > '9' || ch[0] < '0'))    //����Ϊ1�Ҳ���0-9֮�䷵��false
		return false;
	else
		return true;
}




int main()
{
	cout << "�������Կո�ָ�Ĳ�ͬ���������������������ɵ���׺���ʽ���ɰ����������ţ��Լ��Ӽ��˳�" << endl;
	Stack<const char*> expression;
	expression.push("#");
	//const char* in = read();
	char in[21];
	cin >> in;
	while (!expression.isEmpty())
	{
		if (isNum(in))    //���������
		{
			if (in[0] == '+')    //����з���
				for (char* pin = in + 1; *pin; pin++)
					cout << *pin;      //ֱ�����
			else
				cout << in;     //������ֱ�����
			cout << ' ';
			if (getchar() == '\n')
			{
				expression.push("#");
				in[0] = '#';
				in[1] = '\0';
			}
			else
				cin >> in;
		}
		else
		{
			if (icp(in[0]) > isp(expression.getTop()[0]))     //icp(ch)>isp(op)
			{
				char* i = new char[21];
				i[20] = '\0';
				char* pin = in, * pi = i;
				for (; *pi && *pin; pi++, pin++)
					*pi = *pin;
				*pi = '\0';
				expression.push(i);
				if (getchar() == '\n')
				{
					expression.push("#");
					in[0] = '#';
					in[1] = '\0';
				}
				else
					cin >> in;
			}
			else if (icp(in[0]) < isp(expression.getTop()[0]))       //icp(ch)<isp(op)
			{
				const char* x = nullptr;
				expression.pop(x);
				cout << x;
				cout << ' ';
			}
			else if (icp(in[0]) == isp(expression.getTop()[0]))     //icp(ch)==isp(op)
			{
				const char* x = nullptr;
				expression.pop(x);
				if (x[0] == '(')
				{
					if (getchar() == '\n')
					{
						expression.push("#");
						in[0] = '#';
						in[1] = '\0';
					}
					else
						cin >> in;
				}
			}
		}
	}
	cout << '\b' << endl;
}