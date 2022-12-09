#define ADD_LENGTH 50 // the vector adding capacity
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <ctime>
#include <regex>
using namespace std;

template <class T>
class Vector
{
private:
    T *elements = nullptr;
    int maxSize = 0;
    int length = maxSize;
    void setMaxSize(int size) { maxSize = size; };
    void overSize();

public:
    Vector() : maxSize(0), length(0), elements(nullptr){};
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
    Vector(const Vector<T> &v) : elements(v.elements), maxSize(v.maxSize), length(v.length){};
    ~Vector() { delete[] elements; };
    void pushBack(const T &elem);
    int getLength() const { return length; };
    void insert(int i, const T &elem);
    T remove(int i);
    void reMove(const T &aim);
    T minElem();
    T maxElem();
    T &operator[](int i) const;
    int find(const T &elem) const;
    int findByPtr(const T &elem) const;
    T sum();
    void makeEmpty();
};

template <class T>
class priorityQueue
{
private:
    int size;               // the size of the queue
    Vector<T> heap;         // to store the data
    void filterUp(int i);   // to maintain the minHeap from bottle to top
    void filterDown(int i); // to maintain the minHeap from top to bottle
    T popMin();             // pop the minimal elem
    T popMinByPtr();
    int parent(int i);          // return the position of parent
    int left(int i);            // return the position of left child
    int right(int i);           // return the posision of right child
    void swap(T &a, T &b);      // swap the value of a,b
    long long compareTimes = 0; // compare times
    long long swapTimes = 0;    // swap times

public:
    priorityQueue();
    ~priorityQueue();
    bool empty();      // judge whether the queue is empty
    T pop();           // out of the queue
    void push(T elem); // into the queue
    long long getCompareTimes() const
    {
        return compareTimes;
    };
    long long getSwapTimes() const
    {
        return swapTimes;
    };
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
void priorityQueue<T>::swap(T &a, T &b)
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
            compareTimes++;
            if (heap[k] >= heap[j])
                break;
            else
            {
                swapTimes++;
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
            compareTimes++;
            if (j < size - 1 && heap[j] > heap[j + 1])
                j++;
            compareTimes++;
            if (temp <= heap[j])
                break;
            else
            {
                swapTimes++;
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
    T *newArray = new T[maxSize];
    if (!newArray)
        std::cerr << "wrong memeory allocation!\n";
    for (int i = 0; i < maxSize - ADD_LENGTH; i++)
        newArray[i] = elements[i];
    delete[] elements;
    elements = newArray;
}

template <class T>
inline void Vector<T>::pushBack(const T &elem)
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
inline void Vector<T>::insert(int i, const T &elem)
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
inline void Vector<T>::reMove(const T &aim)
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
inline T &Vector<T>::operator[](int i) const
{
    return elements[i];
}

template <class T>
inline int Vector<T>::find(const T &aim) const
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
inline int Vector<T>::findByPtr(const T &elem) const
{
    int pos = -1;
    for (int i = 0; i < length; i++)
    {
        if (*elements[i] == *elem)
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

class Sort
{
public:
    // 八种排序算法
    void bubbleSort(long long &compareTimes, long long &swapTimes);
    void selectionSort(long long &compareTimes, long long &swapTimes);
    void heapSort(long long &compareTimes, long long &swapTimes);
    void mergeSort(int low, int high, long long &compareTimes, long long &swapTimes);
    void quickSort(int low, int high, long long &compareTimes, long long &swapTimes);
    void straightInsertionSort(long long &compareTimes, long long &swapTimes);
    void shellSort(long long &compareTimes, long long &swapTimes);
    void radixSort(long long &compareTimes, long long &swapTimes);
    // 合并数组
    void mergeArray(int low, int mid, int high, long long &compareTimes, long long &swapTimes);
    // 生成随机数
    void generateRandomnumbers(int size);
    // 将乱序数组_array复制到array中
    void copyArray();
    Vector<unsigned int> _array;
    Vector<unsigned int> array;
};

void Sort::copyArray()
{
    int size = _array.getLength();
    if (array.getLength() < size)
    {
        for (int i = 0; array.getLength() < size; ++i)
            array.pushBack(i);
    }
    for (int i = 0; i < size; ++i)
        array[i] = _array[i];
}

void Sort::bubbleSort(long long &compareTimes, long long &swapTimes)
{
    for (int i = 0; i < array.getLength(); i++) // 外层循环，循环array.getLength()次
    {
        for (int j = 0; j < array.getLength() - i - 1; j++) // 内层循环，循环array.getLength() - i - 1次
        {
            compareTimes++;              // 使compareTimes的值增加1
            if (array[j] > array[j + 1]) // 如果array[j]大于array[j + 1]
            {
                swapTimes++; // 使swapTimes的值增加1
                // 交换他们的值
                unsigned int temp = array[j]; // 将array[j]的值赋给临时变量temp
                array[j] = array[j + 1];      // 将array[j + 1]的值赋给array[j]
                array[j + 1] = temp;          // 将temp的值赋给array[j + 1]
            }
        }
    }
}

void Sort::selectionSort(long long &compareTimes, long long &swapTimes)
{
    for (int i = 0; i < array.getLength() - 1; i++) // 外层循环，循环array.getLength() - 1次
    {
        int min = array[i];
        for (int j = i + 1; j < array.getLength(); j++) // 内层循环，循环array.getLength() - i - 1次
        {
            compareTimes++;     // 使compareTimes的值增加1
            if (array[j] < min) // 如果array[j]小于min
            {
                // 找到最小值
                swapTimes++;                  // 使swapTimes的值增加1
                unsigned int temp = array[j]; // 将array[j]的值赋给临时变量temp
                array[j] = min;               // 将min的值赋给array[j]
                min = temp;                   // 将temp的值赋给min
            }
        }
    }
}

void Sort::heapSort(long long &compareTimes, long long &swapTimes)
{
    priorityQueue<unsigned int> s;
    for (int i = 0; i < array.getLength(); i++)
    {
        s.push(array[i]);
    }
    for (int i = 0; i < array.getLength(); i++)
    {
        array[i] = s.pop();
    }
    compareTimes = s.getCompareTimes();
    swapTimes = s.getSwapTimes();
}

void Sort::straightInsertionSort(long long &compareTimes, long long &swapTimes)
{
    unsigned int temp;
    int j;
    for (int i = 1; i < array.getLength(); i++)
    {
        compareTimes++;
        if (array[i] < array[i - 1])
        {
            temp = array[i];
            j = i - 1;
            while (1)
            {
                compareTimes++;
                if (temp >= array[j] || j < 0)
                    break;
                swapTimes++;
                array[j + 1] = array[j];
                j--;
            }
            array[j + 1] = temp;
        }
    }
}

void Sort::shellSort(long long &compareTimes, long long &swapTimes)
{
    int n = array.getLength(); // 获取数组的长度
    int gap = n;               // 初始化增量序列
    while (gap > 1)            // 当增量序列的值大于1时循环
    {
        gap = gap / 2;                    // 每次将增量序列除以2
        for (int i = 0; i < n - gap; i++) // 从0到n - gap循环
        {
            int end = i;                 // end初始化为i
            int temp = array[end + gap]; // 将array[end + gap]赋值给临时变量temp
            while (end >= 0)             // 如果end >= 0，继续循环
            {
                compareTimes++;        // 比较次数加1
                if (temp < array[end]) // 如果temp < array[end]
                {
                    swapTimes++;                   // 交换次数加1
                    array[end + gap] = array[end]; // 交换array[end + gap]和array[end]
                    end -= gap;                    // end减去gap
                }
                else       // 否则
                    break; // 退出循环
            }
            array[end + gap] = temp; // 将临时变量temp赋值给array[end + gap]
        }
    }
}

void Sort::radixSort(long long &compareTimes, long long &swapTimes)
{
    int max = array[0];
    int base = 1;
    int n = array.getLength();

    for (int i = 0; i < n; i++)
    {
        compareTimes++;
        if (array[i] > max)
            max = array[i]; // 取得数组中的最大数
    }

    int *tmp = (int *)malloc(sizeof(int) * n);

    while (max / base > 0)
    {
        int bucket[10] = {0};
        for (int i = 0; i < n; i++)
            bucket[array[i] / base % 10]++;

        for (int i = 1; i < 10; i++)
            bucket[i] += bucket[i - 1];

        for (int i = n - 1; i >= 0; i--)
        {
            tmp[bucket[array[i] / base % 10] - 1] = array[i];
            bucket[array[i] / base % 10]--;
        }
        for (int i = 0; i < n; i++)
            array[i] = tmp[i];
        base *= 10;
    }
    free(tmp);
}

void Sort::mergeArray(int low, int mid, int high, long long &compareTimes, long long &swapTimes)
{
    int *tmp = (int *)malloc(sizeof(int) * array.getLength());
    if (low >= high)
        return;
    else
    {
        int i = low, j = mid + 1, k = low;
        while (i <= mid && j <= high)
        {
            compareTimes++;
            if (array[i] <= array[j])
                tmp[k++] = array[i++];
            else
                tmp[k++] = array[j++];
        }
        while (i <= mid)
            tmp[k++] = array[i++];
        while (j <= high)
            tmp[k++] = array[j++];
        for (int i = low; i <= high; i++)
            array[i] = tmp[i];
        free(tmp);
    }
}

void Sort::mergeSort(int low, int high, long long &compareTimes, long long &swapTimes)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        mergeSort(low, mid, compareTimes, swapTimes);
        mergeSort(mid + 1, high, compareTimes, swapTimes);
        mergeArray(low, mid, high, compareTimes, swapTimes);
    }
}

void Sort::generateRandomnumbers(int size)
{
    srand((unsigned int)time(0));
    _array.makeEmpty();
    for (int i = 0; i < size; i++)
    {
        _array.pushBack(rand());
    }
}

void Sort::quickSort(int low, int high, long long &compareTimes, long long &swapTimes)
{
    if (low >= high)
        return;
    int i = low;
    int j = high;
    int key = array[low];
    while (i < j)
    {
        for (; array[j] >= key && i < j; compareTimes++)
            j--;
        swapTimes++;
        array[i] = array[j];
        for (; array[i] <= key && i < j; compareTimes++)
            i++;
        swapTimes++;
        array[j] = array[i];
    }
    swapTimes++;
    array[i] = key;
    quickSort(low, i - 1, compareTimes, swapTimes);
    quickSort(i + 1, high, compareTimes, swapTimes);
}

int main()
{
    cout << "**\t排序算法比较      \t**\n"
         << "==================================\n"
         << "**\t1 --- 冒泡排序    \t**\n"
         << "**\t2 --- 选择排序    \t**\n"
         << "**\t3 --- 直接插入排序\t**\n"
         << "**\t4 --- 希尔排序    \t**\n"
         << "**\t5 --- 快速排序    \t**\n"
         << "**\t6 --- 堆排序      \t**\n"
         << "**\t7 --- 归并排序    \t**\n"
         << "**\t8 --- 基数排序    \t**\n"
         << "**\t9 --- 退出程序    \t**\n"
         << "==================================\n";
    int n;
    while (1)
    {
        cout << "请输入要产生的随机数的个数：";
        cin >> n;
        if (cin.fail() || n < 3 || n > 1000000)
        {
            cout << "Invalid Input!!!" << endl;
            cin.clear();
            cin.ignore(99999, '\n');
            continue;
        }
        break;
    }
    Sort obj;
    obj.generateRandomnumbers(n);
    int choice;
    while (1)
    {
        cout << endl;
        while (1)
        {
            cout << "请选择排序算法：";
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 9)
            {
                cout << "Invalid Input!!!" << endl;
                cin.clear();
                cin.ignore(99999, '\n');
                continue;
            }
            else
                break;
        }
        DWORD timeStart, timeEnd;
        long long swapTimes = 0, compareTimes = 0;
        switch (choice)
        {
        case 1:
            obj.copyArray();
            timeStart = GetTickCount();
            obj.bubbleSort(compareTimes, swapTimes);
            timeEnd = GetTickCount();
            cout << "所用时间：" << (timeEnd - timeStart) << "ms" << endl;
            cout << "交换次数：" << swapTimes << endl;
            cout << "比较次数：" << compareTimes << endl;
            break;
        case 2:
            obj.copyArray();
            timeStart = GetTickCount();
            obj.selectionSort(compareTimes, swapTimes);
            timeEnd = GetTickCount();
            cout << "所用时间：" << (timeEnd - timeStart) << "ms" << endl;
            cout << "交换次数：" << swapTimes << endl;
            cout << "比较次数：" << compareTimes << endl;
            break;
        case 3:
            obj.copyArray();
            timeStart = GetTickCount();
            obj.straightInsertionSort(compareTimes, swapTimes);
            timeEnd = GetTickCount();
            cout << "所用时间：" << (timeEnd - timeStart) << "ms" << endl;
            cout << "交换次数：" << swapTimes << endl;
            cout << "比较次数：" << compareTimes << endl;
            break;
        case 4:
            obj.copyArray();
            timeStart = GetTickCount();
            obj.shellSort(compareTimes, swapTimes);
            timeEnd = GetTickCount();
            cout << "所用时间：" << (timeEnd - timeStart) << "ms" << endl;
            cout << "交换次数：" << swapTimes << endl;
            cout << "比较次数：" << compareTimes << endl;
            break;
        case 5:
            obj.copyArray();
            timeStart = GetTickCount();
            obj.quickSort(0, obj.array.getLength(), compareTimes, swapTimes);
            timeEnd = GetTickCount();
            cout << "所用时间：" << (timeEnd - timeStart) << "ms" << endl;
            cout << "交换次数：" << swapTimes << endl;
            cout << "比较次数：" << compareTimes << endl;
            break;
        case 6:
            obj.copyArray();
            timeStart = GetTickCount();
            obj.heapSort(compareTimes, swapTimes);
            timeEnd = GetTickCount();
            cout << "所用时间：" << (timeEnd - timeStart) << "ms" << endl;
            cout << "交换次数：" << swapTimes << endl;
            cout << "比较次数：" << compareTimes << endl;
            break;
        case 7:
            obj.copyArray();
            timeStart = GetTickCount();
            obj.mergeSort(0, obj.array.getLength(), compareTimes, swapTimes);
            timeEnd = GetTickCount();
            cout << "所用时间：" << (timeEnd - timeStart) << "ms" << endl;
            cout << "交换次数：" << swapTimes << endl;
            cout << "比较次数：" << compareTimes << endl;
            break;
        case 8:
            obj.copyArray();
            timeStart = GetTickCount();
            obj.radixSort(compareTimes, swapTimes);
            timeEnd = GetTickCount();
            cout << "所用时间：" << (timeEnd - timeStart) << "ms" << endl;
            cout << "交换次数：" << swapTimes << endl;
            cout << "比较次数：" << compareTimes << endl;
            break;
        case 9:
            system("pause");
            return 0;
        default:
            break;
        }
    }
}