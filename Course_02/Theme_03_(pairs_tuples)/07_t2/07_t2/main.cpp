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
	std::map<std::string, TasksInfo> person_task_status;
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
		return person_task_status.at(person);
	}
	
	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const std::string& person) {
		if(person_task_status.count(person) == 0){
			//map<int, map<int, int>> i = {1, {1, 1}}
			person_task_status.insert({ person, {{TaskStatus::NEW, 1}, {TaskStatus::IN_PROGRESS, 0}, {TaskStatus::TESTING, 0},{TaskStatus::DONE, 0} } });
		}
		else
			person_task_status.at(person).at(TaskStatus::NEW)++;
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {
				
		TasksInfo untouched;
		TasksInfo updated;

		if (person_task_status.count(person) == 0)
			return std::tuple(updated, untouched);
		
		TasksInfo& current = person_task_status.at(person);
		untouched = current;

		for (int status = static_cast<int>(TaskStatus::NEW); status < (static_cast<int>(TaskStatus::DONE)-1); ++status) {
			TaskStatus now = static_cast<TaskStatus>(status);
			TaskStatus next = static_cast<TaskStatus>(status + 1);

			if (current.at(now)) {
				current.at(now)--;
				untouched.at(now)--;

				if (current.at(next)) {
					current.at(next)++;
				}
				else {
					current.insert({ next,1 });
				}
				task_count--;
			}
		}
		/*
		while (task_count--) {
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
		*/
		return std::tuple(updated, untouched);
	}
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь

void PrintTasksInfo(TasksInfo tasks_info) {

	TasksInfo t = tasks_info;

	std::cout << t[TaskStatus::NEW] << " new tasks" <<
		", " << t[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
		", " << t[TaskStatus::TESTING] << " tasks are being tested" <<
		", " << t[TaskStatus::DONE] << " tasks are done" << std::endl;
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