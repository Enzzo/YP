#include <map>
#include <string>
#include <algorithm>
#include <tuple>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
// ѕеречислимый тип дл€ статуса задачи
enum class TaskStatus {
    NEW,          // нова€
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};
// ќбъ€вл€ем тип-синоним дл€ map<TaskStatus, int>,
// позвол€ющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

void del_zero(TasksInfo& tasks_with_zero) {
    vector<TaskStatus> task_to_del;
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
public:
    // ѕолучить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return tasks_to_person.at(person);
    };
    // ƒобавить новую задачу (в статусе NEW) дл€ конкретного разработчитка
    void AddNewTask(const string& person) {
        ++tasks_to_person[person][TaskStatus::NEW];
    }
    // ќбновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        if (tasks_to_person.count(person) == 0) {
            TasksInfo empty;
            return { empty, empty };
        }
        int sum_tasks = tasks_to_person[person][TaskStatus::NEW] +
            tasks_to_person[person][TaskStatus::IN_PROGRESS] +
            tasks_to_person[person][TaskStatus::TESTING];
        if (task_count > sum_tasks) {
            task_count = sum_tasks;
        }

        TasksInfo untouched_task = tasks_to_person.at(person);
        TasksInfo updated_task;
        for (size_t i = 0; i < static_cast<size_t>(task_count); ++i) {
            if (untouched_task[TaskStatus::NEW] > 0) { // !=0 >=0 ??
                --untouched_task[TaskStatus::NEW];
                ++updated_task[TaskStatus::IN_PROGRESS];
            }
            else if (untouched_task[TaskStatus::IN_PROGRESS] > 0) {
                --untouched_task[TaskStatus::IN_PROGRESS];
                ++updated_task[TaskStatus::TESTING];
            }
            else if (untouched_task[TaskStatus::TESTING] > 0) {
                --untouched_task[TaskStatus::TESTING];
                ++updated_task[TaskStatus::DONE];
            }
        }
        //сложить оставшиес€ и обновленные?
       // tasks_to_person.at(person) = untouched_task;
        //map<string, TasksInfo> tasks_to_person;
       // using TasksInfo = map<TaskStatus, int>;
        tasks_to_person[person][TaskStatus::NEW]
            = untouched_task[TaskStatus::NEW] + updated_task[TaskStatus::NEW];
        tasks_to_person[person][TaskStatus::IN_PROGRESS]
            = untouched_task[TaskStatus::IN_PROGRESS] + updated_task[TaskStatus::IN_PROGRESS];
        tasks_to_person[person][TaskStatus::TESTING]
            = untouched_task[TaskStatus::TESTING] + updated_task[TaskStatus::TESTING];
        tasks_to_person[person][TaskStatus::DONE]
            = untouched_task[TaskStatus::DONE] + updated_task[TaskStatus::DONE];
        untouched_task.erase(TaskStatus::DONE);
        //удалить нули
        del_zero(tasks_to_person.at(person));
        del_zero(untouched_task);
        //using TasksInfo = map<TaskStatus, int>;
        return { updated_task, untouched_task };
    }
private:
    map<string, TasksInfo> tasks_to_person;
};