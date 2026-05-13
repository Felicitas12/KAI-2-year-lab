// ============================================================
// Models/Fop.cs — Клас «ФОП» (Фізична особа-підприємець)
// Зберігає реєстраційні дані та посилання на транзакції/звіти.
// ============================================================

namespace FopFinance.Models;

/// <summary>
/// Головна сутність системи — Фізична особа-підприємець.
/// Реєструє транзакції та формує звіти.
/// </summary>
public class Fop
{
    // --- Поля (відповідають діаграмі класів) ---

    /// <summary>Унікальний ідентифікатор запису.</summary>
    public int Id { get; set; }

    /// <summary>Прізвище, ім'я, по батькові підприємця.</summary>
    public string Pib { get; set; } = string.Empty;

    /// <summary>Ідентифікаційний податковий номер (10 цифр).</summary>
    public string Ipn { get; set; } = string.Empty;

    /// <summary>Група єдиного податку (1–4).</summary>
    public int GrupaYeP { get; set; }

    /// <summary>Ставка єдиного податку у відсотках.</summary>
    public double StavkaPodatku { get; set; }

    // --- Колекції пов'язаних об'єктів ---

    /// <summary>Список усіх транзакцій ФОП.</summary>
    public List<Tranzakcia> Tranzakcii { get; set; } = new();

    /// <summary>Список контрагентів з власної бази ФОП.</summary>
    public List<Kontrahent> Kontrahenty { get; set; } = new();

    // --- Методи (відповідають діаграмі класів) ---

    /// <summary>
    /// Повертає поточний баланс: сума доходів мінус сума витрат.
    /// </summary>
    public double GetBalance()
    {
        double dohody  = Tranzakcii
            .Where(t => t.Typ == "дохід"  && t.Validate())
            .Sum(t => t.Suma);

        double vytraty = Tranzakcii
            .Where(t => t.Typ == "витрата" && t.Validate())
            .Sum(t => t.Suma);

        return dohody - vytraty;
    }

    /// <summary>
    /// Реєструє нову транзакцію: перевіряє її та додає до списку.
    /// </summary>
    /// <param name="t">Транзакція для реєстрації.</param>
    /// <exception cref="InvalidOperationException">
    /// Кидається, якщо транзакція не пройшла валідацію.
    /// </exception>
    public void AddTransaction(Tranzakcia t)
    {
        if (!t.Validate())
            throw new InvalidOperationException(
                $"Транзакція {t.NomerDoc} не пройшла перевірку.");

        Tranzakcii.Add(t);
    }

    /// <summary>
    /// Формує звіт за вказаний рядковий період (напр. "Q1 2025").
    /// </summary>
    /// <param name="period">Рядок-позначення періоду.</param>
    public Zvit GenReport(string period)
    {
        // Рахуємо загальні доходи та витрати за всіма валідними транзакціями
        var valid = Tranzakcii.Where(t => t.Validate()).ToList();

        double sumaDoh = valid
            .Where(t => t.Typ == "дохід")
            .Sum(t => t.Suma);

        double sumaVyt = valid
            .Where(t => t.Typ == "витрата")
            .Sum(t => t.Suma);

        return new Zvit
        {
            Id          = new Random().Next(1000, 9999),
            Typ         = "Єдиний податок",
            DataStvor   = DateTime.Today,
            Period      = period,
            SumaDoxodiv = sumaDoh,
            SumaVytrat  = sumaVyt
        };
    }

    public override string ToString() =>
        $"[ФОП #{Id}] {Pib} | ІПН: {Ipn} | Група: {GrupaYeP} | Ставка: {StavkaPodatku}%";
}
