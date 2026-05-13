// ============================================================
// Services/FopService.cs — Сервіс для роботи з ФОП
// Реалізує бізнес-логіку: додавання транзакцій,
// отримання контрагентів, розрахунок балансу та звітів.
// ============================================================

using FopFinance.Data;
using FopFinance.Models;

namespace FopFinance.Services;

/// <summary>
/// Сервісний шар між UI та моделями.
/// Відповідає за валідацію, координацію та збереження даних.
/// </summary>
public class FopService
{
    private readonly InMemoryDatabase _db;

    public FopService(InMemoryDatabase db) => _db = db;

    // --- Транзакції ---

    /// <summary>
    /// Додає нову транзакцію до бази та до ФОП.
    /// </summary>
    public void AddTransaction(Tranzakcia t)
    {
        t.Id = _db.NextTranzakciaId();
        _db.CurrentFop.AddTransaction(t);   // валідація всередині Fop
        _db.Tranzakcii.Add(t);
    }

    /// <summary>
    /// Повертає всі транзакції, відфільтровані за типом.
    /// Якщо typ == null — повертає всі.
    /// </summary>
    public List<Tranzakcia> GetTransactions(string? typ = null) =>
        typ == null
            ? _db.Tranzakcii
            : _db.Tranzakcii.Where(t => t.Typ == typ).ToList();

    // --- Баланс та податки ---

    /// <summary>Повертає поточний баланс ФОП (доходи − витрати).</summary>
    public double GetBalance() => _db.CurrentFop.GetBalance();

    /// <summary>
    /// Розраховує суму єдиного податку за всіма доходами.
    /// </summary>
    public double CalcTotalTax()
    {
        double stavka = _db.CurrentFop.StavkaPodatku;
        return _db.Tranzakcii
            .Where(t => t.Validate())
            .Sum(t => t.CalcTax(stavka));
    }

    // --- Контрагенти ---

    /// <summary>Повертає список всіх контрагентів ФОП.</summary>
    public List<Kontrahent> GetContragents() => _db.Kontrahenty;

    /// <summary>Додає нового контрагента до бази.</summary>
    public void AddContragent(Kontrahent k)
    {
        k.Id = _db.NextKontrahentId();
        _db.Kontrahenty.Add(k);
        _db.CurrentFop.Kontrahenty.Add(k);
    }

    // --- Рахунки ---

    /// <summary>Повертає всі рахунки з бази.</summary>
    public List<Rahunok> GetRahunky() => _db.Rahunky;

    /// <summary>
    /// Додає рахунок та пов'язує його з контрагентом.
    /// </summary>
    public void AddRahunok(Rahunok r, Kontrahent? k = null)
    {
        r.Id = _db.NextRahunokId();
        _db.Rahunky.Add(r);
        k?.Rahunky.Add(r);
    }

    /// <summary>
    /// Оплачує рахунок: змінює його статус та автоматично
    /// породжує транзакцію-дохід (з State Machine Diagram).
    /// </summary>
    public void PayRahunok(Rahunok r)
    {
        var tranzakcia = r.Pay();   // статус Рахунку → "Оплачений"
        AddTransaction(tranzakcia); // автоматична реєстрація транзакції
    }

    // --- Звіти ---

    /// <summary>
    /// Генерує та зберігає звіт за вказаний період.
    /// </summary>
    public Zvit GenReport(string period)
    {
        var zvit = _db.CurrentFop.GenReport(period);
        zvit.Id = _db.NextZvitId();
        _db.Zvity.Add(zvit);
        return zvit;
    }

    /// <summary>Повертає всі збережені звіти.</summary>
    public List<Zvit> GetZvity() => _db.Zvity;
}
