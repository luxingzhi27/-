#include<iostream>
#include<cstring>
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
	int stuNum = 0;   //����
	string stuName; //����
	string stuSex;  //�Ա�
	int stuAge = 0;   //����
	string stuAim;  //����Ŀ��
	//����ѧ���������
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
			cout << "����Ƿ������������룺" << endl;
		}
	}
	return in;
}

void display(list<stuInfo>& stuList, int num)
{
	if (num == 0)
		cout << "�޿�����Ϣ��" << endl;
	else
	{
		cout << endl << "����\t����\t�Ա�\t����\t�������" << endl;
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
	cout << "�����뽨��������Ϣϵͳ��" << endl;
	int num = 0;
	while (1)
	{
		cout << "�����뿼��������";
		cin >> num;
		if (!cin || num <= 0)
		{
			cout << "���벻�Ϸ������������룡" << endl;
			cin.clear();
			cin.ignore(9999, '\n');
		}
		else
			break;
	}

	cout << "���������뿼���Ŀ��ţ��������Ա����估�������" << endl;
	list<stuInfo> stuList;
	stuInfo tmp;
	for (int i = 0; i < num; i++)
	{
		cin >> tmp;
		stuList.pushBack(tmp);
	}

	display(stuList, num);

	cout << endl << "��ѡ����Ҫ���еĲ�����1Ϊ���룬2Ϊɾ����3Ϊ���ң�4Ϊ�޸ģ�5Ϊͳ�ƣ�0Ϊȡ��������" << endl;
	while (1)
	{
		cout << endl << "��ѡ����Ҫ���еĲ�����";
		int choose = 0;
		cin >> choose;
		if (choose == 1)
		{
			while (1)
			{
				cout << endl << "��������Ҫ���뿼����λ�ã�";
				int pos = 0;
				cin >> pos;
				if (pos<1 || pos>num + 1)
				{
					cout << "����λ�����������²��롣" << endl;
					continue;
				}
				else
				{
					cout << endl << "���������뿼���Ŀ��ţ��������Ա����估�������" << endl;
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
				cout << endl << "��������Ҫɾ�������Ŀ��ţ�";
				int pos = 0;
				cin >> pos;
				listNode<stuInfo>* ptr = stuList.getFirst();
				int cnt = 1;
				while (ptr)      //ͨ������Ѱ�ҿ���λ��
				{
					if (ptr->val.stuNum == pos)
						break;
					ptr = ptr->next;
					cnt++;
				}
				if (!ptr)      //δ�ҵ�����
					cout << "�����ѧ����Ϣ��" << endl;
				else
				{
					stuInfo reMoveStu;
					stuList.remove(cnt, reMoveStu);    //ɾ��������Ϣ
					cout << "��ɾ���Ŀ�����Ϣ�ǣ�" << reMoveStu << endl;
					display(stuList, stuList.getLength());
					break;
				}
			}
		}
		else if (choose == 3)
		{
			while (1)
			{
				cout << endl << "��������Ҫ���ҿ����Ŀ��ţ�";
				int pos = 0;
				cin >> pos;
				listNode<stuInfo>* ptr = stuList.getFirst();
				while (ptr)
				{
					if (ptr->val.stuNum == pos)    //ͨ�����Ų��ҿ�����Ϣ
						break;
					ptr = ptr->next;
				}
				if (!ptr)
					cout << "�����ѧ����Ϣ��" << endl;    //δ�ҵ�
				else
				{
					stuInfo findStu = ptr->val;   //�ҵ������
					cout << endl << "�����ҵĿ�����Ϣ�ǣ�" << endl << "����\t����\t�Ա�\t����\t�������" << endl << findStu << endl;
					break;
				}
			}
		}
		else if (choose == 4)
		{
			while (1)
			{
				cout << endl << "��������Ҫ�޸Ŀ����Ŀ��ţ�";
				int pos = 0;
				cin >> pos;
				listNode<stuInfo>* ptr = stuList.getFirst();
				while (ptr)
				{
					if (ptr->val.stuNum == pos)   //ͨ������Ѱ�ҿ�����Ϣ
						break;
					ptr = ptr->next;
				}
				if (!ptr)
					cout << "�����ѧ����Ϣ��" << endl;
				else
				{
					cout << "�����������޸ĺ����Ŀ��ţ��������Ա����估�������" << endl;
					stuInfo modifyStu;
					cin >> modifyStu;   //�ҵ����޸Ŀ�����Ϣ
					ptr->val = modifyStu;
					cout << "�޸ĺ�����Ϣ���£�";
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