#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <Windows.h>

#include "GameInstances.h"
#include "GameManager.h"
#include "NetworkService.h"
#include "PlatformInstances.h"
#include "UserAccount.h"
#include "StreamingService.h"


class ConsoleEventLogger : public IGameEventObserver {
public:
    void onGameEvent(const GameEventData& d) override {
        std::cout << "  [ПОДІЯ] ";
        switch (d.type) {
        case GameEvent::Installed:     std::cout << "Гру '" << d.gameName << "' встановлено.\n"; break;
        case GameEvent::Uninstalled:   std::cout << "Гру '" << d.gameName << "' видалено.\n"; break;
        case GameEvent::Started:       std::cout << "Гру '" << d.gameName << "' запущено.\n"; break;
        case GameEvent::Stopped:       std::cout << "Гру '" << d.gameName << "' зупинено.\n"; break;
        case GameEvent::Saved:         std::cout << "Збереження '" << d.details << "' у грі '" << d.gameName << "'.\n"; break;
        case GameEvent::Loaded:        std::cout << "Завантажено '" << d.details << "' у грі '" << d.gameName << "'.\n"; break;
        case GameEvent::RatingChanged: std::cout << "Рейтинг '" << d.gameName << "': " << d.details << "\n"; break;
        }
    }
};

void printOp(const std::string& op, const std::string& err) {
    if (err.empty()) std::cout << "  [OK]   " << op << "\n";
    else std::cout << "  [FAIL] " << op << "\n         -> " << err << "\n";
}

void printGameCard(const Game& g) {
    const auto& r = g.getRequirements();
    std::cout << "\n  - " << g.getName() << " [" << genreName(g.getGenre()) << "]\n"
        << "    Встановлена : " << (g.isInstalled() ? "Так" : "Ні") << "\n"
        << "    Запущена    : " << (g.isRunning() ? "Так" : "Ні") << "\n"
        << "    Рейтинг     : " << g.getRating() << "  (" << g.getRatingCount() << " оцінок)\n"
        << "    Розмір HDD  : " << r.hddSizeGB << " ГБ\n";
    if (g.hasSaves()) {
        std::cout << "    Збереження  : ";
        for (const auto& s : g.getSaves()) std::cout << "[" << s << "] ";
        std::cout << "\n";
    }
    std::cout << "  ------------------------------------\n";
}

void printPlatformInfo(const Platform& p) {
    const auto& hw = p.getHardware();
    std::cout << "  Платформа: " << p.getName()
        << " | CPU " << hw.cpuGHz << " ГГц"
        << " | RAM " << hw.ramGB << " ГБ"
        << " | GPU " << hw.gpuGB << " ГБ"
        << " | HDD вільно " << hw.hddFreeGB << " ГБ\n";
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    ConsoleEventLogger logger;
    NetworkService     network;
    GameManager        mgr;
    UserAccount        player("Gamer42");

    // Автоматично логінимо гравця та вмикаємо мережу для зручності демо
    player.login("secure_pass");
    network.connect();

    // Створюємо десктоп-платформу
    DesktopPlatform desktop(HardwareSpec(3.5, 16, 8, 100));

    // Створюємо базові ігри
    auto simGame = std::make_unique<SimulatorGame>("FlightSim", GameRequirements(3.0, 16, 6, 25));
    auto fpsGame = std::make_unique<ShooterGame>("Combat Zone", GameRequirements(2.5, 8, 4, 15));
    auto webGame = std::make_unique<BrowserGame>("Pixel Puzzle", GameRequirements(1.0, 1, 0, 0));
    auto strGame = std::make_unique<StrategyGame>("StarCraft", GameRequirements(2.0, 4, 2, 10)); // Новий жанр

    simGame->subscribe(&logger);
    fpsGame->subscribe(&logger);
    webGame->subscribe(&logger);
    strGame->subscribe(&logger);

    mgr.addGame(std::move(simGame));
    mgr.addGame(std::move(fpsGame));
    mgr.addGame(std::move(webGame));
    mgr.addGame(std::move(strGame));

    std::cout << "========================================\n";
    std::cout << "      СИСТЕМА СИМУЛЯЦІЇ ІГОР\n";
    std::cout << "========================================\n";
    std::cout << "Ласкаво просимо, " << player.getUsername() << "!\n\n";

    std::cout << "--- Поточна система ---\n";
    printPlatformInfo(desktop);
    std::cout << "-----------------------\n";

    bool running = true;
    while (running) {
        std::cout << "\n======================================\n";
        std::cout << "             ГОЛОВНЕ МЕНЮ\n";
        std::cout << "======================================\n";
        std::cout << "  1. Показати бібліотеку ігор\n";
        std::cout << "  2. Встановити гру\n";
        std::cout << "  3. Запустити гру\n";
        std::cout << "  4. Зупинити активну гру\n";
        std::cout << "  5. Зберегти прогрес\n";
        std::cout << "  6. Завантажити збереження\n";
        std::cout << "  7. Видалити гру\n";
        std::cout << "  0. Вихід з програми\n";
        std::cout << "--------------------------------------\n";
        std::cout << "Ваш вибір: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "  [Помилка] Введіть числовале значення!\n";
            continue;
        }
        std::cin.ignore(10000, '\n');
        std::string inputStr;
        std::string slotStr;

        switch (choice) {
        case 1:
            std::cout << "\n  === Стан бібліотеки ===\n";
            for (const auto& g : mgr.getLibrary()) {
                printGameCard(*g);
            }
            break;
        case 2:
            std::cout << "Введіть точну назву гри для встановлення: ";
            std::getline(std::cin, inputStr);
            printOp("Встановлення '" + inputStr + "'", mgr.installGame(inputStr, desktop));
            break;
        case 3:
            std::cout << "Введіть точну назву гри для запуску: ";
            std::getline(std::cin, inputStr);
            printOp("Запуск '" + inputStr + "'", mgr.startGame(inputStr, desktop, player));
            break;
        case 4:
            printOp("Зупинка поточної гри", mgr.stopGame());
            break;
        case 5:
            std::cout << "Введіть назву гри: ";
            std::getline(std::cin, inputStr);
            std::cout << "Введіть назву слота: ";
            std::getline(std::cin, slotStr);
            printOp("Збереження", mgr.saveGame(inputStr, slotStr));
            break;
        case 6:
            std::cout << "Введіть назву гри: ";
            std::getline(std::cin, inputStr);
            std::cout << "Введіть назву слота: ";
            std::getline(std::cin, slotStr);
            printOp("Завантаження", mgr.loadGame(inputStr, slotStr));
            break;
        case 7:
            std::cout << "Введіть точну назву гри для видалення: ";
            std::getline(std::cin, inputStr);
            printOp("Видалення '" + inputStr + "'", mgr.uninstallGame(inputStr, desktop));
            break;
        case 0:
            std::cout << "\nЗавершення роботи. До побачення!\n";
            if (mgr.getActiveGame()) {
                mgr.stopGame();
            }
            running = false;
            break;
        default:
            std::cout << "  [Помилка] Невідома команда. Спробуйте ще раз.\n";
            break;
        }
    }

    return 0;
}