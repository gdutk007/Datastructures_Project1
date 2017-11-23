#pragma once
#include <iostream>
#include <stdexcept>
#include "List.h"

namespace cop3530 {
    template<typename T>
    class PSLL : public List<T> {
    private:
        struct node{
            T data;
            node* next;
        };
        node* head;
        node* tail;
        node* free;
    public:

        template <typename DataT>
        class PSLL_Iter: public std::iterator<std::forward_iterator_tag,DataT>{
        public:
            // type aliases required for C++ iterator compatibility
            typedef DataT value_type;
            typedef DataT& reference ;
            typedef DataT* pointer;
            typedef std::ptrdiff_t difference_type ;
            typedef std::forward_iterator_tag iterator_category;

            // type aliases for prettier code
            typedef PSLL_Iter self_type;
            typedef PSLL_Iter& self_reference;

        private:
            node* iterator;

        public:
            explicit PSLL_Iter(node *start = nullptr) : iterator(start) {}

            PSLL_Iter(const PSLL_Iter &src) : iterator(src.iterator) {}


            reference const operator*() const {
                return iterator->data;
            }

            pointer operator->() const {
                return iterator;
            }

            self_reference operator=(PSLL_Iter<DataT> const &src) {
                iterator = src.iterator;
                return *this;
            }

            self_reference operator++() {
                iterator = iterator->next;
                return *this;
            } // preincrement

            self_type operator++(int) {
                PSLL_Iter tmp(*this);
                iterator = iterator->next;
                return tmp;
            } // postincrement

            bool operator==(PSLL_Iter<DataT> const &rhs) const {
                return iterator == rhs.iterator;
            }

            bool operator!=(PSLL_Iter<DataT> const &rhs) const {
                return iterator != rhs.iterator;
            }
        }; // end SSLL_Iter

    public:
        typedef std::size_t size_t;
        typedef T value_type;
        typedef PSLL_Iter<T> iterator;
        typedef PSLL_Iter<T const> const_iterator;

        // This is the begin function which will return the head of the list when the iterator is used
        iterator Begin() {
            return iterator(head);
        }

        // This is the End() function, returns the spot after the end of the tail
        iterator End() {
            return iterator(tail->next);
        }

        // This is the begin function which will return the head of the list when the iterator is used
        const_iterator Begin() const {
            return const_iterator(head);
        }
        // This is the End() function, returns the spot after the end of the tail
        const_iterator End() const{
            return const_iterator(tail->next);
        }

        /// This is the constructor for the SSLL class
        PSLL(){
            head = tail = free = nullptr;
        }
        /// This is the copy constructor for the SSLL class
        PSLL(const PSLL& src){
            head = tail = nullptr;
            free = src.free;
            node* temp = src.head;
            while(temp){
                push_back(temp->data);
                temp = temp->next;
            }
        }
        /// The next method will overload the assignment operator according to the rule of three/five
        PSLL& operator=(const PSLL & src){
            if (&src == this) // check for self-assignment
                return *this; // do nothing

            delete this;
            node* temp = src.head;
            while (temp) {
                push_back(temp->data);
                temp = temp->next;
            }
            return *this;
        }
        ///Destructor
        ~PSLL(){
            node* temp = head;
            while (temp) {
                node* deleteptr = temp;
                temp = temp->next;
                delete deleteptr;
            }
            head = nullptr;
            node* poolTemp = free;
            while (poolTemp) {
                node* next = poolTemp->next;
                delete poolTemp;
                poolTemp = next;
            }
            free = nullptr;
        }

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
        virtual bool contains(T element, bool* equalsFunction)override;
        virtual void print(std::ostream& )override;
        virtual T* contents()override;
    };

    template<typename T>
    void PSLL<T>::Insert(T element, unsigned int position){
        if(position < 0){
            throw std::runtime_error("Runtime error; position cannot be zero\n");
        }else if(position > length()){
            throw std::runtime_error("Runtime error; that index is out of bounds\n");
        }else if(is_empty() && free == nullptr){
            node* n = new node;
            n->data = element;
            n->next = nullptr;
            head = tail = n;
        }else if(is_empty() && free){
            head = tail = free;
            free = free->next;
            head->data = element;
            head->next = nullptr;
        }else if(free){
            node* temp = head;
            if(position  == 0){
                tail->next = free;
                free = free->next;
                tail->next->next = head;
                head = tail->next;
                tail->next = nullptr;
                head->data = element;
            }else{
                node* temp = head;
                for (int i = 0; i < position; ++i) {
                    if(i+1 == position - 1){
                        tail->next = free;
                        free = free->next;
                        tail->next->next = temp->next;
                        temp->next = tail->next;
                        temp->next->data = element;
                        tail->next = nullptr;
                        break;
                    }
                    temp = temp->next;
                }
                temp = nullptr;
                delete temp;
            }
        }else{
            node* temp = head;
            if(position  == 0){
                node *n = new node;
                n->data = element;
                n->next = head;
                head = n;
            }else {
                for (int i = 0; i < position ; ++i) {
                    if (i + 1 == position ) {
                        node *n = new node;
                        n->data = element;
                        n->next = temp->next;
                        temp->next = n;
                        break;
                    }
                    temp = temp->next;
                }
                temp = nullptr;
                delete temp;
            }
        }
    }

    template<typename T>
    void PSLL<T>::push_back(T element){
        Insert(element,length());
    }

    template<typename T>
    void PSLL<T>::push_front(T element){
        Insert(element,0);
    }

    template<typename T>
    void PSLL<T>::replace(T element, unsigned int position){
        if(is_empty()){
            throw std::runtime_error("Runtime error; the list is empty\n");
        }else if(position == 0){
            throw std::runtime_error("Runtime error; position cannot be 0\n");
        }else if(position > length()){
            throw std::runtime_error("Runtime error; position is out of bounds\n");
        }else{
            // is the position-1 i equal to zero we will just replace whatever is at the head node;
            if(position -1 == 0){
                head->data = element;
            }else{// if position-1 is not equal to zero we will iterate until we find what we need;
                node* temp = head;
                for (int i = 0; i < position - 1; ++i) {
                    if(i+1 == position-1){
                        temp->next->data = element;
                        temp = nullptr;
                        delete temp;
                        break;
                    }
                    temp = temp->next;
                }
            }
        }
    }

    template<typename T>
    T PSLL<T>::remove(unsigned int position){
        T temp1;
        if(is_empty()){
            throw std::runtime_error("Runtime error; The list is empty, nothing can be removed\n");
        }else if(position > length()){
            throw std::runtime_error("Runtime error; that position is out of bounds\n");
        }else{
            if(position-1 == 0){
                node* temp = head;
                temp1 = head->data;
                head = head->next;
                temp->next = free;
                free = temp;
                temp = nullptr;
                delete temp;
            }else{
                node* temp = head;
                for (int i = 0; i <position-1 ; ++i) {
                    if(i+1 == position -1){
                        temp1 = temp->next->data;
                        node* deleteptr = temp->next;
                        temp->next = temp->next->next;
                        deleteptr->next = free;
                        free = deleteptr;
                        deleteptr = nullptr;
                        delete deleteptr;
                        break;
                    }
                    temp = temp->next;
                }
            }
        }
        // The following if-statemet will remove the excess unused nodes from the pool


        node* temp = free;
        size_t  freeCount = 0;
        while(temp){
            freeCount = freeCount + 1;
            temp = temp->next;
        }
        temp = nullptr;
        delete temp;
        // freeCount = freeCount + 1; // actual value of the count
        if(freeCount > 50){
            node* deleteptr = free;
            free = free->next;
            deleteptr->next = nullptr;
            delete deleteptr;
        }
        return temp1;
    }

    template<typename T>
    T PSLL<T>::pop_back(){
        return remove(length());
    }

    template<typename T>
    T PSLL<T>::pop_front(){

        return remove(1);
    }

    template<typename T>
    T& PSLL<T>::item_at(unsigned int position){
        if(is_empty()){
            throw std::runtime_error("Runtime error; the list is empty, there are no items on the list\n");
        }else if(position == 0){
            throw std::runtime_error("Runtime error; position cant be zero\n");
        }else if(position > length()){
            throw std::runtime_error("Runtime error; That position is out of bounds\n");
        }else{
            if(position - 1 == 0){
                return head->data;
            }else{
                node* temp = head;
                for (int i = 0; i < position-1; ++i) {
                    if(i+1 == position - 1){
                        return temp->next->data;
                    }
                    temp = temp->next;
                }
            }
        }
    }

    template<typename T>
    T& PSLL<T>::peek_back(){

        return tail->data;
    }

    template<typename T>
    T& PSLL<T>::peek_front(){

        return head->data;
    }

    template<typename T>
    bool PSLL<T>::is_empty(){
        if(head == nullptr){
            return 1;
        }else{
            return 0;
        }
    }

    template<typename T>
    bool PSLL<T>::is_full(){
        if(free == nullptr){
            return 1;
        }else{
            return 0;
        }
    }

    template<typename T>
    size_t PSLL<T>::length(){
        node* temp = head;
        size_t length = 0;
        if(temp == nullptr){
            length = 0;
            return length;
        }else{
            while(temp){
                length = length + 1;
                temp = temp->next;
            }
            return length;
        }
    }

    template<typename T>
    void PSLL<T>::clear(){
        if(is_empty()){
            throw std::runtime_error("Runtime error; the list is already empty\n");
        }else{
            unsigned int length1 = length();
            for (int i = 0; i < length1 ; ++i) {
                remove(1);
            }
        }
    }

    template<typename T>
    bool PSLL<T>::contains(T element, bool* equalsFunction){

        return 0;
    }

    template<typename T>
    void PSLL<T>::print(std::ostream& theStream){
        if(head == nullptr){
            throw std::runtime_error("Runtime error; there is nothing on the list so could not print out values\n");
        }else{
            node* temp = head;
            while(temp){
                theStream << temp->data << "\n";
                temp = temp->next;
            }
        }

    }

    template<typename T>
    T* PSLL<T>::contents(){
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
