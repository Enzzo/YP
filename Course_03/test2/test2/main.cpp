#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

// ��������
class Tentacle {
public:
    explicit Tentacle(int id)
        : id_(id) {
    }

    int GetId() const {
        return id_;
    }

private:
    int id_ = 0;
};

// ��������
class Octopus {
public:
    Octopus() {
        Tentacle* t = nullptr;
        try {
            for (int i = 1; i <= 8; ++i) {
                t = new Tentacle(i);      // ����� ��������� ���������� bad_alloc
                tentacles_.push_back(t);  // ����� ��������� ���������� bad_alloc

                // �������� ��������� �� ��������, ������� ��� �������� � tentacles_,
                // ����� �� ������� ��� � ����������� catch ��������
                t = nullptr;
            }
        }
        catch (const std::bad_alloc&) {
            // ������� ��������, ������� ������ ������� � ��������� tentacles_
            Cleanup();
            // ������� ��������, ������� �������, �� �� �������� � tentacles_
            delete t;
            // ����������� �� ���� ������� ��������� � ������� ����������,
            // ������� ����������� ����������, ����� �������� ����������� ���� �� ������
            // throw ��� ���������� ������ catch ��������� ���������� ���������� ����������
            throw;
        }
    }

    const Tentacle& GetTentacle(int index) const {
        if (index < 0 || static_cast<size_t>(index) >= tentacles_.size()) {
            throw std::out_of_range("Invalid tentacle index"s);
        }
        // ����� ���������� ��������� � ������, �������������� ���
        return *tentacles_[index];
    }

    ~Octopus() {
        // �������� ������� ��������� � ������������ ������ (��������),
        // ������� ������ ���� ������� ��� ��� ����������.
        // ���������� - ������ �����, ����� ���������� �� �����.
        Cleanup();
    }

private:
    void Cleanup() {
        // ������� �������� ��������� �� ������������ ������
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        // ������� ������ ���������� �� ��������
        tentacles_.clear();
    }

    // ������ ������ ��������� �� ��������. ���� ������� ������� ��������� � ����
    std::vector<Tentacle*> tentacles_;
};

int main() {
    {
        Octopus octopus;
        // �� ������ ������ ��� ������ ���������
        Octopus octopus1 = octopus;
        std::cout << "Tentacle id="s << octopus1.GetTentacle(3).GetId() << std::endl;
    }
    std::cout << "OK"s << std::endl;
}