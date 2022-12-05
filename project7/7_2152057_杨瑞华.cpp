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
    int getLength() const { return size; };
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

void input(int& in, int min, int max)
{
    while (1)
    {
        cin >> in;
        if (!cin || in<min || in>max)
        {
            cin.clear();
            cin.ignore(9999, '\n');
            cout << "error input!\n";
        }
        else
            break;
    }
}

int minPrice(priorityQueue<int>& lengths)
{
    int price = 0;
    while (!lengths.empty() && lengths.getLength() != 1)
    {
        int min1 = lengths.pop();
        price += min1;
        int min2 = lengths.pop();
        price += min2;
        lengths.push(min1 + min2);
    }
    return price;
}


int main()
{
    int n = 0;
    input(n, 2, 10000);
    priorityQueue<int> lengths;
    for (int i = 0; i < n; i++)
    {
        int tmp = 0;
        input(tmp, 1, 10000);
        lengths.push(tmp);
    }
    cout << minPrice(lengths) << endl;
    system("pause");
    return 0;
}