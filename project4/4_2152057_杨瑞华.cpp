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
	T* elements;         //储存栈元素
	int top;             //栈顶元素指针
	int maxSize;         //栈的最大容量
	void overCapacity(); //栈满时扩容操作
public:
	Stack(int size = maxCapacity);
	~Stack();
	bool pop(T& tmp);    //出栈
	void push(T elem);   //进栈
	int getSize();       //得到栈的大小
	bool isEmpty();      //判断栈空否
	bool isFull();       //判断栈满否
	T getTop();          //获取栈顶元素
	bool reverse();      //将栈倒转
};

template<class T>
inline void Stack<T>::overCapacity()
{
	//当容量不足时扩充容量
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
	if (!isEmpty())       //如果栈不空，先出栈，栈顶指针再自减
	{
		tmp = elements[top--];
		return true;
	}
	else                   //栈空，返回false
		return false;
}

template<class T>
inline void Stack<T>::push(T elem)
{
	if (isFull())              //如果栈满则扩容
		overCapacity();
	elements[++top] = elem;   //栈顶指针先+1然后元素进栈
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

int isp(char x)      //栈内优先级
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

int icp(char ch)   //栈外优先级
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

bool isNum(const char* ch)     //是否是数字
{
	if (strlen(ch) == 1 && (ch[0] > '9' || ch[0] < '0'))    //长度为1且不在0-9之间返回false
		return false;
	else
		return true;
}




int main()
{
	cout << "请输入以空格分割的不同对象（运算符，操作数）组成的中缀表达式，可包含左右括号，以及加减乘除" << endl;
	Stack<const char*> expression;
	expression.push("#");
	//const char* in = read();
	char in[21];
	cin >> in;
	while (!expression.isEmpty())
	{
		if (isNum(in))    //如果是数字
		{
			if (in[0] == '+')    //如果有符号
				for (char* pin = in + 1; *pin; pin++)
					cout << *pin;      //直接输出
			else
				cout << in;     //是数字直接输出
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