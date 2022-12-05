#include<iostream>
using namespace std;

template<class T>
class listNode
{
public:
	T val;    //�ڵ�����
	listNode<T>* next;  //��һ�ڵ�
};

template<class T>
class list :public listNode<T>
{
	//����������������
	template <class S>
	friend ostream& operator<<(ostream& out, list<S> x);
	template <class Q>
	friend istream& operator>>(istream& in, list<Q>& x);
private:
	listNode<T>* first;
public:
	list();       //���캯��
	~list();      //��������
	bool empty() const;   //�����Ƿ�Ϊ��
	int getLength()const;  //��ȡ������
	listNode<T>* find(T elem);   //�ҵ�ָ��Ԫ��
	bool insert(int i, T& elem);    //����ָ��λ��Ԫ��
	bool remove(int i, T& elem);    //ɾ��ָ��λ��Ԫ��
	bool pushBack(T& elem);    //��ĩβ����Ԫ��
	void display() const;      //�������
	listNode<T>* getFirst() { return first; };    //�����׸��ڵ�
};


template<class T>
inline list<T>::list()
{
	this->first = nullptr;      //��ʼ����Ϊ������
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
	if (i == 0 || first == nullptr)           //�����ͷ�����������Ϊ��
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
	else                                   //���м��β������
	{
		listNode<T>* current = first;
		for (int k = 1; k < i; k++)      //���ȱ�����Ҫ����λ��
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

			//��������֮��
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
	if (i == 1)        //����Ƴ���һ��
	{
		del = first;
		first = first->next;
	}
	else             //�Ƴ��м��ĩβ
	{
		current = first;
		for (int k = 1; k < i - 1; k++)    //������Ҫɾ��λ��
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
	delete del;   //ɾ��
	return true;
}

template<class T>
bool list<T>::pushBack(T& elem)    //��ĩβ���Ԫ��
{
	listNode<T>* newNode = new listNode<T>;
	if (!newNode)
	{
		cerr << "Error allocating memory!" << endl;
		return false;
	}

	listNode<T>* tmp = first;
	if (tmp)                   //���ͷ�ڵ㲻��
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = newNode;
		newNode->next = nullptr;
		newNode->val = elem;
	}
	else      //����Ϊ����first��Ϊ�½ڵ�ĵ�ַ
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
		if (cin.fail())       //����������
		{
			cin.clear();     //�ı�����״̬
			cin.ignore(9999, '\n');    //��ջ�����
			cerr << "������������������" << endl;     //����������ʾ
		}
		else
			break;     //�˳�����ѭ��
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
		if (last > n)     //����Ϊ����
		{
			cerr << "���벻����ǽ���Ҫ�����������������" << n << "����֮����" << endl;
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
	cout << "����������ǽ������������У�" << endl;
	list<int>* list1 = new list<int>();
	list<int>* list2 = new list<int>();
	listInput(*list1);
	cout << "������һ�飺" << endl;
	listInput(*list2);

	listNode<int>* pb = list2->getFirst();
	listNode<int>* pa = list1->getFirst();
	listNode<int>* pc = new listNode<int>();   //ָ��paǰһ��ָ�룬����ɾ��
	listNode<int>* tmp = pc;
	pc->next = pa;
	pc->val = 0;

	while (pa != nullptr && pb != nullptr)
	{
		if (pa->val == pb->val)    //�����ȣ�����
		{
			pa = pa->next;
			pb = pb->next;
			pc = pc->next;
		}
		else if (pa->val < pb->val)  //С�ڣ�����ɾ��
		{
			int elem = 0;
			if (pa == list1->getFirst())         //���Ҫɾ������ͷ�ڵ�
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
		else                      //���ڣ�pb����ƶ�
			pb = pb->next;
	}
	while (pa != nullptr)         //��list1ʣ�µ�Ԫ��ɾ��
	{
		int elem = 0;
		if (pa == list1->getFirst())      //���Ҫɾ������ͷ�ڵ�
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
	cout << "���Ľ���Ϊ��";
	if (list1->getFirst())
		cout << *list1 << endl;
	else
		cout << "NULL" << endl;
	if (pc == tmp)
		delete pc;
	return 0;
}