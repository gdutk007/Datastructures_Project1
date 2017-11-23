#pragma once
#include <iostream>
#include <stdexcept>
#include "List.h"
namespace cop3530 {
    template<typename T>
    class CDAL : public List<T> {
    private:
        struct node{
            T *array;
            node *next;
        };
        size_t capacity;
        node *current;
        node *head;
        size_t tail;
    public:
        template <typename DataT>
        class CDAL_Iter: public std::iterator<std::forward_iterator_tag,DataT>{
        public:
            // type aliases required for C++ iterator compatibility
            using value_type = DataT;
            using reference = DataT& ;
            using pointer = DataT*;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            // type aliases for prettier code
            using self_type = CDAL_Iter;
            using self_reference = CDAL_Iter&;
        private:
            node* here;
            int currentIndex;
        public:
            explicit CDAL_Iter(node* start = nullptr, int Index = 0) : here(start), currentIndex(Index) {}

            CDAL_Iter(const CDAL_Iter& src) : here(src.here), currentIndex(src.currentIndex) {}

            reference operator*() const {
                if (!here) {
                    throw std::runtime_error("Runtime_Error: Null node\n");
                }
                return here->array[currentIndex];
            }

            pointer operator->() const {
                if (!here) {
                    throw std::runtime_error("Runtime_Error: Null node\n");
                }
                return &here->array[currentIndex];
            }

            self_reference operator=(const CDAL_Iter& src) {
                if (*this == src) {
                    return *this;
                }
                here = src.here;
                currentIndex = src.currentIndex;
                return *this;
            }

            self_reference operator++() {
                ++currentIndex;
                if (currentIndex == 50 && here->next) {
                    here = here->next;
                    currentIndex = 0;
                }
                return *this;
            } // preincrement


            self_type operator++(int) {
                CDAL_Iter results(*this);
                ++(*this);
                return results;
            } // postincrement

            bool operator==(const CDAL_Iter& rhs) const {
                return here == rhs.here && currentIndex == rhs.currentIndex;
            }

            bool operator!=(const CDAL_Iter& rhs) const {
                return !(here == rhs.here && currentIndex == rhs.currentIndex);
            }

        };

        using iterator = CDAL_Iter<T>;
        using const_iterator = CDAL_Iter<T const>;

        iterator Begin(){
            return iterator(head, 0);
        }

        iterator End(){
            node* temp = head;
            while(temp != current){
                temp = temp->next;
            }

            return iterator(temp,tail%50);
        }

        const_iterator Begin() const {
            return const_iterator(head,0);
        }

        const_iterator End() const {
            node* temp = head;
            while(temp != current){
                temp = temp->next;
            }
            return const_iterator(temp,tail%50);
        }



        CDAL();

        ~CDAL();

        virtual void Insert(T element, unsigned int position) override;

        virtual void push_back(T element) override;

        virtual void push_front(T element) override;

        virtual void replace(T element, unsigned int position) override;

        virtual T remove(unsigned int position) override;

        virtual T pop_back() override;

        virtual T pop_front() override;

        virtual T& item_at(unsigned int position) override;

        virtual T& peek_back() override;

        virtual T& peek_front() override;

        virtual bool is_empty() override;

        virtual bool is_full() override;

        virtual size_t length() override;

        virtual void clear() override;

        virtual bool contains(T element, bool*) override;

        virtual void print(std::ostream& theStream) override;

        virtual T *contents() override;

    };

    template<typename T>
    CDAL<T>::CDAL() {
        capacity = 50;
        tail = 0;
        head = current = nullptr;
    }

    template<typename T>
    CDAL<T>::~CDAL() {
        node *temp = head;

        while (temp) {
            node* deleteptr = temp;
            temp = temp->next;
            deleteptr->next = nullptr;
            delete deleteptr;
        }
        delete temp;
        head = current = nullptr;
        tail = 0;
    }

    template<typename T>
    void CDAL<T>::Insert(T element, unsigned int position) {

        if (is_empty()) {
            if (position == 0) {
                node *n = new node;
                n->next = nullptr;
                n->array = new T[capacity + 1];
                head = current = n;
                current->array[position] = element;
                tail = tail + 1;
            } else {
                std::runtime_error("Runtime_Error: that position is out of Bounds(1)\n");
            }
        } else if (is_full()) {
            if(current->next == nullptr){
                node* n = new node;
                n->next = nullptr;
                n->array = new T[capacity];
                current->next = n;
                current = current->next;
            }else{
                current = current->next;
            }
            node* temp = head;
            for (int i = 0; i <(position)/50 ; ++i) {
                temp = temp->next;
            }
            T temp1, temp2;
            temp1 = element;
            temp2 = temp->array[49];
            T* newArray = new T[capacity];
            for (int i = 0; i < 50 ; ++i) {
                if(i < (position)%50) newArray[i] = temp->array[i];
                if(i == (position)%50) newArray[i] = element;
                if(i > (position)%50) newArray[i] = temp->array[i - 1];
            }
            T* deletePtr = temp->array;
            temp->array = newArray;
            newArray = nullptr;
            delete [] deletePtr;
            delete [] newArray;
            temp1 = temp2;
            temp = temp->next;

            while(temp){
                if(temp == current){
                    T* newArray = new T[capacity];
                    for (int i = 0; i < 50 ; ++i) {
                        if(i < 0) newArray[i] = temp->array[i];
                        if(i == 0) newArray[i] = temp1;
                        if(i > 0) newArray[i] = temp->array[i - 1];
                    }
                    T* deletePtr = temp->array;
                    temp->array = newArray;
                    newArray = nullptr;
                    delete [] deletePtr;
                    delete [] newArray;
                }else{
                    temp2 = temp->array[49];
                    T* newArray = new T[capacity];
                    for (int i = 0; i < 50 ; ++i) {
                        if(i < 0) newArray[i] = temp->array[i];
                        if(i == 0) newArray[i] = temp1;
                        if(i > 0) newArray[i] = temp->array[i - 1];
                    }
                    T* deletePtr = temp->array;
                    temp->array = newArray;
                    newArray = nullptr;
                    delete [] deletePtr;
                    delete [] newArray;
                    temp1 = temp2;
                }
                temp = temp->next;
            }
            tail = tail + 1;
        }else {
            node* temp = head;
            for (int i = 0; i <(position)/50 ; ++i) {
                temp = temp->next;
            }

            T temp1, temp2;
            temp1 = element;
            temp2 = temp->array[49];
            T* newArray = new T[capacity];
            for (int i = 0; i < 50 ; ++i) {
                if(i < (position)%50) newArray[i] = temp->array[i];
                if(i == (position)%50) newArray[i] = element;
                if(i > (position)%50) newArray[i] = temp->array[i - 1];
            }
            T* deletePtr = temp->array;
            temp->array = newArray;
            newArray = nullptr;
            delete [] deletePtr;
            delete [] newArray;
            temp1 = temp2;
            temp = temp->next;
            while(temp){
                if(temp == current){
                    T* newArray = new T[capacity];
                    for (int i = 0; i < 50 ; ++i) {
                        if(i < 0) newArray[i] = temp->array[i];
                        if(i == 0) newArray[i] = temp1;
                        if(i > 0) newArray[i] = temp->array[i - 1];
                    }
                    T* deletePtr = temp->array;
                    temp->array = newArray;
                    newArray = nullptr;
                    delete [] deletePtr;
                    delete [] newArray;
                }else{
                    temp2 = temp->array[49];
                    T* newArray = new T[capacity];
                    for (int i = 0; i < 50 ; ++i) {
                        if(i < 0) newArray[i] = temp->array[i];
                        if(i == 0) newArray[i] = temp1;
                        if(i > 0) newArray[i] = temp->array[i - 1];
                    }
                    T* deletePtr = temp->array;
                    temp->array = newArray;
                    newArray = nullptr;
                    delete [] deletePtr;
                    delete [] newArray;
                    temp1 = temp2;
                }
                temp = temp->next;
            }
            tail = tail + 1;
        }
    }

    template<typename T>
    void CDAL<T>::push_back(T element) {}

    template<typename T>
    void CDAL<T>::push_front(T element) {}

    template<typename T>
    void CDAL<T>::replace(T element, unsigned int position) {}

    template<typename T>
    T CDAL<T>::remove(unsigned int position) {
        T returnVal;
        if( position <  0 || position >= tail ){
            throw std::runtime_error("Runtime_Error: Index is out of bounds\n");
        }else if(head == nullptr){
            throw  std::runtime_error("Runtime_Error: The list is empty, you cannot remove anything\n");
        }else {
            // create temp pointer and point it to the head
            node* temp = head;
            // Traverse the list until node where removal is taking place is reached
            for (int i = 0; i < (position)/50; ++i) {
                temp = temp->next;
            }
            T temp1;

            returnVal = temp->array[(position)%50];
            for (int i = (position) % 50; i < 50 ; ++i) {
                temp->array[i] = temp->array[i + 1];
            }

            while(temp != current){
                if(temp->next)
                    temp->array[49] = temp->next->array[0];
                temp = temp->next;
                for (int i = 0; i < 50 ; ++i) {
                    temp->array[i] = temp->array[i + 1];
                }
            }
            tail = tail - 1;

            if( tail%50 == 0 ){
                temp = head;
                while(temp->next != current){
                    temp = temp->next;
                }
                current = temp;
            }

            if(temp->next){
                temp = temp->next;
                if(temp->next){
                    node *deleteptr = temp->next;
                    temp->next = nullptr;
                    delete deleteptr;
                }
                temp = nullptr; delete temp;
            }


        }
        return returnVal;
    }

    template<typename T>
    T CDAL<T>::pop_back() {
        return remove(length()-1);
    }

    template<typename T>
    T CDAL<T>::pop_front() {
        return remove(0);
    }

    template<typename T>
    T& CDAL<T>::item_at(unsigned int position) {
        node* temp = head;
        // Traverse the list until node where removal is taking place is reached
        for (int i = 0; i < (position)/50; ++i) {
            temp = temp->next;
        }
        return temp->array[position%50];
    }

    template<typename T>
    T& CDAL<T>::peek_back() {
        if(is_empty())
            throw std::runtime_error("Runtime_Error: The list is empty, you cannot peek_back()\n");
        else return current->array[tail%50];
    }

    template<typename T>
    T& CDAL<T>::peek_front() {
        if(is_empty())
            throw std::runtime_error("Runtime_Error: The list is empty, you cannot peek_back()\n");
        else return head->array[0];
    }

    template<typename T>
    bool CDAL<T>::is_full() {
        if(tail % 50 == 0 && tail != 0) return 1;
        else return 0;
    }

    template<typename T>
    bool CDAL<T>::is_empty() {
        if (head == nullptr)return 1;
        else return 0;
    }

    template<typename T>
    size_t CDAL<T>::length() {
        return tail;
    }

    template<typename T>
    void CDAL<T>::clear() {}

    template<typename T>
    bool CDAL<T>::contains(T element, bool* equals_function) {
        return 0;
    }

    template<typename T>
    void CDAL<T>::print(std::ostream& theStream) {
        if (head == nullptr) {
            throw std::runtime_error("head is nullptr\n");
        } else {
            node *temp = head;

            while (temp != current) {
                for (int i = 0; i < 50; ++i) {
                    theStream << temp->array[i] << "\n";
                }
                temp = temp->next;
            }
            if (temp == current)
                if (tail % 50 == 0) {
                    for (int i = 0; i < 50; ++i) {
                        theStream << temp->array[i] << "\n";
                    }
                } else {
                    for (int i = 0; i < tail % 50; ++i) {
                        theStream << temp->array[i] << "\n";

                    }
                }

        }
    }

    template<typename T>
    T *CDAL<T>::contents() {
        return 0;
    }

}