
#ifndef SSLL_LIST_H
#define SSLL_LIST_H

namespace cop3530 {
    template <typename T>
    class List {
    public:
        virtual void Insert(T element, unsigned int position) = 0;
        virtual void push_back(T element) = 0;
        virtual void push_front(T element) = 0;
        virtual void replace(T element, unsigned int position) = 0;
        virtual T remove(unsigned int position) = 0;
        virtual T pop_back() = 0;
        virtual T pop_front() = 0;
        virtual T& item_at(unsigned int position) = 0;
        virtual T& peek_back() = 0;
        virtual T& peek_front() = 0;
        virtual bool is_empty() = 0;
        virtual bool is_full() = 0;
        virtual size_t length() = 0;
        virtual void clear() = 0;
        virtual bool contains(T element, bool*) = 0;
        virtual void print(std::ostream&) = 0;
        virtual T* contents() = 0;
    };




}

#endif //SDAL_LIST_LIST_H
