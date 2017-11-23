
#ifndef CBL_CBL_H
#define CBL_CBL_H
//
// Created by User1 on 9/20/2017.

#include <iostream>
#include <stdexcept>
#include "List.h"

namespace cop3530 {

    template <typename T>
    class CBL : public List<T> {
    private:
        size_t head = 0;
        T* ptr = nullptr;
        T* arrayptr = nullptr;
        size_t tail = 0;
        size_t capacity = 0;
        size_t starting_capacity = 0;
    public:

        template <typename DataT>
        class CBL_Iter
        {
        public:
            // type aliases required for C++ iterator compatibility
            using value_type = DataT;
            using reference = DataT& ;
            using pointer = DataT*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            // type aliases for prettier code
            using self_type = CBL_Iter;
            using self_reference = CBL_Iter&;

        private:
            T* current;
        public:
            explicit CBL_Iter(T* location) {
                current = location;
            }

            CBL_Iter( const CBL_Iter& src ){
                current = src.current;
            }

            reference operator*() const {
                return *current;
            }

            pointer operator->() const {
                return current;
            }

            self_reference operator=( CBL_Iter<DataT> const& src ) {
                if(*this == src){
                    return *this;
                }
                current = src.current;
                return *this;
            }

            self_reference operator++() {
                ++current;
                return *this;
            } // preincrement

            self_type operator++(int) {
                CBL_Iter oldCopy = *this;
                ++current;
                return oldCopy;
            } // postincrement

            bool operator==( CBL_Iter<DataT> const& rhs ) const {
                return current == rhs.current;
            }

            bool operator!=( CBL_Iter<DataT> const& rhs) const {
                return current != rhs.current;
            }
        };

        using iterator = CBL_Iter<T>;
        using const_iterator = CBL_Iter<T const>;

        iterator Begin(){
            return iterator(&ptr[head]);
        }

        iterator End(){
            return iterator(&ptr[tail]);
        }

        const_iterator Begin() const {
            return const_iterator(&ptr[head]);
        }

        const_iterator End() const {
            return const_iterator(&ptr[tail]);
        }


        CBL(const CBL& src) {
            tail = src.tail;
            ptr = new T[src.capacity];
            capacity = src.capacity;
            int i = 0;
            for (int j = head; j != tail + 1 ; j = (j+1)%capacity) {
                ptr[i] = src.ptr[i];
            }
        }


        CBL& operator=(const CBL & src){
            if (&src == this) // check for self-assignment
                return *this; // do nothing

            delete[] ptr;
            ptr = new T[src.ptr];
            tail = src.tail;
            capacity = src.capacity;
            int i = 0;
            for (int j = head; j != tail + 1 ; j = (j+1)%capacity) {
                ptr[i] = src.ptr[i];
            }
            return *this;
        }


        ~CBL(){
            delete [] ptr;
        }

        CBL(size_t space);
        CBL();
        virtual void Insert(T element, unsigned int position)override;
        virtual void push_back(T element)override;
        virtual void push_front(T element)override;
        virtual void replace(T element, unsigned int position)override;
        virtual T remove(unsigned int position)override;
        virtual T pop_back()override;
        virtual T pop_front()override;
        virtual T& item_at(unsigned int position)override;
        virtual T& peek_back()override;
        virtual T& peek_front()override;
        virtual bool is_empty()override;
        virtual bool is_full()override;
        virtual size_t length()override;
        virtual void clear()override;
        virtual bool contains(T element, bool* equals_function)override;
        virtual void print(std::ostream&)override;
        virtual T* contents()override;

    };

    template <typename T>
    CBL<T>::CBL() {
        ptr = new T[50 + 1];
        tail = head = 0;
        capacity = 50 + 1;
        starting_capacity = capacity;
    }

    template <typename T>
    CBL<T>::CBL(size_t space) {
        ptr = new T[space + 1];
        capacity = space + 1;
        starting_capacity = capacity;
        tail = head = 0;
    }

    template <typename T>
    void CBL<T>::Insert(T element, unsigned int position) {
        if(position > length() || position < 0)
            throw std::runtime_error("Runtime_Error: The that position is out of Bounds\n");
        if(is_full()){
            int limit = capacity;
            std::cout << "A new array was made\n";
            int size = length();
            capacity = capacity*1.5;
            T *array = new T[capacity];
            if (head < tail) {
                for (int i = head; i < tail; ++i) {
                    array[i] = ptr[i];
                }
            } else {
                int index = 0;
                for (int i = head, k = 0; i < limit; ++i, ++k) {
                    array[k] = ptr[i];
                    index = k;
                }
                index++;
                for (int j = index, y = 0; y <= tail; ++j, ++y) {
                    array[j] = ptr[y];
                }
            }
            head = 0;
            tail = limit - 1;
            T *deleteptr = &ptr[0];
            ptr = &array[0];
            delete deleteptr;
            bool append = false;
            int temp = ptr[(position + head)%capacity];
            for (int i = head; i != tail + 1; i = (i + 1)% capacity) {
                if(append){
                    int temp1 = ptr[i];
                    ptr[i] = temp;
                    temp = temp1;
                }
                if(i == (head + position)%capacity){
                    ptr[i] = element;
                    append = true;
                }
            }
            append = false;
            tail = (tail + 1)%capacity;
        }else if(is_empty()){
            push_back(element);
        }else{
            bool append = false;
            int temp = ptr[(position + head)%capacity];
            for (int i = head; i != tail + 1; i = (i + 1)% capacity) {
                if(append){
                    int temp1 = ptr[i];
                    ptr[i] = temp;
                    temp = temp1;
                }
                if(i == (head + position)%capacity){
                    ptr[i] = element;
                    append = true;
                }
            }
            append = false;
            tail = (tail + 1)%capacity;
        }
    }

    template <typename T>
    void CBL<T>::push_back(T element) {
        if(is_full()) {
            int limit = capacity;
            std::cout << "A new array was made\n";
            int size = length();
            capacity = capacity*1.5;
            T *array = new T[capacity];
            if (head < tail) {
                for (int i = head; i < tail; ++i) {
                    array[i] = ptr[i];
                }
            } else {
                int index = 0;
                for (int i = head, k = 0; i < limit; ++i, ++k) {
                    array[k] = ptr[i];
                    index = k;
                }
                index++;
                for (int j = index, y = 0; y <= tail; ++j, ++y) {
                    array[j] = ptr[y];
                }
            }
            head = 0;
            tail = limit - 1;
            T *deleteptr = &ptr[0];
            ptr = &array[0];
            delete deleteptr;
            ptr[tail] = element;
            tail = (tail + 1)%capacity;
        }else{
            ptr[tail] = element;
            tail = (tail + 1)%capacity;
        }
    }

    template <typename T>
    void CBL<T>::push_front(T element) {
        if(is_full()){
            int limit = capacity;
            std::cout << "A new array was made\n";
            int size = length();
            capacity = capacity *1.5 ;
            T *array = new T[capacity];
            if (head < tail) {
                for (int i = head; i <= tail; ++i) {
                    array[i] = ptr[i];
                }
            } else {
                int index = 0;
                for (int i = head, k = 0; i < limit; ++i, ++k) {
                    array[k] = ptr[i];
                    index = k;
                }
                index++;
                for (int j = index, y = 0; y < tail; ++j, ++y) {
                    array[j] = ptr[y];
                }
            }
            head = 0;
            tail = limit - 1;
            T* deleteptr = &ptr[0];
            ptr = &array[0];
            delete deleteptr;
        } else {
            head = (head + capacity - 1)% capacity;
            ptr[head] = element;
        }
    }

    template <typename T>
    void CBL<T>::replace(T element, unsigned int position) {
        if(position > length())
            throw std::runtime_error("Runtime_Error: The position is out of bounds\n");
        else{
            ptr[ (head + position)%(capacity - 1) ] = element;
        }
    }

    template <typename T>
    T CBL<T>::remove( unsigned int position) {
        if(is_empty()){
            throw std::runtime_error("Runtime_Error: The list is empty, you cannot remove anything\n");
        }else {
            T temp;
            bool append = false;
            for (int i = head; i != tail + 1; i = (i + 1)%capacity) {
                if(i == (head + position)%capacity){
                    append = true;
                    temp = ptr[i];
                }
                if(append)
                    ptr[i] = ptr[ ( i + 1 )% capacity];
            }
            append = false;
            tail =  (tail + capacity - 1)%capacity;

            if(capacity >= starting_capacity && length() < capacity/2){
                capacity = capacity*0.75;
                T* newArray = new T[capacity];
                for (int j = head,l = 0; j != tail ; j = (j + 1)%capacity, l = l + 1) {
                    newArray[l] = ptr[l];
                }
                T* deletePtr = &ptr[0];
                ptr = &newArray[0];
                delete deletePtr;
            }
            return temp;
        }

    }

    template <typename T>
    T CBL<T>::pop_back() {
        if(is_empty()){
            std::runtime_error("Runtime_Error: The list is empty\n");
        }else{
            return remove(length()-1);
        }
    }

    template <typename T>
    T CBL<T>::pop_front() {
        if(is_empty()){
            throw std::runtime_error("Runtime_Error: The list is empty\n");
        }else {
           return remove(0);
        }
    }

    template <typename T>
    T& CBL<T>::item_at(unsigned int position) {
        return ptr[ (head + position)%capacity];
    }

    template <typename T>
    T& CBL<T>::peek_back() {
        if(is_empty())
            throw std::runtime_error("Runtime_Error: The list is empty\n");
        else
            return ptr[tail - 1];
    }

    template <typename T>
    T& CBL<T>::peek_front() {
        if(is_empty())
            throw std::runtime_error("Runtime_Error: The list is empty\n");
        return ptr[head];
    }

    template <typename T>
    bool CBL<T>::is_full() {
        if( (head == 0 && tail == capacity - 1) || (tail == head - 1) )
            return 1;
        else
            return 0;
    }

    template <typename T>
    bool CBL<T>::is_empty() {
        if(tail == head)
            return 1;
        else
            return 0;
    }

    template <typename T>
    size_t CBL<T>::length() {
        if(head > tail)
            return ( capacity - ( head - tail) );
        else
            return (tail - head);
    }

    template <typename T>
    void CBL<T>::clear() {
        tail = head = 0;
    }

    template <typename T>
    bool CBL<T>::contains(T element, bool* equals_function) {
        return 0;
    }

    template <typename T>
    void CBL<T>::print(std::ostream& theStream) {
        for (int i = head; i%capacity != tail ; i = (i + 1)%capacity) {
            theStream << ptr[i] << "\n";
        }
    }

    template <typename T>
    T* CBL<T>::contents() {
        if(is_empty())
            throw std::runtime_error("Runtime_Error: The list is empty\n");
        else{
            T* newArray = new T[capacity];
            for (int i = head,j = 0; i != tail + 1 ; i = (i + 1)%capacity, ++j) {
                newArray[j] = ptr[i];
            }
        }
        return ptr;
    }

}
#endif //CBL_CBL_H