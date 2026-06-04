// ============================================================
// Models/Tranzakcia.cs — Клас «Транзакція»
// Зберігає фінансову операцію (дохід або витрата).
// ============================================================

namespace FopFinance.Models;

/// <summary>
/// Фінансова операція ФОП — може бути доходом або витратою.
/// Класифікується за Категорією, пов'язана з Контрагентом.
/// </summary>
public class Tranzakcia
{
    /// <summary>Унікальний ідентифікатор транзакції.</summary>
    public int Id { get; set; }

    /// <summary>Дата здійснення операції.</summary>
    public DateTime Data { get; set; }

    /// <summary>Сума операції (завжди додатна).</summary>
    public double Suma { get; set; }

    /// <summary>Тип операції: "дохід" або "витрата".</summary>
    public string Typ { get; set; } = string.Empty;

    /// <summary>Текстовий опис/призначення платежу.</summary>
    public string Opys { get; set; } = string.Empty;

    /// <summary>Номер первинного документа (рахунок, акт тощо).</summary>
    public string NomerDoc { get; set; } = string.Empty;

    /// <summary>Категорія, до якої відноситься транзакція.</summary>
    public Kategoria? Kategoria { get; set; }

    /// <summary>Контрагент-учасник операції (може бути null).</summary>
    public Kontrahent? Kontrahent { get; set; }

    // --- Методи ---

    /// <summary>
    /// Повертає суму з урахуванням знаку:
    /// дохід — додатній, витрата — від'ємний.
    /// </summary>
    public double GetSuma() => Typ == "дохід" ? Suma : -Suma;

    /// <summary>
    /// Розраховує суму податку для цієї транзакції.
    /// Метод застосовується лише до доходів;
    /// ставка передається ззовні (з об'єкта Fop).
    /// </summary>
    /// <param name="stavka">Ставка єдиного податку у відсотках.</param>
    public double CalcTax(double stavka)
    {
        // Податок нараховується тільки на доходи
        if (Typ != "дохід") return 0;
        return Suma * stavka / 100.0;
    }

    /// <summary>
    /// Перевіряє коректність транзакції:
    /// — сума більша за нуль;
    /// — тип є "дохід" або "витрата";
    /// — номер документа не порожній;
    /// — дата не у майбутньому.
    /// </summary>
    public bool Validate()
    {
        if (Suma <= 0)                               return false;
        if (Typ != "дохід" && Typ != "витрата")     return false;
        if (string.IsNullOrWhiteSpace(NomerDoc))    return false;
        if (Data > DateTime.Today)                  return false;
        return true;
    }

    public override string ToString() =>
        $"[#{Id}] {Data:dd.MM.yyyy} | {Typ.ToUpper(),-8} | {Suma,10:N2} грн | {Opys}";
}
