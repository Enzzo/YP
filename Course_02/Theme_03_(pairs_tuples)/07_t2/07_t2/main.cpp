#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>

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

class TeamTasks {
	std::map<std::string, TasksInfo> team;

public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
		return team.at(person);
	}

	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const std::string& person) {
		team[person][TaskStatus::NEW]++;
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {
		TasksInfo upd, untch;

		//Если разработчика с именем person нет, 
		//метод PerformPersonTasks должен вернуть кортеж из двух пустых TasksInfo.
		if(!team.count(person))
		return std::tie(upd, untch);

		//1 РАССМОТРЕТЬ ВСЕ НЕВЫПОЛНЕННЫЕ ЗАДАЧИ РАЗРАБОТЧИКА
		untch = team.at(person);

		//2 Упорядочить их по статусам: сначала все задачи в статусе NEW, 
		//затем все задачи в статусе IN_PROGRESS и, наконец, задачи в статусе TESTING.

		//количество текущих задач:
		int total_tasks = 0;
		for (const auto& [status, tasks] : untch) {
			total_tasks += (status != TaskStatus::DONE) ? tasks : 0;
		}
		if (total_tasks > task_count)total_tasks = task_count;

		//3 Рассмотреть первые task_count задач и перевести каждую из них в следующий статус 
		//в соответствии с естественным порядком: NEW → IN_PROGRESS → TESTING → DONE.
		for (int s = static_cast<int>(TaskStatus::NEW); s < static_cast<int>(TaskStatus::DONE); ++s) {
			const TaskStatus crnt = static_cast<TaskStatus>(s);
			const TaskStatus next = static_cast<TaskStatus>(s + 1);

			while (task_count > 0) {
				if (untch[crnt] > 0) {
					untch[crnt]--;
					upd[next]++;
					task_count--;
				}
				else {
					break;
				}
			}
			team.at(person)[crnt] = untch[crnt] + upd[crnt];
		}
		team.at(person)[TaskStatus::DONE] = upd[TaskStatus::DONE];

		//Почистим списки от пустых задач:
		WipeZero(team.at(person));
		WipeZero(upd);
		WipeZero(untch);

		if (untch.count(TaskStatus::DONE))
			untch.erase(TaskStatus::DONE);

		return std::tie(upd, untch);
	}
private:
	void WipeZero(TasksInfo& tasks) {
		std::vector<TaskStatus> empty_task;
		for (const auto& [status, count] : tasks) {
			if (count == 0)empty_task.push_back(status);
		}

		if(empty_task.size()){
			for (const TaskStatus& status : empty_task)
				tasks.erase(status);
		}
	}
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
	std::cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
		", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
		", " << tasks_info[TaskStatus::TESTING] << " tasks are being testeds" <<
		", "<< tasks_info[TaskStatus::DONE] << " tasks are done" << std::endl;
}

void ShowTaskInfo(const TasksInfo& ti, const std::string& name) {
	std::cout << name << std::endl;
	for (const auto& [status, count] : ti) {
		std::cout << " [ " << static_cast<int>(status) << " ] " << count << std::endl;
	}
}

int main() {
	TeamTasks tasks;
	tasks.AddNewTask("Ilia");
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan");
	}
	//std::cout << "Ilia's tasks: ";
	//PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
	std::cout << "Ivan's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	std::cout << std::endl << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);
	ShowTaskInfo(updated_tasks, "updated");
	ShowTaskInfo(untouched_tasks, "untouched");
	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	std::cout << std::endl << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);
	ShowTaskInfo(updated_tasks, "updated");
	ShowTaskInfo(untouched_tasks, "untouched");
	return 0;
}