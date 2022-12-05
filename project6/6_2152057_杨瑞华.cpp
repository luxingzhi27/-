#include<ostream>
#include<iostream>
#include<cstring>
#include<Windows.h>
using namespace std;

template<class T>
class listNode
{
public:
	T val;
	listNode<T>* next;
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
	list();
	bool empty() const;
	int getLength()const;
	listNode<T>* find(T elem);
	bool insert(int i, T& elem);
	bool remove(int i, T& elem);
	bool pushBack(T& elem);
	void display() const;
	listNode<T>* getFirst() { return first; };
};

//Tree类声明
template <class T>
class Tree;

//节点类
template <class T>
class treeNode
{
	friend class Tree<T>;
private:
	T val;
	treeNode<T>* firstChild;
	treeNode<T>* nextSibling;
public:
	treeNode(treeNode<T>* fch = nullptr, treeNode<T>* ns = nullptr) :firstChild(fch), nextSibling(ns) {};
	treeNode(T data, treeNode<T>* fch = nullptr, treeNode<T>* ns = nullptr) :val(data), firstChild(fch), nextSibling(ns) {};
	void setVal(T data) { val = data; };
	T getVal()const { return val; };
};

//Tree类
template <class T>
class Tree
{
private:
	treeNode<T>* root;
	treeNode<T>* current;
	bool findTarget(treeNode<T>* root, T target);                //根据值寻找，使寻找值成为current
	bool findParent(treeNode<T>* start, treeNode<T>* aim);       //以start为根寻找aim的双亲
	list<treeNode<T>*>* getChildren(treeNode<T> parent);                 //得到parent节点下所有孩子,返回所有孩子的链表
	treeNode<T>* getChildByVal(treeNode<T>* parent, T val);             //以值找孩子
	treeNode<T>* insertByVal(treeNode<T>* parent, T val);               //通过值在parent下插入孩子
	void reMove(treeNode<T>* parent);
public:
	Tree();
	Tree(T val, treeNode<T>* fch = nullptr, treeNode<T>* ns = nullptr);
	~Tree() { reMove(root); };
	void removeChlidren(treeNode<T>* parent);                          //移除parent节点下所有孩子
	void insertChildren(treeNode<T>* parent, int childNum, list<T>* childrenVal);            //在parent节点下添加一定数量孩子
	bool isEmpty() { return root == nullptr; };                          //判断是否是空树
	bool find(T target);                                                 //找到目标，使之成为当前节点
	bool parent();
	list<treeNode<T>*>* children();
	bool childByVal(T val);
	bool insert(T val);
	treeNode<T>* getRoot() const { return root; };
	treeNode<T>* getCurrent() const { return current; };
	void outChidren(ostream& out);
};

template<class T>
inline list<T>::list()
{
	this->first = nullptr;      //初始构造为空链表
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
		for (int k = 1; k < i; k++)
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
		for (int k = 1; k < i - 1; k++)
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
	delete del;
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

template<class T>
inline void Tree<T>::insertChildren(treeNode<T>* parent, int childNum, list<T>* childrenVal)
{
	listNode<T>* pval = childrenVal->getFirst();
	if (!parent->firstChild)
	{
		treeNode<T>* newNode = new treeNode<T>;
		parent->firstChild = newNode;
		parent->firstChild->setVal(pval->val);
		pval = pval->next;
		childNum--;
	}
	treeNode<T>* lastCh = parent->firstChild;
	while (lastCh->nextSibling)
		lastCh = lastCh->nextSibling;
	for (int i = 0; i < childNum; i++)
	{
		treeNode<T>* newNode = new treeNode<T>;
		newNode->setVal(pval->val);
		pval = pval->next;
		lastCh->nextSibling = newNode;
		lastCh = lastCh->nextSibling;
	}
}

template<class T>
inline bool Tree<T>::find(T target)
{
	if (isEmpty())
		return false;
	else
		return findTarget(root, target);
}

template<class T>
inline bool Tree<T>::parent()
{
	if (current == nullptr || current == root)
	{
		current = nullptr;
		return false;
	}
	treeNode<T>* p = current;
	treeNode<T>* q = root;
	return findParent(q, p);
}

template<class T>
inline list<treeNode<T>*>* Tree<T>::children()
{
	if (current == nullptr)
		return nullptr;
	else
		return getChildren(current);
}

template<class T>
inline bool Tree<T>::childByVal(T val)
{
	if (current == nullptr)
		return false;
	else
	{
		treeNode<T>* p = getChildByVal(current, val);
		if (p)
		{
			current = p;
			return true;
		}
		else
			return false;
	}
}

template<class T>
inline bool Tree<T>::insert(T val)
{
	if (current == nullptr)
		return false;
	else
	{
		treeNode<T>* p = insertByVal(current, val);
		if (p)
		{
			current = p;
			return true;
		}
		else
			return false;
	}
}

template<class T>
inline void Tree<T>::outChidren(ostream& out)
{
	treeNode<T>* p = current->firstChild;
	while (p)
	{
		out << p->val << " ";
		p = p->nextSibling;
	}
	out << '\b';
}


template<class T>
inline treeNode<T>* Tree<T>::getChildByVal(treeNode<T>* parent, T val)
{
	if (parent == nullptr)
		return nullptr;
	else
	{
		treeNode<T>* p = parent->firstChild;
		while (p)
		{
			if (p->val == val)
				return p;
			p = p->nextSibling;
		}
	}
}

template<class T>
inline treeNode<T>* Tree<T>::insertByVal(treeNode<T>* parent, T val)
{
	if (parent == nullptr)
		return nullptr;
	else
	{
		treeNode<T>* p = parent->firstChild;
		treeNode<T>* q = new treeNode<T>;
		q->setVal(val);
		if (p == nullptr)
			parent->firstChild = q;
		else
		{
			while (p->nextSibling)
				p = p->nextSibling;
			p->nextSibling = q;
			return q;
		}
	}
}

template<class T>
inline void Tree<T>::reMove(treeNode<T>* parent)
{
	if (parent)
	{
		reMove(parent->firstChild);
		reMove(parent->nextSibling);
		delete parent;
	}
}

template<class T>
inline bool Tree<T>::findTarget(treeNode<T>* root, T target)
{
	bool result = false;
	if (root->val == target)
	{
		result = true;
		current = root;
	}
	else
	{
		treeNode<T>* p = root->firstChild;
		while (p)
		{
			result = findTarget(p, target);
			if (result == true)
				break;
			p = p->nextSibling;
		}
	}
	return result;
}

template<class T>
inline bool Tree<T>::findParent(treeNode<T>* start, treeNode<T>* aim)
{
	bool result = false;
	treeNode<T>* p = start->firstChild;
	while (p && p != aim)
	{
		if (findParent(p, aim))
			return true;
		p = p->nextSibling;
	}
	if (p && p == aim)
	{
		current = start;
		result = true;
	}
	return result;
}

template<class T>
inline list<treeNode<T>*>* Tree<T>::getChildren(treeNode<T> parent)
{
	if (parent == nullptr)
		return nullptr;
	else
	{
		treeNode<T>* p = current->firstChild;
		list<treeNode<T>>* children;
		while (p)
		{
			children->pushBack(p);
			p = p->nextSibling;
		}
	}
	return children;
}

template<class T>
inline Tree<T>::Tree()
{
	treeNode<T>* p = new treeNode<T>;
	root = current = p;
	root->firstChild = nullptr;
	root->nextSibling = nullptr;
}

template <class T>
inline Tree<T>::Tree(T val, treeNode<T>* fch, treeNode<T>* ns)
{
	treeNode<T>* p = new treeNode<T>;
	root = current = p;
	root->val = val;
	root->firstChild = fch;
	root->nextSibling = ns;
}

template<class T>
inline void Tree<T>::removeChlidren(treeNode<T>* parent)
{
	if (parent)
	{
		treeNode<T>* fc = parent->firstChild;
		parent->firstChild = nullptr;
		reMove(fc);
	}
}

Tree<string> genealogy;


void buildFamily()
{
	cout << "请输入要建立家庭人的姓名：";
	string parent;
	cin >> parent;
	if (genealogy.find(parent))
	{
		cout << "请输入" << parent << "的儿女人数：";
		int childrenNum = 0;
		cin >> childrenNum;
		cout << "请依次输入" << parent << "的儿女的姓名：";
		list<string> childrenNames;
		string chName;
		for (int i = 0; i < childrenNum; i++)
		{
			cin >> chName;
			childrenNames.pushBack(chName);
		}
		genealogy.insertChildren(genealogy.getCurrent(), childrenNum, &childrenNames);
		cout << parent << "的第一代子孙是：";
		genealogy.outChidren(cout);
	}
	else
		cout << "无此家庭成员";
	cout << endl;
}

void removeFamily()
{
	cout << "请输入要解散家庭人的姓名：";
	string parent;
	cin >> parent;
	if (genealogy.find(parent))
	{
		cout << "要解散家庭人的姓名是：" << parent << endl;
		cout << parent << "的待解散子女为：";
		genealogy.outChidren(cout);
		genealogy.removeChlidren(genealogy.getCurrent());
	}
	else
		cout << "无此家庭成员";
	cout << endl;
}

void addChild()
{
	cout << "请输入要添加儿子（或女儿）的人的姓名：";
	string parent;
	cin >> parent;
	if (genealogy.find(parent))
	{
		cout << "请输入" << parent << "新添加的儿子或女儿的姓名：";
		string child;
		cin >> child;
		genealogy.insert(child);
		genealogy.find(parent);
		cout << parent << "的第一代子孙是：";
		genealogy.outChidren(cout);
	}
	else
		cout << "无此家庭成员";
	cout << endl;
}

void modifyChild()
{
	cout << "请输入要更改姓名的人的目前姓名：";
	string lastName;
	cin >> lastName;
	if (genealogy.find(lastName))
	{
		cout << "请输入更改后的姓名：";
		string nowName;
		cin >> nowName;
		genealogy.getCurrent()->setVal(nowName);
		cout << lastName << "已改名为" << nowName;
	}
	else
		cout << "无此家庭成员";
	cout << endl;
}

int main()
{
	cout << "**\t家谱管理系统           \t**\n";
	cout << "==================================\n";
	cout << "**\t请选择要执行的操作：   \t**\n";
	cout << "**\tA --- 完善家谱         \t**\n";
	cout << "**\tB --- 添加家庭成员     \t**\n";
	cout << "**\tC --- 解散局部家庭     \t**\n";
	cout << "**\tD --- 更改家庭成员姓名 \t**\n";
	cout << "**\tE --- 退出程序         \t**\n";
	cout << "==================================\n";
	cout << "首先建立一个家谱！" << endl;
	cout << "请输入祖先的姓名：";
	string pName;
	cin >> pName;
	treeNode<string>* root = genealogy.getRoot();
	root->setVal(pName);
	cout << "此家谱的祖先是：" << root->getVal() << endl;
	while (1)
	{
		cout << "\n请选择要执行的操作：";
		char op;
		cin >> op;
		cin.ignore(999999, '\n');
		bool flag = false;
		switch (op)
		{
		case 'a':
		case 'A':
			buildFamily();
			break;
		case 'b':
		case 'B':
			addChild();
			break;
		case 'c':
		case 'C':
			removeFamily();
			break;
		case 'd':
		case 'D':
			modifyChild();
			break;
		case 'e':
		case 'E':
			flag = true;
			system("pause");
			break;
		defalt:
			break;
		}
		if (flag)
			break;
	}
	return 0;
}