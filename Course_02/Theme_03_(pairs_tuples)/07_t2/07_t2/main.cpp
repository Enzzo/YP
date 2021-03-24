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
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
		return person_task_status.at(person);
	}
	
	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const std::string& person) {
		person_task_status[person][TaskStatus::NEW]++;
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {
		
		
		TasksInfo untouched;
		TasksInfo updated;

		if (person_task_status.count(person) == 0)
			return std::tie(updated, untouched);
		
		TasksInfo& current = person_task_status.at(person);
		untouched = current;

		while (--task_count >= 0) {
			if (current.at(TaskStatus::NEW) > 0) {
				current[TaskStatus::NEW]--;
				current[TaskStatus::IN_PROGRESS]++;

				untouched[TaskStatus::NEW]--;
				updated[TaskStatus::IN_PROGRESS]++;
				continue;
			}
			if (current.at(TaskStatus::IN_PROGRESS) > 0) {
				current[TaskStatus::IN_PROGRESS]--;
				current[TaskStatus::TESTING]++;

				if (untouched[TaskStatus::IN_PROGRESS] > 0)
					untouched[TaskStatus::IN_PROGRESS]--;
				updated[TaskStatus::TESTING]++;
				continue;
			}
			if (current.at(TaskStatus::TESTING) > 0) {
				current[TaskStatus::TESTING]--;
				current[TaskStatus::DONE]++;

				if (untouched[TaskStatus::TESTING] > 0)
					untouched[TaskStatus::TESTING]--;
				updated[TaskStatus::DONE]++;
				continue;
			}
		}
		
		return tie(updated, untouched);
	}

private:
	std::map<std::string, std::map<TaskStatus, int>> person_task_status;
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

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	
	std::cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);


	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	
	std::cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);

	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);
	
	return 0;
}