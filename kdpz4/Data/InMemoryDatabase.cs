// ============================================================
// Data/InMemoryDatabase.cs — In-Memory сховище даних
// Зберігає всі сутності у списках (імітує БД без зовнішніх залежностей).
// У реальному проєкті замінюється на EF Core + SQL Server/SQLite.
// ============================================================

using FopFinance.Models;

namespace FopFinance.Data;

/// <summary>
/// Центральне сховище даних застосунку.
/// Містить списки всіх сутностей та лічильники для auto-increment Id.
/// </summary>
public class InMemoryDatabase
{
    // --- Колекції сутностей ---

    /// <summary>Список зареєстрованих ФОП (у даному прикладі — один).</summary>
    public List<Fop> Fopy { get; } = new();

    /// <summary>Глобальний список усіх транзакцій.</summary>
    public List<Tranzakcia> Tranzakcii { get; } = new();

    /// <summary>Глобальний список контрагентів.</summary>
    public List<Kontrahent> Kontrahenty { get; } = new();

    /// <summary>Глобальний список рахунків.</summary>
    public List<Rahunok> Rahunky { get; } = new();

    /// <summary>Збережені звіти.</summary>
    public List<Zvit> Zvity { get; } = new();

    // --- Лічильники Id (auto-increment) ---

    private int _nextTranzakciaId  = 1;
    private int _nextKontrahentId  = 1;
    private int _nextRahunokId     = 1;
    private int _nextZvitId        = 1;

    // --- Методи отримання наступного Id ---

    public int NextTranzakciaId()  => _nextTranzakciaId++;
    public int NextKontrahentId()  => _nextKontrahentId++;
    public int NextRahunokId()     => _nextRahunokId++;
    public int NextZvitId()        => _nextZvitId++;

    // --- Зручний доступ до поточного ФОП ---

    /// <summary>
    /// Повертає першого (єдиного) ФОП у системі.
    /// Кидає виняток, якщо ФОП не зареєстрований.
    /// </summary>
    public Fop CurrentFop =>
        Fopy.FirstOrDefault()
        ?? throw new InvalidOperationException("ФОП не зареєстрований у системі.");
}
