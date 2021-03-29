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
        TasksInfo tmp;
        for (const auto& [status, num] : tasks) {
            if (num > 0) {
                tmp[status] = num;
            }
        }
        tasks = tmp;
    }
    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(const std::string& person, int task_count) {

        if (developers.count(person) == 0) {
            TasksInfo empty;
            return { empty, empty };
        }

        int total_tasks = developers[person][TaskStatus::NEW] + developers[person][TaskStatus::IN_PROGRESS] + developers[person][TaskStatus::TESTING];

        if (task_count > total_tasks) task_count = total_tasks;

        TasksInfo upd, untch = developers.at(person);

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

        developers[person][TaskStatus::NEW] = untch[TaskStatus::NEW] + upd[TaskStatus::NEW];
        developers[person][TaskStatus::IN_PROGRESS] = untch[TaskStatus::IN_PROGRESS] + upd[TaskStatus::IN_PROGRESS];
        developers[person][TaskStatus::TESTING] = untch[TaskStatus::TESTING] + upd[TaskStatus::TESTING];
        developers[person][TaskStatus::DONE] = untch[TaskStatus::DONE] + upd[TaskStatus::DONE];
        untch.erase(TaskStatus::DONE);
        CheckForZeroes(developers.at(person));
        CheckForZeroes(upd);
        return tie(upd, untch);
    }
};