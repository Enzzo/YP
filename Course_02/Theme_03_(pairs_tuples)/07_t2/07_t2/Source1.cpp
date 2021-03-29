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
    std::map<std::string, TasksInfo> developers;
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
        return developers.at(person);
    }
    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const std::string& person) {
        ++developers[person][TaskStatus::NEW];
    }
    TaskStatus UpdateStatus(const TaskStatus& status) const {
        TaskStatus upd = TaskStatus::NEW;
        switch (status) {
        case TaskStatus::NEW:
            upd = TaskStatus::IN_PROGRESS;
            break;
        case TaskStatus::IN_PROGRESS:
            upd = TaskStatus::TESTING;
            break;
        case TaskStatus::TESTING:
            upd = TaskStatus::DONE;
            break;
        case TaskStatus::DONE:
            upd = TaskStatus::NEW;
            break;
        }
        return upd;
    }
    void CheckForZeroes(TasksInfo& tasks) {
        for (const auto& [status, num] : tasks) {
            if (num == 0) {
                tasks.erase(status);
            }
        }
    }
    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {
        TasksInfo updated_tasks;
        TasksInfo untouched_tasks;
        if (developers.count(person) == 0) {
            return make_tuple(updated_tasks, untouched_tasks);
        }
        CheckForZeroes(developers.at(person));
        int not_done_tasks = 0;
        for (const auto& [status, num] : developers.at(person)) {
            not_done_tasks = status != TaskStatus::DONE ? not_done_tasks + num : not_done_tasks;
        }
        task_count = task_count > not_done_tasks ? not_done_tasks : task_count;
        int done_tasks = 0;
        for (const auto& [status, num] : developers.at(person)) {
            if (task_count == 0) {
                break;
            }
            if (status != TaskStatus::DONE) {
                int update = task_count > num ? num : task_count;
                updated_tasks[UpdateStatus(status)] = update;
                task_count -= update;
                if (num - update > 0) {
                    untouched_tasks[status] = num - update;
                }
            }
            else {
                if (num > 0) {
                    done_tasks = num;
                }
            }
        }
        developers.erase(person);
        for (const auto& [status, num] : untouched_tasks) {
            developers[person][status] = num;
        }
        if (done_tasks > 0) {
            developers[person][TaskStatus::DONE] = done_tasks;
        }
        for (const auto& [status, num] : updated_tasks) {
            developers[person][status] += num;
        }
        return make_tuple(updated_tasks, untouched_tasks);
    }
};