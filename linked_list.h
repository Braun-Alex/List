#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <cstdint>
#include <utility>
#include <functional>
template<typename Type>
class LinkedList
{
public:
    struct Node
    {
        Type value=Type();
        Node* next=nullptr;
    };
    struct LinkedIterator
    {
        void operator=(LinkedIterator another_iterator) noexcept
        {
            node=another_iterator.node;
        }
        void operator++() noexcept
        {
            node=node->next;
        }
        Type &operator*() noexcept
        {
            return node->value;
        }
        const Type &operator*() const noexcept
        {
            return node->value;
        }
        bool operator!=(LinkedIterator another_iterator) const noexcept
        {
            return node!=another_iterator.node;
        }
        Node* GetNode() const noexcept
        {
            return node;
        }
        Node* node=nullptr;
    };
    LinkedList() noexcept {}
    LinkedList(const Type &entered_value) noexcept
    {
        PushFront(entered_value);
    }
    LinkedList(Type &&entered_value) noexcept
    {
        PushFront(std::move(entered_value));
    }
    LinkedList(std::initializer_list<Type> entered_list) noexcept
    {
        for (auto the_iterator=entered_list.begin(); the_iterator!=entered_list.end(); ++the_iterator)
            PushFront(*the_iterator);
    }
    LinkedList(const LinkedList<Type> &another_list) noexcept
    {
        LinkedList<Type> copy;
        for (const auto &node_value:another_list) copy.PushFront(node_value);
        for (auto the_iterator=copy.begin(); the_iterator!=copy.end(); ++the_iterator)
            PushFront(std::move(*the_iterator));
    }
    LinkedList(LinkedList<Type> &&another_list) noexcept
    {
        head=another_list.head;
        size=another_list.size;
        another_list.head=nullptr;
        another_list.size=0;
    }
    template<typename ForwardIterator>
    LinkedList(ForwardIterator entered_begin,ForwardIterator entered_end) noexcept
    {
        for (auto the_iterator=entered_begin; the_iterator!=entered_end; ++the_iterator)
        {
            PushFront(*the_iterator);
        }
    }
    ~LinkedList() noexcept
    {
        while (head!=nullptr) PopFront();
    }
    void operator=(const LinkedList<Type> &another_list) noexcept
    {
    if (head==another_list.GetHead()) return;
    while (head!=nullptr) PopFront();
    LinkedList<Type> copy;
    for (const auto &node_value:another_list) copy.PushFront(node_value);
    for (auto the_iterator=copy.begin(); the_iterator!=copy.end(); ++the_iterator)
            PushFront(std::move(*the_iterator));
    }
    void operator=(LinkedList<Type> &&another_list) noexcept
    {
    if (head==another_list.GetHead()) return;
    while (head!=nullptr) PopFront();
    head=another_list.head;
    size=another_list.size;
    another_list.head=nullptr;
    another_list.size=0;
    }
    void PushFront(const Type &entered_value) noexcept
    {
        Node* new_head=new Node;
        new_head->next=head;
        head=new_head;
        head->value=entered_value;
        ++size;
    }
    void PushFront(Type &&entered_value) noexcept
    {
        Node* new_head=new Node;
        new_head->next=head;
        head=new_head;
        head->value=std::move(entered_value);
        ++size;
    }
    void InsertAfter(Node* entered_node,const Type &entered_value) noexcept
    {
        if (entered_node==nullptr) PushFront(entered_value);
        else
        {
            Node* new_value=new Node;
            new_value->next=entered_node->next;
            entered_node->next=new_value;
            new_value->value=entered_value;
            ++size;
        }
    }
    void InsertAfter(Node* entered_node,Type &&entered_value) noexcept
    {
        if (entered_node==nullptr) PushFront(std::move(entered_value));
        else
        {
            Node* new_value=new Node;
            new_value->next=entered_node->next;
            entered_node->next=new_value;
            new_value->value=std::move(entered_value);
            ++size;
        }
    }
    void RemoveAfter(Node* entered_node) noexcept
    {
        if (entered_node==nullptr) PopFront();
        else if (entered_node->next==nullptr) return;
        else
        {
            Node* copy=entered_node->next;
            entered_node->next=entered_node->next->next;
            delete copy;
            --size;
        }
    }
    void PopFront() noexcept
    {
        if (head==nullptr) return;
        Node* copy=head;
        head=head->next;
        delete copy;
        --size;
    }
    Node* GetHead() noexcept
    {
        return head;
    }
    const Node* GetHead() const noexcept
    {
        return head;
    }
    size_t Size() const noexcept
    {
        return size;
    }
    LinkedIterator begin() noexcept
    {
        return LinkedIterator {head};
    }
    LinkedIterator end() noexcept
    {
        return LinkedIterator {nullptr};
    }
    LinkedIterator begin() const noexcept
    {
        return LinkedIterator {head};
    }
    LinkedIterator end() const noexcept
    {
        return LinkedIterator {nullptr};
    }
    LinkedList<Type> SplitWhen(std::function<bool(const Type &value)> predicate) noexcept
    {
        bool does_exist=false;
        LinkedList<Type> reversed_result;
        LinkedIterator the_iterator=begin();
        for (; the_iterator!=end(); ++the_iterator)
            if (predicate(*the_iterator))
            {
                does_exist=true;
                break;
            }
            else
            {
                reversed_result.PushFront(std::move(*the_iterator));
                PopFront();
            }
        if (does_exist)
        {
            reversed_result.PushFront(std::move(*the_iterator));
            PopFront();
        }
        LinkedList<Type> straight_result;
        for (the_iterator=reversed_result.begin(); the_iterator!=reversed_result.end();
        ++the_iterator) straight_result.PushFront(std::move(*the_iterator));
        return straight_result;
    }
private:
    Node* head=nullptr;
    size_t size=0;
};
#endif