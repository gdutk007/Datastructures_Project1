#pragma once
#include <iostream>
#include <stdexcept>
#include "List.h"

namespace cop3530 {
    template<typename T>
    class SSLL : public List<T> {
    private:
        struct node{
            T data;
            node* next;
        };
        node* head;
        node* tail;
    public:

        void f() const { std::cout << "asdfasdf" << std::endl; }
        template <typename DataT>
        class SSLL_Iter: public std::iterator<std::forward_iterator_tag,DataT>{
        public:
            // type aliases required for C++ iterator compatibility
            typedef DataT value_type;
            typedef DataT& reference ;
            typedef DataT* pointer;
            typedef std::ptrdiff_t difference_type ;
            typedef std::forward_iterator_tag iterator_category;

            // type aliases for prettier code
            typedef SSLL_Iter self_type;
            typedef SSLL_Iter& self_reference;

        private:
            node* iterator;

        public:
            explicit SSLL_Iter(node *start = nullptr) : iterator(start) {}

            SSLL_Iter(const SSLL_Iter &src) : iterator(src.iterator) {}


            reference const operator*() const {
                return iterator->data;
            }

            pointer operator->() const {
                return iterator;
            }

            self_reference operator=(SSLL_Iter<DataT> const &src) {
                iterator = src.iterator;
                return *this;
            }

            self_reference operator++() {
                iterator = iterator->next;
                return *this;
            } // preincrement

            self_type operator++(int) {
                SSLL_Iter tmp(*this);
                iterator = iterator->next;
                return tmp;
            } // postincrement

            bool operator==(SSLL_Iter<DataT> const &rhs) const {
                return iterator == rhs.iterator;
            }

            bool operator!=(SSLL_Iter<DataT> const &rhs) const {
                return iterator != rhs.iterator;
            }
        }; // end SSLL_Iter

    public:
        typedef std::size_t size_t;
        typedef T value_type;
        typedef SSLL_Iter<T> iterator;
        typedef SSLL_Iter<T const> const_iterator;

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
        SSLL(){
            head = tail = nullptr;
        }
        /// This is the copy constructor for the SSLL class
        SSLL(const SSLL& src){
            head = tail = nullptr;
            node* temp = src.head;
            while(temp){
                push_back(temp->data);
                temp = temp->next;
            }
        }
        /// The next method will overload the assignment operator according to the rule of three/five
        SSLL& operator=(const SSLL & src){
            if(this == &src){
                return *this;
            }
            clear();
            node* temp = src.head;
            while(temp){
                push_back(temp->data);
                temp = temp->next;
            }
            return *this;
        }
        ///Destructor
        ~SSLL(){
            node* temp = head;
            while(temp){
                node* deletePtr = temp;
                temp = temp->next;
                delete deletePtr;
            }
            head = tail = nullptr;
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
        virtual bool contains(T element,  bool* )override;
        virtual void print(std::ostream& theStream)override;
        virtual T* contents()override;
        // The nested class will begin here

    };

    template<typename T>
    void SSLL<T>::Insert(T element, unsigned int position){
        if(is_empty()){
            node* n = new node;
            n->data = element;
            n->next = nullptr;
            head = tail = n;
        }else if(head == nullptr && tail == nullptr && position - 1 == 0 ){
            node* n = new node;
            n->data = element;
            n->next = nullptr;
            head = tail = n;
        }else{
            if(position == 0 || position > length()+1){
                throw std::runtime_error("That position is out of bounds\n");
                // remember to add exception for whenever the lsit becomes too full;
            }else{
                node* temp = head;
                if(position - 1 == 0){
                    node* n = new node;
                    n->data = element;
                    n->next = temp;
                    head = n;
                }else{
                    temp = head;
                    for (int i = 0; i < position - 1 ; i++) {
                        if(i+1 == position - 1 && temp->next != nullptr){
                            node* n = new node;
                            n->next = temp->next;
                            n->data = element;
                            temp->next = n;
                            break;
                        }else if(temp->next == nullptr){
                            // add the condition which will add a new node at the end of the list!!!!
                            node* n = new node;
                            n->next = nullptr;
                            n->data = element;
                            temp->next = n;
                            tail = tail->next;
                        }
                        temp = temp->next;
                    }
                    temp = nullptr;
                    delete temp;
                }

            }
        }
    }

    template<typename T>
    void SSLL<T>::push_back(T element){
        Insert(element,length());
    }

    template<typename T>
    void SSLL<T>::push_front(T element){
        Insert(element,1);
    }

    template<typename T>
    void SSLL<T>::replace(T element, unsigned int position){
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
    T SSLL<T>::remove(unsigned int position){
        if(is_empty()){
            throw std::runtime_error("Runtime error; the list is empty\n");
        }else if(position == 0){
            throw std::runtime_error("Runtime error; position cannot be 0\n");
        }else if(position > length()){
            throw std::runtime_error("Runtime error; position is out of bounds\n");
        }else{
            // if user input is correct, code will attempt to replace data here
            node* temp = head;
            T temp1;
            if(position - 1 == 0){
                head = head->next;
                temp1 = temp->data;
                temp->next = nullptr;
                delete temp;
            }else{

                for (int i = 0; i < position - 1; ++i) {
                    if(i + 1 == position - 1){
                        node* deletePtr = temp->next;
                        temp1 = deletePtr->data;
                        temp->next = temp->next->next;
                        deletePtr->next = nullptr;
                        temp = nullptr;
                        delete deletePtr, temp;
                        break;
                    }
                    temp = temp->next;
                }

            }
            return temp1;
        }
    }

    template<typename T>
    T SSLL<T>::pop_back(){
        return remove(length());
    }

    template<typename T>
    T SSLL<T>::pop_front(){

        return remove(1);
    }

    template<typename T>
    T& SSLL<T>::item_at(unsigned int position){
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
    T& SSLL<T>::peek_back(){

        return tail->data;
    }

    template<typename T>
    T& SSLL<T>::peek_front(){

        return head->data;
    }

    template<typename T>
    bool SSLL<T>::is_empty(){
        if(head == nullptr){
            return 1;
        }else{
            return 0;
        }
    }

    template<typename T>
    bool SSLL<T>::is_full(){
        return 0;
    }

    template<typename T>
    size_t SSLL<T>::length(){
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
    void SSLL<T>::clear(){
        if(is_empty()){
            throw std::runtime_error("Runtime error; the list is already empty\n");
        }else{
            node* temp = head;
            while(temp){
                node* deletePtr = temp;
                temp = temp->next;
                head = temp;
                deletePtr->next = nullptr;
                delete deletePtr;
            }
            head = tail = nullptr;
            std::cout<< "The list has been cleared\n";
        }
    }

    template<typename T>
    bool SSLL<T>::contains(T element, bool*){

        return 0;
    }

    template<typename T>
    void SSLL<T>::print(std::ostream& theStream){
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
    T* SSLL<T>::contents(){
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