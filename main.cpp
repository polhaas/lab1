#include <iostream>
#include <malloc.h>
#pragma once

using namespace std;

template <class T>
class CircularBuffer {
private:
    T* buffer;
public:
    using value_type = T;

    CircularBuffer(){

       cout << "Start working" << endl;
    }
    ~CircularBuffer(){
        cout << "End working" << endl;
    }

    CircularBuffer(const CircularBuffer& cb){
        cout << cb << endl;
    }

    CircularBuffer(int capacity, const T& elem){ //Конструирует буфер заданной ёмкости, целиком заполняет его элементом
        buffer = new T[capacity];
        for (int i = 0; i < capacity; i++)
           buffer[i] = elem;

    }

    explicit CircularBuffer(int capacity){  //Конструирует буфер заданной ёмкости.
        buffer = new T[capacity];
        for (int i = 0; i < capacity; i++)
            buffer[i] = 0;
    }


    CircularBuffer& operator=(const CircularBuffer& cb){ //Оператор присваивания.
        for (int i = 0; i < capacity(); i++)
            buffer[i] = cb[i];
    }

    T& operator[](int i){  //Доступ к элементу по индексу
        return buffer[i];
    }
    const T& operator[](int i) const{
        return buffer[i];
    }

    T& at(int i){
        if ( i > capacity()){
            cout << "wrong index" << endl;
            exit(0);
        }
        return buffer[i];
    }
    const T& at(int i) const{
        if ( i > capacity()){
            cout << "wrong index" << endl;
            exit(0);
        }
        return buffer[i];
    }

    int size() const{//Количество элементов, хранящихся в буфере.
        int count = 0;
        for (int i = 0; i < capacity(); i++)
            if (buffer[i] != 0)
                count++;
        return count;
    }

    int capacity() const{   //ёмкость буфера
        return _msize(buffer) / sizeof(T);
    }


    int reserve() const{   //Количество свободных ячеек в буфере.
        return capacity()-size();
    }

    bool empty() const{   //Пуст ли буфер(true - пуст)
        return capacity() == 0;
    }

    bool full() const{  //Полон ли буфер(true - полон)
        return capacity() == size();
    }

    void push_back(const T& item = T()){  //Добавляет элемент в конец буфера.
        if (full())
            buffer[0] = item;
        else
            buffer[size()] = item;
    }

    void push_front(const T& item = T()){  //Добавляет новый элемент перед первым элементом буфера.
        T value;
        if (empty() || full())
            buffer[0] = item;
        else{
            for (int i = strlen(buffer) - 1; i >= 0;  i--)
                buffer[i++] = buffer[i];
            buffer[0] = item;
        }
    }

    void pop_back(){    //удаляет последний элемент буфера.
        buffer[size()-1] = NULL;
    }
    void pop_front(){  //удаляет первый элемент буфера.
        buffer[0] = NULL;
    }

    void rotate(int new_start){  //new_start - новое начало
        T buffer_new[capacity()];
        for (int i = 0; i < capacity(); i++){
            if (i-4 < 0)
                buffer_new[capacity() - i + 4] = buffer[i];
            else
                buffer_new[i - 4] = buffer[i];
        }
        buffer = buffer_new;
    }

    void set_capacity(int new_capacity_){
        cin >> new_capacity_;
        T buffer_new[new_capacity_];
        buffer = buffer_new;
    }
    //Изменяет размер буфера.
    //В случае расширения, новые элементы заполняются элементом item.
    void resize(int new_size, const T& item = T()){
        T buffer_new[new_size];
        if (new_size > capacity()) {
            for (int i = 0; i < capacity(); i++)
                buffer_new[i] = buffer[i];
            for (int i = capacity(); i < new_size; i++)
                buffer_new[i] = item;
        } else
            for (int i = 0; i < capacity(); i++)
                buffer_new[i] = buffer[i];
         buffer = buffer_new;
    }


    void swap(CircularBuffer& cb){ //Обменивает содержимое буфера с буфером cb.
        T elem;
        for (int i = 0; i < capacity(); i++){
            elem = cb[i];
            cb[i] = buffer[i];
            buffer[i] = elem;
        }
    }


    void insert(int pos, const T& item = T()){  //Вставляет элемент item по индексу pos. Ёмкость буфера остается неизменной
        buffer[pos] = item;
    }


    void erase(int first, int last){ //Удаляет элементы из буфера в интервале [first, last).
        for (int i = first; i < last; i++)
            buffer[i] = NULL;
    }


    void clear(){  //Очищает буфер.
        erase(0, capacity());
    }
};

template <class T>
bool operator==(const CircularBuffer<T> &a, const CircularBuffer<T> &b){
    if (a.capacity() != b.capacity())
        return false;
    if (a.size() != b.size)
        return false;
    for (int i = 0; i < a.size(); i++)
        if (a[i] != b[i])
            return  false;
    return true;
}

template <class T>
bool operator!=(const CircularBuffer<T> &a, const CircularBuffer<T> &b){
    if (a.capacity() != b.capacity())
        return true;
    if (a.size() != b.size)
        return true;
    for (int i = 0; i < a.size(); i++)
        if (a[i] != b[i])
            return  true;
    return false;
}
int main() {

    CircularBuffer <char> buf(10);
    int a = buf.size();
    cout << a << endl;
    cout << buf[1] << endl;
    buf.insert(1, 'j');
    cout << buf[1] << endl;

    return 0;
}