#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
	NEW,          // новая
	IN_PROGRESS,  // в разработке
	TESTING,      // на тестировании
	DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = std::map<TaskStatus, int>;

void rem_zero(TasksInfo& tasks_with_zero) {
	std::vector<TaskStatus> task_to_del;
	for (auto& tasks : tasks_with_zero) {
		if (tasks.second == 0) {
			task_to_del.push_back(tasks.first);
		}
	}
	for (auto& status : task_to_del) {
		tasks_with_zero.erase(status);
	}
}

class TeamTasks {
	std::map<std::string, TasksInfo> team;
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const std::string& person)const {			
		return team.at(person);
	}
	
	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const std::string& person) {

		team[person][TaskStatus::NEW]++;
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {

		if (team.count(person) == 0) {
			TasksInfo empty;
			return { empty, empty };
		}

		int total_tasks = team[person][TaskStatus::NEW] + team[person][TaskStatus::IN_PROGRESS] + team[person][TaskStatus::TESTING];
		
		if (task_count > total_tasks) task_count = total_tasks;

		TasksInfo upd, untch = team.at(person);

		while (task_count-- > 0) {

			if (untch[TaskStatus::NEW] > 0) {
				untch[TaskStatus::NEW]--;
				upd[TaskStatus::IN_PROGRESS]++;
			}
			else if (untch[TaskStatus::IN_PROGRESS] > 0) {
				untch[TaskStatus::IN_PROGRESS]--;
				upd[TaskStatus::TESTING]++;
			}
			else if (untch[TaskStatus::TESTING] > 0) {
				untch[TaskStatus::TESTING]--;
				upd[TaskStatus::DONE]++;
			}
			else
				break;
		}

		team[person][TaskStatus::NEW] = untch[TaskStatus::NEW] + upd[TaskStatus::NEW];
		team[person][TaskStatus::IN_PROGRESS] = untch[TaskStatus::IN_PROGRESS] + upd[TaskStatus::IN_PROGRESS];
		team[person][TaskStatus::TESTING] = untch[TaskStatus::TESTING] + upd[TaskStatus::TESTING];
		team[person][TaskStatus::DONE] = untch[TaskStatus::DONE] + upd[TaskStatus::DONE];
		untch.erase(TaskStatus::DONE);
		rem_zero(team.at(person));
		rem_zero(upd);
		return tie(upd, untch);
	}
};
// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
	std::cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
		", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
		", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
		", " << tasks_info[TaskStatus::DONE] << " tasks are done" << std::endl;
}

int main() {
	TeamTasks tasks;
	tasks.AddNewTask("Ilia");
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan");
	}
	std::cout << "Ilia's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
	std::cout << "Ivan's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);
}