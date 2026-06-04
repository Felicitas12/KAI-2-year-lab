// ============================================================
// Models/Kategoria.cs — Клас «Категорія»
// Довідник категорій для класифікації транзакцій.
// ============================================================

namespace FopFinance.Models;

/// <summary>
/// Категорія транзакції (наприклад "Послуги IT", "Оренда", "Реклама").
/// </summary>
public class Kategoria
{
    public int Id { get; set; }

    /// <summary>Назва категорії.</summary>
    public string Nazva { get; set; } = string.Empty;

    /// <summary>Тип категорії: "доходи" або "витрати".</summary>
    public string TypKategorii { get; set; } = string.Empty;

    /// <summary>
    /// Повертає список усіх категорій (статичний довідник).
    /// У реальній системі читалось би з БД.
    /// </summary>
    public static List<Kategoria> GetList() => new()
    {
        new Kategoria { Id = 1, Nazva = "Послуги IT",       TypKategorii = "доходи"  },
        new Kategoria { Id = 2, Nazva = "Консалтинг",       TypKategorii = "доходи"  },
        new Kategoria { Id = 3, Nazva = "Оренда офісу",     TypKategorii = "витрати" },
        new Kategoria { Id = 4, Nazva = "Реклама",          TypKategorii = "витрати" },
        new Kategoria { Id = 5, Nazva = "Програмне забезп.", TypKategorii = "витрати" },
        new Kategoria { Id = 6, Nazva = "Інші доходи",      TypKategorii = "доходи"  },
        new Kategoria { Id = 7, Nazva = "Інші витрати",     TypKategorii = "витрати" },
    };

    public override string ToString() => $"[{Id}] {Nazva} ({TypKategorii})";
}
