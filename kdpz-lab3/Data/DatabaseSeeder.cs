// ============================================================
// Data/DatabaseSeeder.cs — Заповнення бази тестовими даними
// Виконується один раз при старті програми.
// ============================================================

using FopFinance.Models;

namespace FopFinance.Data;

/// <summary>
/// Статичний клас для ініціалізації сховища демо-даними.
/// </summary>
public static class DatabaseSeeder
{
    public static void Seed(InMemoryDatabase db)
    {
        // --- Категорії (статичний довідник, не потребують Id від DB) ---
        var kategorii = Kategoria.GetList();

        // --- Контрагенти ---
        var k1 = new Kontrahent
        {
            Id     = db.NextKontrahentId(),
            Nazva  = "ТОВ «Технологія плюс»",
            Edrpou = "12345678",
            Typ    = "юр.особа"
        };
        var k2 = new Kontrahent
        {
            Id     = db.NextKontrahentId(),
            Nazva  = "ФОП Петренко Іван Сергійович",
            Edrpou = "3456789012",
            Typ    = "фіз.особа"
        };
        db.Kontrahenty.AddRange(new[] { k1, k2 });

        // --- Рахунки ---
        var r1 = new Rahunok
        {
            Id    = db.NextRahunokId(),
            Nomer = "РХ-2025-001",
            Data  = new DateTime(2025, 1, 10),
            Suma  = 15000
        };
        r1.Issue(); // Чернетка → Виставлений
        k1.Rahunky.Add(r1);
        db.Rahunky.Add(r1);

        // --- Транзакції ---
        var t1 = new Tranzakcia
        {
            Id         = db.NextTranzakciaId(),
            Data       = new DateTime(2025, 1, 15),
            Suma       = 25000,
            Typ        = "дохід",
            Opys       = "Розробка вебсайту",
            NomerDoc   = "АКТ-001",
            Kategoria  = kategorii[0], // Послуги IT
            Kontrahent = k1
        };
        var t2 = new Tranzakcia
        {
            Id         = db.NextTranzakciaId(),
            Data       = new DateTime(2025, 1, 20),
            Suma       = 3500,
            Typ        = "витрата",
            Opys       = "Оренда офісу (січень)",
            NomerDoc   = "ДОГ-002",
            Kategoria  = kategorii[2], // Оренда офісу
            Kontrahent = k2
        };
        var t3 = new Tranzakcia
        {
            Id         = db.NextTranzakciaId(),
            Data       = new DateTime(2025, 2, 5),
            Suma       = 18000,
            Typ        = "дохід",
            Opys       = "Консалтингові послуги",
            NomerDoc   = "АКТ-002",
            Kategoria  = kategorii[1], // Консалтинг
            Kontrahent = k2
        };
        var t4 = new Tranzakcia
        {
            Id         = db.NextTranzakciaId(),
            Data       = new DateTime(2025, 2, 12),
            Suma       = 2200,
            Typ        = "витрата",
            Opys       = "Реклама в соцмережах",
            NomerDoc   = "РАХ-003",
            Kategoria  = kategorii[3], // Реклама
        };

        // Додаємо транзакції до глобального списку та до контрагентів
        db.Tranzakcii.AddRange(new[] { t1, t2, t3, t4 });
        k1.Tranzakcii.Add(t1);
        k2.Tranzakcii.AddRange(new[] { t2, t3 });

        // --- Реєструємо ФОП та прив'язуємо дані ---
        var fop = new Fop
        {
            Id             = 1,
            Pib            = "Журбенко Ілля Геннадійович",
            Ipn            = "1234567890",
            GrupaYeP       = 3,
            StavkaPodatku  = 5.0,
            Tranzakcii     = db.Tranzakcii,
            Kontrahenty    = db.Kontrahenty
        };
        db.Fopy.Add(fop);
    }
}
