#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iomanip>

using namespace std;

const int TEAM_COUNT = 16;

struct TeamResult {
    char name[50];
    int wins;
    int draws;
    int losses;
    int points;
};

void inputTeams(TeamResult teams[]) {
    cout << "Введіть дані про 16 команд. Для кожної команди сума перемог, нічиїх і поразок повинна дорівнювати 15.\n\n";

    for (int i = 0; i < TEAM_COUNT; ++i) {
        cout << "Команда " << i + 1 << ":\n";
        cout << "Назва: ";
        cin >> ws;
        cin.getline(teams[i].name, 50);

        while (true) {
            cout << "Кількість перемог: ";
            cin >> teams[i].wins;
            cout << "Кількість нічиїх: ";
            cin >> teams[i].draws;
            cout << "Кількість поразок: ";
            cin >> teams[i].losses;

            if (teams[i].wins >= 0 && teams[i].draws >= 0 && teams[i].losses >= 0 &&
                teams[i].wins + teams[i].draws + teams[i].losses == 15) {
                break;
            }
            cout << "Помилка: сума результатів повинна дорівнювати 15. Повторіть введення.\n";
        }

        teams[i].points = teams[i].wins * 2 + teams[i].draws;
        cout << "Нараховано очок: " << teams[i].points << "\n\n";
    }
}

bool saveToFile(const char* fileName, const TeamResult teams[]) {
    FILE* file = fopen(fileName, "wb");
    if (file == nullptr) {
        return false;
    }

    size_t written = fwrite(teams, sizeof(TeamResult), TEAM_COUNT, file);
    fclose(file);
    return written == TEAM_COUNT;
}

bool loadFromFile(const char* fileName, TeamResult teams[]) {
    FILE* file = fopen(fileName, "rb");
    if (file == nullptr) {
        return false;
    }

    size_t readCount = fread(teams, sizeof(TeamResult), TEAM_COUNT, file);
    fclose(file);
    return readCount == TEAM_COUNT;
}

bool betterTeam(const TeamResult& a, const TeamResult& b) {
    if (a.points != b.points) return a.points > b.points;
    if (a.wins != b.wins) return a.wins > b.wins;
    return strcmp(a.name, b.name) < 0;
}

void printTable(const TeamResult teams[], int count) {
    cout << left
         << setw(5)  << "№"
         << setw(24) << "Команда"
         << setw(10) << "Перемоги"
         << setw(10) << "Нічиї"
         << setw(10) << "Поразки"
         << setw(8)  << "Очки" << '\n';

    cout << string(67, '-') << '\n';

    for (int i = 0; i < count; ++i) {
        cout << left
             << setw(5)  << i + 1
             << setw(24) << teams[i].name
             << setw(10) << teams[i].wins
             << setw(10) << teams[i].draws
             << setw(10) << teams[i].losses
             << setw(8)  << teams[i].points << '\n';
    }
}

int main() {
    const char* fileName = "championship.dat";
    TeamResult teams[TEAM_COUNT];
    TeamResult fromFile[TEAM_COUNT];

    inputTeams(teams);

    if (!saveToFile(fileName, teams)) {
        cerr << "Не вдалося записати дані у файл.\n";
        return 1;
    }

    if (!loadFromFile(fileName, fromFile)) {
        cerr << "Не вдалося прочитати дані з файла.\n";
        return 1;
    }

    sort(fromFile, fromFile + TEAM_COUNT, betterTeam);

    cout << "\nПідсумкова турнірна таблиця:\n";
    printTable(fromFile, TEAM_COUNT);

    cout << "\nПризові місця:\n";
    printTable(fromFile, 3);

    cout << "\nКількість перемог команд-призерів:\n";
    for (int i = 0; i < 3; ++i) {
        cout << i + 1 << " місце: " << fromFile[i].name
             << " — " << fromFile[i].wins << " перемог(и).\n";
    }

    return 0;
}
