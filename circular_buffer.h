

#ifndef CIRCULARBUFFER_CIRCULAR_BUFFER_H
#define CIRCULARBUFFER_CIRCULAR_BUFFER_H
#include <iostream>
#include <malloc.h>
#pragma once

using namespace std;

template <class T>
class CircularBuffer {
private:
    T* buffer;
    int head;
    int size_b;
public:
    using value_type = T;

    ~CircularBuffer(){
        delete [] buffer;
    }
    CircularBuffer(){
             buffer = new T[0];
             head = 0;
             size_b = 0;
    }

    CircularBuffer(const CircularBuffer& cb){
        buffer = new T [capacity(cb)];
        	    head = cb.head;
        	    size_b = cb.size_b;
                for(int i = 0; i < cb.size; i++)
                    buffer[i] = cb.buffer[i];
    }

    CircularBuffer(int capacity, const T& elem){ //Конструирует буфер заданной ёмкости, целиком заполняет его элементом
        buffer = new T[capacity];
        head = 0;
        size_b = capacity;
        for (int i = 0; i < capacity; i++)
            buffer[i] = elem;

    }

    explicit CircularBuffer(int capacity){  //Конструирует буфер заданной ёмкости.
        buffer = new T[capacity];
        head = 0;
        size_b = 0;
        for (int i = 0; i < capacity; i++)
            buffer[i] = 0;
    }


    CircularBuffer& operator=(const CircularBuffer& cb){//Оператор присваивания.
        if (this == &cb)
            return *this;
        set_capacity(cb.capacity());
        resize(cb.size_b);
        head = cb[0];
        for (int i = 0; i < capacity(); i++)
            buffer[i] = cb[i];
        return *this;
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

    //Ссылка на первый элемент
    T& front(){
        if(empty())
            throw "Buffer is empty";
                return buffer[head];
    }
    const T& front() const{
        if(empty())
            throw "Buffer is empty";
        return buffer[head];
    }

    //Ссылка на последний элемент.
    T& back(){
        return buffer[size()-1];
    }
    const T& back() const{
        return buffer[size()-1];
    }

    int size() const{//Количество элементов, хранящихся в буфере.
        return size_b;
    }

    int capacity() const{
        return _msize(buffer)/sizeof(T);
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
            for (int i = size_b - 1; i >= 0;  i--)
                buffer[i++] = buffer[i];
            buffer[0] = item;
        }
    }

    void pop_back(){    //удаляет последний элемент буфера.
        if(!empty())
            size_b--;
        else
            throw "buffer is empty";
    }

    void pop_front(){  //удаляет первый элемент буфера.
        if(!empty()) {
            head++;
            if(head == capacity())
                head = 0;
            size_b--;
        }
        else
            throw "buffer is empty";
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

    //Линеаризация - сдвинуть кольцевой буфер так, что его первый элемент
    //переместится в начало аллоцированной памяти. Возвращает указатель
    //на первый элемент.
    T* linearize(){
        rotate(0);
        return buffer;
    }

    //Проверяет, является ли буфер линеаризованным.
    bool is_linearized() const{
        return head == 0;
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
        linearize();
        copy(&buffer[first], &buffer[last+1], capacity() - last);
        size_b-= (last - first + 1);
    }


    void clear(){  //Очищает буфер.
        erase(0, capacity());
    }
};

template <class T>
bool operator==(const CircularBuffer<T> &a, const CircularBuffer<T> &b){
    if (a.capacity() != b.capacity())
        return false;
    if (a.size() != b.size())
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
    if (a.size() != b.size())
        return true;
    for (int i = 0; i < a.size(); i++)
        if (a[i] != b[i])
            return  true;
    return false;
}
#endif
