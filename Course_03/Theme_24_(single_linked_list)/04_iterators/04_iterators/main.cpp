#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

#include "single_linked_list.h"

void Test2() {
    
    // ������������ �� ������� ������
    {
        SingleLinkedList<int> list;
        // ����������� ������ ��� ������� � ����������� ������� begin()/end()
        const auto& const_list = list;
        
        // ��������� begine � end � ������� ��������� ����� ���� �����
        assert(list.begin() == list.end());
        assert(const_list.begin() == const_list.end());        
        assert(list.cbegin() == list.cend());
        assert(list.cbegin() == const_list.begin());
        assert(list.cend() == const_list.end());
    }

    // ������������ �� ��������� ������
    {
        SingleLinkedList<int> list;
        const auto& const_list = list;

        list.PushFront(1);
        assert(list.GetSize() == 1u);
        assert(!list.IsEmpty());
        
        assert(const_list.begin() != const_list.end());
        assert(const_list.cbegin() != const_list.cend());
        assert(list.begin() != list.end());
        
        assert(const_list.begin() == const_list.cbegin());
        
        assert(*list.cbegin() == 1);
        
        *list.begin() = -1;
        assert(*list.cbegin() == -1);
        const auto old_begin = list.cbegin();
        list.PushFront(2);
        assert(list.GetSize() == 2);
        
        const auto new_begin = list.cbegin();
        assert(new_begin != old_begin);
        
        // �������� ��������������
        {
            auto new_begin_copy(new_begin);
            assert((++(new_begin_copy)) == old_begin);
        }
        // �������� ��������������
        {
            auto new_begin_copy(new_begin);
            assert(((new_begin_copy)++) == new_begin);
            assert(new_begin_copy == old_begin);
        }
        // ��������, ����������� �� ������� ����� ���������� �������� ����� ��������� end()
        {
            auto old_begin_copy(old_begin);
            assert((++old_begin_copy) == list.end());
        }
    }
    
    // �������������� ����������
    {
        SingleLinkedList<int> list;
        list.PushFront(1);
        // ��������������� ConstItrator �� Iterator
        SingleLinkedList<int>::ConstIterator const_it(list.begin());
        assert(const_it == list.cbegin());
        assert(*const_it == *list.cbegin());

        SingleLinkedList<int>::ConstIterator const_it1;
        // ������������ ConstIterator-� �������� Iterator
        const_it1 = list.begin();
        assert(const_it1 == const_it);
    }
    // �������� ��������� ->
    {
        using namespace std;
        SingleLinkedList<std::string> string_list;

        string_list.PushFront("one"s);
        assert(string_list.cbegin()->length() == 3u);
        string_list.begin()->push_back('!');
        assert(*string_list.begin() == "one!"s);
    }
}

int main() {
    Test2();
    return 0;
}