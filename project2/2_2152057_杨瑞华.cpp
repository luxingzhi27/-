#include<iostream>
using namespace std;

template<class T>
class listNode
{
public:
	T val;    //节点数据
	listNode<T>* next;  //下一节点
};

template<class T>
class list :public listNode<T>
{
	//重载输入输出运算符
	template <class S>
	friend ostream& operator<<(ostream& out, list<S> x);
	template <class Q>
	friend istream& operator>>(istream& in, list<Q>& x);
private:
	listNode<T>* first;
public:
	list();       //构造函数
	~list();      //析构函数
	bool empty() const;   //链表是否为空
	int getLength()const;  //获取链表长度
	listNode<T>* find(T elem);   //找到指定元素
	bool insert(int i, T& elem);    //插入指定位置元素
	bool remove(int i, T& elem);    //删除指定位置元素
	bool pushBack(T& elem);    //在末尾加入元素
	void display() const;      //输出链表
	listNode<T>* getFirst() { return first; };    //返回首个节点
};


template<class T>
inline list<T>::list()
{
	this->first = nullptr;      //初始构造为空链表
}

template<class T>
inline list<T>::~list()
{
	if (first)
	{
		listNode<T>* p = first, * q = nullptr;
		while (p != nullptr)
		{
			q = p->next;
			delete p;
			p = q;
		}
	}
}

template<class T>
bool list<T>::empty() const
{
	if (first == nullptr)
		return true;
	return false;
}

template<class T>
int list<T>::getLength() const
{
	listNode<T>* tmp = this->first;
	int length = 0;
	while (tmp)
	{
		length++;
		tmp = tmp->next;
	}
	return length;
}

template<class T>
listNode<T>* list<T>::find(T elem)
{
	listNode<T>* tmp = this->first;
	while (tmp)
	{
		if (tmp->val == elem)
			break;
		tmp = tmp->next;
	}
	return tmp;
}


template<class T>
bool list<T>::insert(int i, T& elem)
{
	if (i == 0 || first == nullptr)           //如果在头部插入或链表为空
	{
		listNode<T>* newNode = new listNode<T>;
		if (!newNode)
		{
			cerr << "Error allocating memory!" << endl;
			return false;
		}
		newNode->val = elem;
		newNode->next = first;
		first = newNode;
	}
	else                                   //在中间或尾部插入
	{
		listNode<T>* current = first;
		for (int k = 1; k < i; k++)      //首先遍历至要插入位置
		{
			if (current == nullptr)
				break;
			else
				current = current->next;
		}
		if (current == nullptr)
		{
			cerr << "invalid insertion postion!" << endl;
			return false;
		}
		else
		{
			listNode<T>* newNode = new listNode<T>;
			if (!newNode)
			{
				cerr << "Error allocating memory!" << endl;
				return false;
			}

			//插入链表之中
			newNode->next = current->next;
			newNode->val = elem;
			current->next = newNode;
		}
	}
	return true;
}

template<class T>
bool list<T>::remove(int i, T& elem)
{
	listNode<T>* del, * current;
	if (i == 1)        //如果移除第一个
	{
		del = first;
		first = first->next;
	}
	else             //移除中间或末尾
	{
		current = first;
		for (int k = 1; k < i - 1; k++)    //遍历至要删除位置
			current = current->next;
		if (current == nullptr || current->next == nullptr || i < 1)
		{
			cerr << "invalid remove position!" << endl;
			return false;
		}
		del = current->next;
		current->next = del->next;
	}
	elem = del->val;
	delete del;   //删除
	return true;
}

template<class T>
bool list<T>::pushBack(T& elem)    //在末尾添加元素
{
	listNode<T>* newNode = new listNode<T>;
	if (!newNode)
	{
		cerr << "Error allocating memory!" << endl;
		return false;
	}

	listNode<T>* tmp = first;
	if (tmp)                   //如果头节点不空
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = newNode;
		newNode->next = nullptr;
		newNode->val = elem;
	}
	else      //链表为空则将first置为新节点的地址
	{
		first = newNode;
		first->next = nullptr;
		first->val = elem;
	}
	return true;
}

template<class T>
inline void list<T>::display() const
{
	listNode<T>* tmp = first;
	while (tmp)
	{
		cout << tmp->val << " ";
		tmp = tmp->next;
	}
}

template <class T>
ostream& operator<<(ostream& out, list<T> x)
{
	listNode<T>* tmp = x.first;
	if (tmp == nullptr)
		out << "NULL";
	else
	{
		while (tmp->next)
		{
			out << tmp->val << " ";
			tmp = tmp->next;
		}
		out << tmp->val;
	}
	return out;
}

template<class T>
inline istream& operator>>(istream& in, list<T>& x)
{
	int tmp = 0;
	while (in >> tmp)
	{
		if (tmp > 0)
			x.pushBack(tmp);
		else if (tmp == 0)
		{
			cerr << "illegal input!" << endl;
			break;
		}
		else
			break;
	}
	return in;
}

void numInput(int& n)
{
	while (1)
	{
		cin >> n;
		if (cin.fail())       //如果输入错误
		{
			cin.clear();     //改变输入状态
			cin.ignore(9999, '\n');    //清空缓冲区
			cerr << "输入有误，请重新输入" << endl;     //给出错误提示
		}
		else
			break;     //退出输入循环
	}
}


void listInput(list<int>& in)
{
	int n = 0, last = 0;
	while (1)
	{
		numInput(n);
		if (n <= 0)
			break;
		if (last > n)     //输入为降序
		{
			cerr << "输入不满足非降序要求，请重新输入出错数" << n << "及其之后数" << endl;
			cin.clear();
			cin.ignore(9999, '\n');
			continue;
		}
		last = n;
		in.pushBack(n);
	}
}

int main()
{
	cout << "请输入两组非降序正整数序列：" << endl;
	list<int>* list1 = new list<int>();
	list<int>* list2 = new list<int>();
	listInput(*list1);
	cout << "输入另一组：" << endl;
	listInput(*list2);

	listNode<int>* pb = list2->getFirst();
	listNode<int>* pa = list1->getFirst();
	listNode<int>* pc = new listNode<int>();   //指向pa前一个指针，方便删除
	listNode<int>* tmp = pc;
	pc->next = pa;
	pc->val = 0;

	while (pa != nullptr && pb != nullptr)
	{
		if (pa->val == pb->val)    //如果相等，保留
		{
			pa = pa->next;
			pb = pb->next;
			pc = pc->next;
		}
		else if (pa->val < pb->val)  //小于，将其删除
		{
			int elem = 0;
			if (pa == list1->getFirst())         //如果要删除的是头节点
			{
				list1->remove(1, elem);
				pa = list1->getFirst();
				pc->next = pa;
			}
			else
			{
				pc->next = pa->next;
				delete pa;
				pa = pc->next;
			}
		}
		else                      //大于，pb向后移动
			pb = pb->next;
	}
	while (pa != nullptr)         //将list1剩下的元素删除
	{
		int elem = 0;
		if (pa == list1->getFirst())      //如果要删除的是头节点
		{
			list1->remove(1, elem);
			pa = list1->getFirst();
			pc->next = pa;
		}
		else
		{
			pc->next = pa->next;
			delete pa;
			pa = pc->next;
		}
	}
	cout << "最后的交集为：";
	if (list1->getFirst())
		cout << *list1 << endl;
	else
		cout << "NULL" << endl;
	if (pc == tmp)
		delete pc;
	return 0;
}