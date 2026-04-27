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


void printAction(const std::string& action, const std::string& err) {
    std::cout << "ДІЯ: " << action << "\n";
    if (err.empty()) std::cout << "РЕЗУЛЬТАТ: [УСПІХ]\n\n";
    else             std::cout << "РЕЗУЛЬТАТ: [БЛОКУВАННЯ] " << err << "\n\n";
}

void printInfo(const std::string& action, const std::string& info) {
    std::cout << "ДІЯ: " << action << "\n";
    std::cout << "РЕЗУЛЬТАТ: " << info << "\n\n";
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    GameManager mgr;
    NetworkService network;
    StreamingService streamSvc;
    UserAccount player("IlliaPC");

    // init -----------------------
    DesktopPlatform desktop(HardwareSpec(3.5, 16, 8, 100));
    MobilePlatform  mobile(HardwareSpec(1.5, 2, 1, 10));
    ConsolePlatform console(HardwareSpec(3.0, 8, 4, 50), 0);

    std::cout << "--- ПОЧАТКОВИЙ СТАН ---\n";
    std::cout << "Користувач: " << player.getUsername() << " (Не авторизований)\n";
    std::cout << "Мережа: Відключена\n";
    std::cout << "Платформи:\n";
    std::cout << " - Desktop (Потужний процесор, 100 ГБ вільно)\n";
    std::cout << " - Mobile  (Слабкий процесор, 10 ГБ вільно)\n";
    std::cout << " - Console (0 підключених маніпуляторів)\n";
    std::cout << "-----------------------\n\n";

    // base data  -----------------------
    mgr.addGame(std::make_unique<SimulatorGame>("FlightSim", GameRequirements(2.0, 8, 4, 20)));
    mgr.addGame(std::make_unique<ShooterGame>("Doom", GameRequirements(2.5, 8, 4, 30)));
    mgr.addGame(std::make_unique<OnlineAdventureGame>("WoW", GameRequirements(2.0, 4, 2, 15), network));
    mgr.addGame(std::make_unique<BrowserGame>("Agar.io", GameRequirements(1.0, 1, 0, 0)));

    printAction("Спроба додати другий Шутер (CS:GO), коли в бібліотеці вже є гра цього жанру",
        mgr.addGame(std::make_unique<ShooterGame>("CS:GO", GameRequirements(2.0, 4, 2, 10))));

    printAction("Спроба додати гру нового жанру (Strategy - StarCraft)",
        mgr.addGame(std::make_unique<StrategyGame>("StarCraft", GameRequirements(1.5, 4, 2, 10))));

    printAction("Спроба інсталювати Doom (потрібно 30 ГБ) на Mobile (вільно 10 ГБ)",
        mgr.installGame("Doom", mobile));

    printAction("Спроба запустити FlightSim на Desktop без попередньої інсталяції",
        mgr.startGame("FlightSim", desktop, player));

    mgr.installGame("FlightSim", desktop);

    printAction("Спроба запустити інстальований FlightSim без авторизації користувача",
        mgr.startGame("FlightSim", desktop, player));

    player.login("password");

    printAction("Спроба запустити інстальований FlightSim після авторизації користувача",
        mgr.startGame("FlightSim", desktop, player));
    mgr.stopGame();

	// simulation of various edge cases  -----------------------
    mgr.installGame("FlightSim", mobile);
    printAction("Спроба запустити FlightSim (мінімум CPU 2.0 ГГц) на Mobile (CPU 1.5 ГГц)",
        mgr.startGame("FlightSim", mobile, player));

    mgr.startGame("FlightSim", desktop, player);
    mgr.installGame("Doom", desktop);
    printAction("Спроба запустити Doom, коли вже активно запущена інша гра (FlightSim)",
        mgr.startGame("Doom", desktop, player));
    mgr.stopGame();

    printAction("Спроба зберегти стан гри FlightSim до її фактичного запуску",
        mgr.saveGame("FlightSim", "Slot1"));

    mgr.startGame("FlightSim", desktop, player);
    printAction("Спроба завантажити збереження, якщо жодного стану ще не було збережено",
        mgr.loadGame("FlightSim", "Slot1"));

    printAction("Збереження поточного стану запущеної гри у 'Slot1'",
        mgr.saveGame("FlightSim", "Slot1"));

    printAction("Завантаження існуючого стану гри зі 'Slot1'",
        mgr.loadGame("FlightSim", "Slot1"));
    mgr.stopGame();

    printAction("Спроба запустити браузерну гру Agar.io відразу, без встановлення",
        mgr.startGame("Agar.io", desktop, player));
    mgr.stopGame();

    mgr.installGame("WoW", desktop);
    printAction("Спроба запустити онлайн-гру WoW без підключення до інтернету",
        mgr.startGame("WoW", desktop, player));

    network.connect();
    printAction("Спроба запустити онлайн-гру WoW після підключення до інтернету",
        mgr.startGame("WoW", desktop, player));
    mgr.stopGame();

    mgr.installGame("Doom", console);
    printAction("Спроба запустити гру Doom на Console без підключених маніпуляторів",
        mgr.startGame("Doom", console, player));

    console.connectController();
    printAction("Спроба запустити гру Doom на Console після підключення маніпулятора",
        mgr.startGame("Doom", console, player));
    mgr.stopGame();

    mgr.rateGame("FlightSim", 5);
    printInfo("Отримання рейтингу гри FlightSim після оцінювання користувачем",
        "Оцінка " + std::to_string(mgr.getRating("FlightSim")).substr(0, 3) + " з 5");

    printAction("Спроба розпочати трансляцію з Desktop", streamSvc.startStream(desktop));
    printAction("Спроба розпочати трансляцію з Mobile", streamSvc.startStream(mobile));
    printAction("Спроба розпочати трансляцію з Console", streamSvc.startStream(console));

    return 0;
}