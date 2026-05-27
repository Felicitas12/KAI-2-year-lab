// ============================================================
// Models/Kontrahent.cs — Клас «Контрагент»
// Юридична або фізична особа, з якою взаємодіє ФОП.
// ============================================================

namespace FopFinance.Models;

/// <summary>
/// Контрагент — партнер ФОП по фінансовим операціям.
/// Веде базу виставлених рахунків та здійснених транзакцій.
/// </summary>
public class Kontrahent
{
    public int Id { get; set; }

    /// <summary>Повна назва контрагента.</summary>
    public string Nazva { get; set; } = string.Empty;

    /// <summary>
    /// Код ЄДРПОУ (для юросіб) або ІПН (для фізосіб).
    /// </summary>
    public string Edrpou { get; set; } = string.Empty;

    /// <summary>Тип: "юр.особа" або "фіз.особа".</summary>
    public string Typ { get; set; } = string.Empty;

    // --- Пов'язані колекції ---

    /// <summary>Транзакції з цим контрагентом.</summary>
    public List<Tranzakcia> Tranzakcii { get; set; } = new();

    /// <summary>Рахунки, виставлені цим контрагентом.</summary>
    public List<Rahunok> Rahunky { get; set; } = new();

    // --- Методи ---

    /// <summary>Повертає всі транзакції з цим контрагентом.</summary>
    public List<Tranzakcia> GetTransactions() => Tranzakcii;

    /// <summary>Повертає всі рахунки від цього контрагента.</summary>
    public List<Rahunok> GetInvoices() => Rahunky;

    public override string ToString() =>
        $"[#{Id}] {Nazva} | {Typ} | ЄДРПОУ/ІПН: {Edrpou}";
}
