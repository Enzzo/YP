#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

#include "single_linked_list.h"

void Test1() {
    // �����, �������� �� ����� ���������
    struct DeletionSpy {
        DeletionSpy() = default;
        explicit DeletionSpy(int& instance_counter) noexcept
            : instance_counter_ptr_(&instance_counter)  //
        {
            OnAddInstance();
        }
        DeletionSpy(const DeletionSpy& other) noexcept
            : instance_counter_ptr_(other.instance_counter_ptr_)  //
        {
            OnAddInstance();
        }
        DeletionSpy& operator=(const DeletionSpy& rhs) noexcept {
            if (this != &rhs) {
                auto rhs_copy(rhs);
                std::swap(instance_counter_ptr_, rhs_copy.instance_counter_ptr_);
            }
            return *this;
        }
        ~DeletionSpy() {
            OnDeleteInstance();
        }

    private:
        void OnAddInstance() noexcept {
            if (instance_counter_ptr_) {
                ++(*instance_counter_ptr_);
            }
        }
        void OnDeleteInstance() noexcept {
            if (instance_counter_ptr_) {
                assert(*instance_counter_ptr_ != 0);
                --(*instance_counter_ptr_);
            }
        }

        int* instance_counter_ptr_ = nullptr;
    };

    // �������� ������� � ������
    {
        SingleLinkedList<int> l;
        assert(l.IsEmpty());
        assert(l.GetSize() == 0u);

        l.PushFront(0);
        l.PushFront(1);
        assert(l.GetSize() == 2);
        assert(!l.IsEmpty());

        l.Clear();
        assert(l.GetSize() == 0);
        assert(l.IsEmpty());
    }

    // �������� ������������ �������� ���������
    {
        int item0_counter = 0;
        int item1_counter = 0;
        int item2_counter = 0;
        {
            SingleLinkedList<DeletionSpy> list;
            list.PushFront(DeletionSpy{ item0_counter });
            list.PushFront(DeletionSpy{ item1_counter });
            list.PushFront(DeletionSpy{ item2_counter });

            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
            list.Clear();
            assert(item0_counter == 0);
            assert(item1_counter == 0);
            assert(item2_counter == 0);

            list.PushFront(DeletionSpy{ item0_counter });
            list.PushFront(DeletionSpy{ item1_counter });
            list.PushFront(DeletionSpy{ item2_counter });
            assert(item0_counter == 1);
            assert(item1_counter == 1);
            assert(item2_counter == 1);
        }
        assert(item0_counter == 0);
        assert(item1_counter == 0);
        assert(item2_counter == 0);
    }

    // ��������������� �����, ��������� ���������� ����� �������� N-�����
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                }
                else {
                    --(*countdown_ptr);
                }
            }
        }
        // ������������ ��������� ����� ���� �� ���������
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // ����� �������� ��������� �������. ���� �� ����� nullptr, �� ����������� ��� ������ �����������.
        // ��� ������ ���������, ����������� ����������� �������� ����������
        int* countdown_ptr = nullptr;
    };

    {
        bool exception_was_thrown = false;
        // ��������������� ��������� ������� ����������� �� ����, ���� �� ����� ��������� ����������
        for (int max_copy_counter = 5; max_copy_counter >= 0; --max_copy_counter) {
            // ������ �������� ������
            SingleLinkedList<ThrowOnCopy> list;
            list.PushFront(ThrowOnCopy{});
            try {
                int copy_counter = max_copy_counter;
                list.PushFront(ThrowOnCopy(copy_counter));
                // ���� ����� �� �������� ����������, ������ ������ ������� � ����� ���������
                assert(list.GetSize() == 2);
            }
            catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                // ����� ������������ ���������� ��������� ������ ������ �������� �������
                assert(list.GetSize() == 1);
                break;
            }
        }
        assert(exception_was_thrown);
    }
}

int main() {

    Test1();
    return 0;
}