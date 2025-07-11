#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <vector>

#include "ptrvector.h"
#include "octopus.h"

using namespace std::literals;

int main() {
    // �������� ������������ ����������
    {
        Octopus octopus1(3);
        // ����������� ��������� ��������� ���������
        octopus1.GetTentacle(2).LinkTo(octopus1.GetTentacle(1));

        // �� ������������ octopus2 ����� ��� ����������� ���������
        Octopus octopus2(10);

        octopus2 = octopus1;

        // ����� ������������ ���������� �������� ����� ����� �� �� ���������,
        // ��� � �������� �������������� �������
        assert(octopus2.GetTentacleCount() == octopus1.GetTentacleCount());
        for (int i = 0; i < octopus2.GetTentacleCount(); ++i) {
            auto& tentacle1 = octopus1.GetTentacle(i);
            auto& tentacle2 = octopus2.GetTentacle(i);
            assert(&tentacle2 != &tentacle1);
            assert(tentacle2.GetId() == tentacle1.GetId());
            assert(tentacle2.GetLinkedTentacle() == tentacle1.GetLinkedTentacle());
        }
    }

    // �������� ���������������� ����������
    {
        Octopus octopus(3);
        // ����������� ��������� ���������
        octopus.GetTentacle(0).LinkTo(octopus.GetTentacle(1));

        std::vector<std::pair<Tentacle*, Tentacle*>> tentacles;
        // ��������� ���������� � ��������� ��������� � ��� �����
        for (int i = 0; i < octopus.GetTentacleCount(); ++i) {
            tentacles.push_back({ &octopus.GetTentacle(i), octopus.GetTentacle(i).GetLinkedTentacle() });
        }

        // ��������� ����������������
        octopus = octopus;

        // ����� ���������������� ��������� ��������� �� ������ ����������
        assert(octopus.GetTentacleCount() == static_cast<int>(tentacles.size()));
        for (int i = 0; i < octopus.GetTentacleCount(); ++i) {
            auto& tentacle_with_link = tentacles.at(i);
            assert(&octopus.GetTentacle(i) == tentacle_with_link.first);
            assert(octopus.GetTentacle(i).GetLinkedTentacle() == tentacle_with_link.second);
        }
    }
    return 0;
}