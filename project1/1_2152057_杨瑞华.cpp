#include<iostream>
#include<cstring>
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
bool list<T>::pushBack(T& elem)
{
	listNode<T>* newNode = new listNode<T>;
	if (!newNode)
	{
		cerr << "Error allocating memory!" << endl;
		return false;
	}

	listNode<T>* tmp = first;
	if (tmp)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = newNode;
		newNode->next = nullptr;
		newNode->val = elem;
	}
	else
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


class stuInfo
{
public:
	int stuNum = 0;   //考号
	string stuName; //姓名
	string stuSex;  //性别
	int stuAge = 0;   //年龄
	string stuAim;  //报考目标
	//重载学生输入输出
	friend ostream& operator<<(ostream& out, stuInfo s);
	friend istream& operator>>(istream& in, stuInfo& s);
};

ostream& operator<<(ostream& out, stuInfo s)
{
	out << s.stuNum << '\t' << s.stuName << '\t' << s.stuSex << '\t' << s.stuAge << '\t' << s.stuAim << endl;
	return out;
}

istream& operator>>(istream& in, stuInfo& s)
{
	while (1)
	{
		in >> s.stuNum >> s.stuName >> s.stuSex >> s.stuAge >> s.stuAim;
		if (in.good())
			break;
		else
		{
			in.clear();
			in.ignore(9999, '\n');
			cout << "输入非法，请重新输入：" << endl;
		}
	}
	return in;
}

void display(list<stuInfo>& stuList, int num)
{
	if (num == 0)
		cout << "无考生信息。" << endl;
	else
	{
		cout << endl << "考号\t姓名\t性别\t年龄\t报考类别" << endl;
		listNode<stuInfo>* ptr = stuList.getFirst();

		for (int i = 0; i < num; i++)
		{
			cout << ptr->val;
			ptr = ptr->next;
		}
	}
}

int main()
{
	cout << "首先请建立考生信息系统！" << endl;
	int num = 0;
	while (1)
	{
		cout << "请输入考生人数：";
		cin >> num;
		if (!cin || num <= 0)
		{
			cout << "输入不合法，请重新输入！" << endl;
			cin.clear();
			cin.ignore(9999, '\n');
		}
		else
			break;
	}

	cout << "请依次输入考生的考号，姓名，性别，年龄及报考类别！" << endl;
	list<stuInfo> stuList;
	stuInfo tmp;
	for (int i = 0; i < num; i++)
	{
		cin >> tmp;
		stuList.pushBack(tmp);
	}

	display(stuList, num);

	cout << endl << "请选择您要进行的操作（1为插入，2为删除，3为查找，4为修改，5为统计，0为取消操作）" << endl;
	while (1)
	{
		cout << endl << "请选择您要进行的操作：";
		int choose = 0;
		cin >> choose;
		if (choose == 1)
		{
			while (1)
			{
				cout << endl << "请输入您要插入考生的位置：";
				int pos = 0;
				cin >> pos;
				if (pos<1 || pos>num + 1)
				{
					cout << "插入位置有误，请重新插入。" << endl;
					continue;
				}
				else
				{
					cout << endl << "请依次输入考生的考号，姓名，性别，年龄及报考类别！" << endl;
					stuInfo insertStu;
					cin >> insertStu;
					bool flagOfInsert = true;
					flagOfInsert = stuList.insert(pos - 1, insertStu);
					display(stuList, stuList.getLength());
					break;
				}
			}
		}
		else if (choose == 2)
		{
			while (1)
			{
				cout << endl << "请输入您要删除考生的考号：";
				int pos = 0;
				cin >> pos;
				listNode<stuInfo>* ptr = stuList.getFirst();
				int cnt = 1;
				while (ptr)      //通过考号寻找考生位置
				{
					if (ptr->val.stuNum == pos)
						break;
					ptr = ptr->next;
					cnt++;
				}
				if (!ptr)      //未找到考生
					cout << "无相关学生信息。" << endl;
				else
				{
					stuInfo reMoveStu;
					stuList.remove(cnt, reMoveStu);    //删除考生信息
					cout << "您删除的考生信息是：" << reMoveStu << endl;
					display(stuList, stuList.getLength());
					break;
				}
			}
		}
		else if (choose == 3)
		{
			while (1)
			{
				cout << endl << "请输入您要查找考生的考号：";
				int pos = 0;
				cin >> pos;
				listNode<stuInfo>* ptr = stuList.getFirst();
				while (ptr)
				{
					if (ptr->val.stuNum == pos)    //通过考号查找考生信息
						break;
					ptr = ptr->next;
				}
				if (!ptr)
					cout << "无相关学生信息。" << endl;    //未找到
				else
				{
					stuInfo findStu = ptr->val;   //找到后输出
					cout << endl << "您查找的考生信息是：" << endl << "考号\t姓名\t性别\t年龄\t报考类别" << endl << findStu << endl;
					break;
				}
			}
		}
		else if (choose == 4)
		{
			while (1)
			{
				cout << endl << "请输入您要修改考生的考号：";
				int pos = 0;
				cin >> pos;
				listNode<stuInfo>* ptr = stuList.getFirst();
				while (ptr)
				{
					if (ptr->val.stuNum == pos)   //通过考号寻找考生信息
						break;
					ptr = ptr->next;
				}
				if (!ptr)
					cout << "无相关学生信息。" << endl;
				else
				{
					cout << "请依次输入修改后考生的考号，姓名，性别，年龄及报考类别：" << endl;
					stuInfo modifyStu;
					cin >> modifyStu;   //找到后修改考生信息
					ptr->val = modifyStu;
					cout << "修改后考生信息如下：";
					display(stuList, stuList.getLength());
					break;
				}
			}
		}
		else if (choose == 5)
		{
			display(stuList, stuList.getLength());
		}
		else if (choose == 0)
		{
			return 0;
		}
	}
	return 0;
}