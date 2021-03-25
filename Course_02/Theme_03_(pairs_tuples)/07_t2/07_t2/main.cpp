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
	std::map<std::string, TasksInfo> task;
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const std::string& person){			
		return task.at(person);
	}
	
	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const std::string& person) {
		
		if(task.count(person) == 0){
			//map<int, map<int, int>> i = {1, {1, 1}}
			task.insert({ person, {{TaskStatus::NEW, 1}, {TaskStatus::IN_PROGRESS, 0}, {TaskStatus::TESTING, 0},{TaskStatus::DONE, 0} } });
		}
		else
			task.at(person).at(TaskStatus::NEW)++;
	
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {

		int total_tasks = 0;
		for (int i = static_cast<int>(TaskStatus::NEW); i < static_cast<int>(TaskStatus::DONE); i++) {
			total_tasks += task.at(person).at(static_cast<TaskStatus>(i));
		}
		if (task_count > total_tasks) task_count = total_tasks;

		TasksInfo untouched;
		TasksInfo updated;

		if (task.count(person)) {
			TasksInfo current = task.at(person);
			untouched = current;

			for (size_t status = static_cast<int>(TaskStatus::NEW); (status < (static_cast<int>(TaskStatus::DONE)) && task_count > 0); ++status) {
				TaskStatus now = static_cast<TaskStatus>(status);
				TaskStatus next = static_cast<TaskStatus>(status + 1);

				while (task_count > 0) {
					if (current.at(now)) {
						current.at(now)--;
						current.at(next)++;

						if (!updated.count(next))
							updated.insert({ next, 0 });
						updated.at(next)++;

						if (untouched.at(now) > 0) untouched.at(now)--;

						task_count--;
					}
					else
						break;
				}
			}
			task.at(person) = current;
		}
		return tie(updated, untouched);
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
	//tasks.AddNewTask("Ilia");
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan");
	}
	/*
	std::cout << "Ilia's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
	*/
	std::cout << "Ivan's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
	
	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan", 2);

	std::cout << "\nUpdated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "\nUpdated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "\nUpdated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "\nUpdated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "\nUpdated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);
}