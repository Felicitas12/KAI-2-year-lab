// ============================================================
// Program.cs — Точка входу програми
// Тут запускається консольний інтерфейс та ініціалізується
// сховище даних із тестовими записами.
// ============================================================

using FopFinance.Data;
using FopFinance.Models;
using FopFinance.Services;
using FopFinance.UI;

// Ініціалізуємо сховище даних (in-memory репозиторій)
var db = new InMemoryDatabase();

// Заповнюємо базу тестовими даними для демонстрації
DatabaseSeeder.Seed(db);

// Запускаємо головне меню консольного інтерфейсу
var menu = new MainMenu(db);
menu.Run();
