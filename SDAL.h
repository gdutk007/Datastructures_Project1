//
// Created by User1 on 9/21/2017.
//

#ifndef SDAL_SDAL_H
#define SDAL_SDAL_H
//
// Created by User1 on 9/20/2017.

#include <iostream>
#include <stdexcept>
#include "List.h"

namespace cop3530 {

    template <typename T>
    class SDAL : public List<T> {
    private:
        T* ptr = nullptr;
        T* arrayptr = nullptr;
        size_t tail = 0;
        size_t capacity = 0;
        size_t starting_Capacity = 0;
    public:

        template <typename DataT>
        class SDAL_Iter
        {
        public:
            // type aliases required for C++ iterator compatibility
            using value_type = DataT;
            using reference = DataT& ;
            using pointer = DataT*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            // type aliases for prettier code
            using self_type = SDAL_Iter;
            using self_reference = SDAL_Iter&;

        private:
            T* current;
        public:
            explicit SDAL_Iter(T* location) {
                current = location;
            }

            SDAL_Iter( const SDAL_Iter& src ){
                current = src.current;
            }

            reference operator*() const {
                return *current;
            }

            pointer operator->() const {
                return current;
            }

            self_reference operator=( SDAL_Iter<DataT> const& src ) {
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
                SDAL_Iter oldCopy = *this;
                ++current;
                return oldCopy;
            } // postincrement

            bool operator==( SDAL_Iter<DataT> const& rhs ) const {
                return current == rhs.current;
            }

            bool operator!=( SDAL_Iter<DataT> const& rhs) const {
                return current != rhs.current;
            }
        };

        using iterator = SDAL_Iter<T>;
        using const_iterator = SDAL_Iter<T const>;

        iterator Begin(){
            return iterator(ptr);
        }

        iterator End(){
            return iterator(ptr + 50);
        }

        const_iterator Begin() const {
            return const_iterator(ptr);
        }

        const_iterator End() const {
            return const_iterator((ptr + 50));
        }


        SDAL(const SDAL& src) {
            tail = src.tail;
            ptr = new T[src.capacity];
            capacity = src.capacity;
            int i = 0;
            while (i <= src.tail) {
                ptr[i] = src.ptr[i];
                i++;
            }
        }


        SDAL& operator=(const SDAL & src){
            if (&src == this) // check for self-assignment
                return *this; // do nothing

            delete[] ptr;
            ptr = new T[src.ptr];
            tail = src.tail;
            capacity = src.capacity;
            int i = 0;
            while (i <= src.tail) {
                ptr[i] = src.ptr[i];
                i++;
            }
            return *this;
        }

        ~SDAL(){
            delete [] ptr;
        }

        SDAL(size_t space);
        SDAL();
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
        virtual bool contains(T element, bool*)override;
        virtual void print(std::ostream&)override;
        virtual T* contents()override;

    };

    template <typename T>
    SDAL<T>::SDAL() {
        ptr = new T[50];
        tail = 0;
        capacity = 50;
        starting_Capacity = capacity;
    }

    template <typename T>
    SDAL<T>::SDAL(size_t space) {
        ptr = new T[space];
        capacity = space;
        tail = 0;
        starting_Capacity = capacity;
    }


    template <typename T>
    void SDAL<T>::Insert(T element, unsigned int position) {
        if(position < 0 || position > length())
            throw std::runtime_error("Runtime_Error: That position is out of bounds\n");
        else if(is_empty()){
            ptr[0] = element;
            tail = tail + 1;
        }else{

            if(is_full()){
                capacity = capacity*3/2;
                T* newArray = new T[capacity];
                for (int i = 0; i < tail + 1; ++i) {
                    newArray[i] = ptr[i];
                }
                std::cout << "A new larger array was made\n";
                ptr = &newArray[0];
            }

            bool append = false;
            int temp = ptr[position];
            for (int i = 0; i < tail + 1; ++i) {
                if(append){
                    int temp1 = ptr[i];
                    ptr[i] = temp;
                    temp = temp1;
                }
                if(i == position){
                    ptr[i] = element;
                    append = true;
                }
            }
            append = false;
            tail = tail + 1;
        }

    }


    template <typename T>
    void SDAL<T>::push_back(T element) {
        Insert(element,length());
    }


    template <typename T>
    void SDAL<T>::push_front(T element) {
        Insert(element,0);
    }


    template <typename T>
    void SDAL<T>::replace(T element, unsigned int position) {
        if(position > tail || position < 0){
            std::cout << "that position is out of bounds\n";
        }else{
            ptr[position] = element;
        }
    }

    ///FIX REMOVE AND MAKE IT CLEANER
    template <typename T>
    T SDAL<T>::remove( unsigned int position) {
        if(position >= tail || is_empty() || position < 0)
            throw std::runtime_error("Runtime_Error: That position is out of bounds and cannot be removed\n");
        else{
            T temp;
            for (int i = 0; i < tail + 1 ; ++i) {
                if(i == position)
                    temp = ptr[position];
                if(i >= position)
                    ptr[i] = ptr[i + 1];
            }
            if(capacity >= starting_Capacity && length() < capacity/2 ){
                capacity = capacity*0.75 ;
                T* array = new T[capacity];
                for (int i = 0; i < tail + 1 ; ++i) {
                    array[i] = ptr[i];
                }
                ptr = &array[0];
            }
            tail = tail - 1;
            return temp;
        }

    }

    template <typename T>
    T SDAL<T>::pop_back() {
        if(is_empty())
            throw std::runtime_error("Runtime_Error: The list is empty\n");
        else
            return remove(length()-1);
    }

    template <typename T>
    T SDAL<T>::pop_front() {
        if(is_empty())
            throw std::runtime_error("Runtime_Error: The list is empty\n");
        else
            return remove(0);
    }

    template <typename T>
    T& SDAL<T>::item_at(unsigned int position) {
        if(position >= tail || position < 0){
            throw std::runtime_error("Runtime_Error: That position is out of bounds\n");
        }else {
            return ptr[position];
        }
    }

    template <typename T>
    T& SDAL<T>::peek_back() {
        if(is_empty())
            throw std::runtime_error("Runtime_error: The list is empty\n");
        else
            return ptr[tail - 1];
    }

    template <typename T>
    T& SDAL<T>::peek_front() {
        if(is_empty())
            throw std::runtime_error("Runtime_error: The list is empty\n");
        else
            return ptr[0];
    }

    template <typename T>
    bool SDAL<T>::is_full() {
        if (tail == capacity) {
            return 1;
        }
        else {
            return 0;
        }
    }

    template <typename T>
    bool SDAL<T>::is_empty() {
        if (tail == 0) {
            return 1;
        }
        else {
            return 0;
        }
    }

    template <typename T>
    size_t SDAL<T>::length() {
        if(is_empty()){
            return 0;
        }else{

        }
        return tail;
    }

    template <typename T>
    void SDAL<T>::clear() {
        tail = 0;
    }

    template <typename T>
    bool SDAL<T>::contains(T element, bool* equals_Function) {
        return 0;
    }

    template <typename T>
    void SDAL<T>::print(std::ostream& theStream) {
        if (is_empty())
            throw std::runtime_error("Runtime_error: The list is empty\n");
        else
            for (int i = 0; i < tail ; ++i) {
                theStream << ptr[i] << "\n";
            }
    }

    template <typename T>
    T* SDAL<T>::contents() {
        if(is_empty()){
            throw std::runtime_error("Runtime error; There is nothing on the list\n");
        }else{
            T* ptr = new T[length()];
            for (int i = 1; i <length() ; ++i) {
                ptr[i-1] = item_at(i);
            }
            return ptr;
        }
    }

}
#endif //SDAL_SDAL_H