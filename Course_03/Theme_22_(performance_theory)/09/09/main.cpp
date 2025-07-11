#include <algorithm>
#include <deque>
#include <string>
#include <iostream>

struct Ticket {
    int id;
    std::string name;
};

class TicketOffice {
    int last_id_ = 0;
    std::deque<Ticket> tickets_;

public:
    // �������� ����� � �������
    inline void PushTicket(const std::string& name) {
        // ���������� �����
        tickets_.push_back({ last_id_++, name });
    }

    // �������� ���������� ��������� �������
    inline int GetAvailable() const {
        // ���������� �����
        return tickets_.size();
    }

    // �������� ���������� ��������� ������� ������������ ����
    inline int GetAvailable(const std::string& name) const {
        // ���������� �����
        return std::count_if(tickets_.begin(), tickets_.end(), [name](const Ticket& t) {
            return t.name == name;
            });
    }

    // �������� ������ ������ (�� ������������ id)
    void Invalidate(int minimum) {
        // ���������� �����
        while (tickets_.front().id < minimum) {
            tickets_.pop_front();
        }
    }
};

int main() {

    TicketOffice office;

    office.PushTicket("t1");
    office.PushTicket("t2");
    office.PushTicket("t3");
    office.PushTicket("t3");
    office.PushTicket("t4");

    std::cout << office.GetAvailable();
    std::cout << office.GetAvailable("t3");

    office.Invalidate(3);

    int x = 5;

    return 0;
}