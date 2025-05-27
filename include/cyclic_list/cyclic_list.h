#pragma once

#include <iostream>
#include <stdexcept>
#include <algorithm>        
#include <iterator>        

template <typename T>
class CyclicList {
private:
    class Node {
    public:
        T data;
        Node* next;
        Node(T value = T{}, Node* next_node = nullptr) : data(value), next(next_node) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;     
    size_t size_ = 0;

    Node* getMiddle(Node* h) {
        if (h == nullptr) return h;
        Node* slow = h;
        Node* fast = h;
        while (fast->next != nullptr && fast->next->next != nullptr && fast->next != head && fast->next->next != head) {
            if (fast->next == h || (fast->next->next != nullptr && fast->next->next == h)) break;        
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    Node* sortedMerge(Node* a, Node* b, bool reverse) {
        if (a == nullptr) return b;
        if (b == nullptr) return a;

        Node* result = nullptr;
        bool compare_result;

        if (!reverse) {   
            compare_result = (a->data < b->data);       
        }
        else {   
            compare_result = (a->data > b->data);       
        }

        if (compare_result) {
            result = a;
            result->next = sortedMerge(a->next, b, reverse);
        }
        else {
            result = b;
            result->next = sortedMerge(a, b->next, reverse);
        }
        return result;
    }

    void mergeSortRecursive(Node** headRef, bool reverse) {
        Node* h = *headRef;
        Node* a;
        Node* b;

        if (h == nullptr || h->next == h) {            
            return;
        }

        Node* original_tail = this->tail;    
        if (original_tail) original_tail->next = nullptr;   

        Node* middle = getMiddleStandard(h);

        a = h;
        b = middle->next;
        middle->next = nullptr;     

        mergeSortRecursive(&a, reverse);
        mergeSortRecursive(&b, reverse);

        *headRef = sortedMerge(a, b, reverse);

        if (*headRef != nullptr) {
            this->head = *headRef;   
            Node* current = this->head;
            while (current->next != nullptr) {
                current = current->next;
            }
            this->tail = current;   
            this->tail->next = this->head;   
        }
        else {
            this->head = nullptr;
            this->tail = nullptr;
        }
    }

    Node* getMiddleStandard(Node* h_linear) {
        if (h_linear == nullptr) return h_linear;
        Node* slow = h_linear;
        Node* fast = h_linear->next;        
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }


public:
    CyclicList() = default;

    ~CyclicList() {
        clear();
    }

    CyclicList(const CyclicList& oth) : head(nullptr), tail(nullptr), size_(0) {
        if (oth.empty()) {
            return;
        }
        Node* current_oth = oth.head;
        do {
            push_back(current_oth->data);    
            current_oth = current_oth->next;
        } while (current_oth != oth.head);
    }

    CyclicList& operator=(const CyclicList& oth) {
        if (this == &oth) {
            return *this;
        }
        clear();    
        if (oth.empty()) {
            return *this;
        }
        Node* current_oth = oth.head;
        do {
            push_back(current_oth->data);    
            current_oth = current_oth->next;
        } while (current_oth != oth.head);
        return *this;
    }
    CyclicList(CyclicList&& oth) noexcept
        : head(oth.head), tail(oth.tail), size_(oth.size_) {
        oth.head = nullptr;
        oth.tail = nullptr;
        oth.size_ = 0;
    }

    CyclicList& operator=(CyclicList&& oth) noexcept {
        if (this == &oth) {
            return *this;
        }
        clear();
        head = oth.head;
        tail = oth.tail;
        size_ = oth.size_;
        oth.head = nullptr;
        oth.tail = nullptr;
        oth.size_ = 0;
        return *this;
    }


    bool empty() const {
        return (size_ == 0);
    }

    void clear() {
        if (empty()) return;

        Node* current = head->next;
        while (current != head) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        delete head;
        head = nullptr;
        tail = nullptr;    
        size_ = 0;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = newNode;
            tail = newNode;
            head->next = head;     
        }
        else {
            newNode->next = head;
            tail->next = newNode;
            tail = newNode;
        }
        size_++;
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = newNode;
            tail = newNode;
            head->next = head;
        }
        else {
            newNode->next = head;
            head = newNode;
            tail->next = head;       
        }
        size_++;
    }

    void pop_back() {      
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        size_--;
        if (size_ == 0) {
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        else {
            Node* current = head;
            while (current->next != tail) {   
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = head;
        }
    }

    void pop_front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        size_--;
        if (size_ == 0) {
            delete head;        
            head = nullptr;
            tail = nullptr;
        }
        else {
            Node* temp = head;
            head = head->next;
            tail->next = head;      
            delete temp;
        }
    }

    size_t size() const {
        return size_;
    }

    T& operator[](size_t index) {   
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    const T& operator[](size_t index) const {   
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        Node* current = head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("List is empty, cannot access back element.");
        }
        return tail->data;
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("List is empty, cannot access back element.");
        }
        return tail->data;
    }


    int find(const T& value) const {
        if (empty()) {
            return -1;
        }
        Node* curr = head;
        int index = 0;
        do {
            if (curr->data == value) return index;
            curr = curr->next;
            index++;
        } while (curr != head && index < size_);            
        return -1;
    }

    void insert(size_t index, const T& value) {  
        if (index > size_) {
            throw std::out_of_range("Index out of range for insert");
        }
        if (index == 0) {
            push_front(value);  
        }
        else if (index == size_) {
            push_back(value);  
        }
        else {
            Node* newNode = new Node(value);
            Node* current = head;
            for (size_t i = 0; i < index - 1; i++) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
            size_++;
        }
    }

    void erase(size_t index) {  
        if (index >= size_ || empty()) {    
            throw std::out_of_range("Index out of range for erase or list is empty");
        }
        if (size_ == 1) {      
            pop_front();     
            return;
        }
        if (index == 0) {
            pop_front();  
        }
        else {              
            Node* current = head;
            for (size_t i = 0; i < index - 1; i++) {
                current = current->next;
            }
            Node* temp = current->next;
            current->next = temp->next;
            if (temp == tail) {      
                tail = current;
            }
            delete temp;
            size_--;
        }
    }

    void sort(const bool reverse = false) {
        if (size_ < 2) {
            return;
        }
        mergeSortRecursive(&head, reverse);
    }


    void reverse() {  
        if (size_ < 2) {
            return;
        }
        Node* prev = tail;      
        Node* current = head;
        Node* next_node = nullptr;

        Node* old_head = head;        

        for (size_t i = 0; i < size_; ++i) {
            next_node = current->next;
            current->next = prev;
            prev = current;
            current = next_node;
        }
        head = prev;         
        tail = old_head;      
    }


    size_t count(const T& value) const {  
        if (empty()) return 0;
        size_t n = 0;
        Node* current = head;
        for (size_t i = 0; i < size_; ++i) {     
            if (current->data == value) {
                n++;
            }
            current = current->next;
        }
        return n;
    }

    void print() const {  
        if (empty()) {
            std::cout << "(empty)" << std::endl;
            return;
        }
        Node* current = head;
        for (size_t i = 0; i < size_; ++i) {
            std::cout << current->data << (i == size_ - 1 ? "" : " -> ");
            current = current->next;
        }
        std::cout << " (loops to head)" << std::endl;
    }
    const Node* get_head_node() const { return head; }
    Node* get_head_node() { return head; }

};