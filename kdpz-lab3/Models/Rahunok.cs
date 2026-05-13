// ============================================================
// Models/Rahunok.cs — Клас «Рахунок»
// Рахунок-фактура, що виставляється контрагентом.
// Моделює стани відповідно до State Machine Diagram (Лаб.2).
// ============================================================

namespace FopFinance.Models;

/// <summary>
/// Рахунок-фактура. Проходить стани:
/// Чернетка → Виставлений → Оплачений / Скасований.
/// </summary>
public class Rahunok
{
    public int Id { get; set; }

    /// <summary>Унікальний номер рахунку (наприклад "РХ-2025-001").</summary>
    public string Nomer { get; set; } = string.Empty;

    /// <summary>Дата виставлення рахунку.</summary>
    public DateTime Data { get; set; }

    /// <summary>Сума рахунку у гривнях.</summary>
    public double Suma { get; set; }

    /// <summary>
    /// Поточний статус рахунку:
    /// "Чернетка", "Виставлений", "Оплачений", "Скасований".
    /// </summary>
    public string Status { get; private set; } = "Чернетка";

    // --- Методи (State Machine: переходи між станами) ---

    /// <summary>
    /// Виставити рахунок: Чернетка → Виставлений.
    /// </summary>
    public void Issue()
    {
        if (Status != "Чернетка")
            throw new InvalidOperationException(
                $"Рахунок '{Nomer}' вже виставлений або завершений.");

        Status = "Виставлений";
        Console.WriteLine($"✓ Рахунок {Nomer} виставлено.");
    }

    /// <summary>
    /// Оплатити рахунок: Виставлений → Оплачений.
    /// При оплаті автоматично породжується Транзакція з typ="дохід".
    /// </summary>
    /// <returns>Транзакція-дохід, що відповідає оплаті.</returns>
    public Tranzakcia Pay()
    {
        if (Status != "Виставлений")
            throw new InvalidOperationException(
                $"Рахунок '{Nomer}' не виставлений — оплата неможлива.");

        Status = "Оплачений";
        Console.WriteLine($"✓ Рахунок {Nomer} оплачено.");

        // Згідно з State Machine Diagram: оплата породжує транзакцію-дохід
        return new Tranzakcia
        {
            Id       = new Random().Next(1000, 9999),
            Data     = DateTime.Today,
            Suma     = Suma,
            Typ      = "дохід",
            Opys     = $"Оплата рахунку {Nomer}",
            NomerDoc = Nomer
        };
    }

    /// <summary>
    /// Скасувати рахунок: будь-який стан → Скасований.
    /// </summary>
    public void Cancel()
    {
        if (Status == "Оплачений")
            throw new InvalidOperationException(
                $"Рахунок '{Nomer}' вже оплачений — скасувати не можна.");

        Status = "Скасований";
        Console.WriteLine($"✗ Рахунок {Nomer} скасовано.");
    }

    public override string ToString() =>
        $"[{Nomer}] {Data:dd.MM.yyyy} | {Suma:N2} грн | Статус: {Status}";
}
