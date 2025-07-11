#include <iostream>
#include <stdexcept>
#include <vector>

class Tower {
public:
    // ����������� � ����� SetDisks �����, ����� ��������� ������� �����
    Tower(int disks_num) {
        FillTower(disks_num);
    }

    int GetDisksNum() const {
        return disks_.size();
    }

    void SetDisks(int disks_num) {
        FillTower(disks_num);
    }

    // ��������� ���� �� ���� ����������� �����
    // �������� �������� �� ����������, ������� ������������� ���� �������
    void AddToTop(int disk) {
        int top_disk_num = disks_.size() - 1;
        if (0 != disks_.size() && disk >= disks_[top_disk_num]) {
            throw std::invalid_argument("���������� ��������� ������� ���� �� ���������");
        }
        else {
            // �������� ���� ����� � ����������� ��� � ����� �������
            //TODO:+
            disks_.push_back(disk);
        }
    }

    // �� ������ ���������� ����������� ��� ������ ������� ������
    //TODO:+
    void RemFromTop() {
        if (disks_.size() == 0) {
            throw std::invalid_argument("�������� �� ����� ���� ������");
        }
        disks_.pop_back();
    }

    //TODO:
// disks_num - ���������� ������������ ������
 // destination - �������� ����� ��� �����������
 // buffer - �����, ������� ����� ������������ � �������� ������ ��� ������
    void MoveDisks(int disks_num, Tower& destination, Tower& buffer) {
        if (disks_num == 1) {
            this->RemFromTop();
            destination.AddToTop(1);
            return;
        }
        MoveDisks(disks_num - 1, buffer, destination);
        this->RemFromTop();
        destination.AddToTop(disks_num);
        buffer.MoveDisks(disks_num - 1, destination, *this);
    }

private:
    std::vector<int> disks_;

    // ���������� ��������� ����� FillTower, ����� �������� ���������� ����
    void FillTower(int disks_num) {
        for (int i = disks_num; i > 0; i--) {
            disks_.push_back(i);
        }
    }
};

//TODO: +
void SolveHanoi(std::vector<Tower>& towers) {
    int disks_num = towers[0].GetDisksNum();

    // �������� �������, ����� �� towers[0] ���� 0 ������,
    // �� towers[1] 0 ������,
    // � �� towers[2] ���� disks_num ������

    // ��������� ��������
    // ������ ���������� ��� ����� �� ��������� �����
    // � �������������� ������� ����� ��� ������
    towers[0].MoveDisks(disks_num, towers[2], towers[1]);
}

int main() {
    int towers_num = 3;
    int disks_num = 3;
    std::vector<Tower> towers;

    // ������� � ������ ��� ������ �����
    for (int i = 0; i < towers_num; ++i) {
        towers.push_back(0);
    }
    // ������� �� ������ ����� ��� ������
    towers[0].SetDisks(disks_num);

    SolveHanoi(towers);
    int x = 2;
    return 0;
}