#include <iostream>
#include <stdexcept>
#include <vector>

class Tower {
public:
    // конструктор и метод SetDisks нужны, чтобы правильно создать башни
    Tower(int disks_num) {
        FillTower(disks_num);
    }

    int GetDisksNum() const {
        return disks_.size();
    }

    void SetDisks(int disks_num) {
        FillTower(disks_num);
    }

    // добавляем диск на верх собственной башни
    // обратите внимание на исключение, которое выбрасывается этим методом
    void AddToTop(int disk) {
        int top_disk_num = disks_.size() - 1;
        if (0 != disks_.size() && disk >= disks_[top_disk_num]) {
            throw std::invalid_argument("Невозможно поместить большой диск на маленький");
        }
        else {
            // допишите этот метод и используйте его в вашем решении
            //TODO:+
            disks_.push_back(disk);
        }
    }

    // вы можете дописывать необходимые для вашего решения методы
    //TODO:+
    void RemFromTop() {
        if (disks_.size() == 0) {
            throw std::invalid_argument("Пирамида не может быть пустой");
        }
        disks_.pop_back();
    }

    //TODO:
// disks_num - количество перемещаемых дисков
 // destination - конечная башня для перемещения
 // buffer - башня, которую нужно использовать в качестве буфера для дисков
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

    // используем приватный метод FillTower, чтобы избежать дубликации кода
    void FillTower(int disks_num) {
        for (int i = disks_num; i > 0; i--) {
            disks_.push_back(i);
        }
    }
};

//TODO: +
void SolveHanoi(std::vector<Tower>& towers) {
    int disks_num = towers[0].GetDisksNum();

    // допишите функцию, чтобы на towers[0] было 0 дисков,
    // на towers[1] 0 дисков,
    // и на towers[2] было disks_num дисков

    // запускаем рекурсию
    // просим переложить все диски на последнюю башню
    // с использованием средней башни как буфера
    towers[0].MoveDisks(disks_num, towers[2], towers[1]);
}

int main() {
    int towers_num = 3;
    int disks_num = 3;
    std::vector<Tower> towers;

    // добавим в вектор три пустые башни
    for (int i = 0; i < towers_num; ++i) {
        towers.push_back(0);
    }
    // добавим на первую башню три кольца
    towers[0].SetDisks(disks_num);

    SolveHanoi(towers);
    int x = 2;
    return 0;
}