// ============================================================
// UI/MainMenu.cs — Головне меню консольного інтерфейсу
// Всі взаємодії з користувачем зосереджені тут.
// ============================================================

using FopFinance.Data;
using FopFinance.Models;
using FopFinance.Services;

namespace FopFinance.UI;

/// <summary>
/// Консольне меню верхнього рівня.
/// Делегує операції до FopService, не містить бізнес-логіки.
/// </summary>
public class MainMenu
{
    private readonly FopService _svc;
    private readonly InMemoryDatabase _db;

    public MainMenu(InMemoryDatabase db)
    {
        _db  = db;
        _svc = new FopService(db);
    }

    /// <summary>Головний цикл меню — працює до вибору "Вихід".</summary>
    public void Run()
    {
        Console.OutputEncoding = System.Text.Encoding.UTF8;

        PrintHeader();

        while (true)
        {
            PrintMenu();
            var choice = Console.ReadLine()?.Trim();

            Console.WriteLine();

            switch (choice)
            {
                case "1": ShowFopInfo();         break;
                case "2": ShowTransactions();    break;
                case "3": AddTransaction();      break;
                case "4": ShowContragents();     break;
                case "5": AddContragent();       break;
                case "6": ShowRahunky();         break;
                case "7": AddRahunok();          break;
                case "8": PayRahunok();          break;
                case "9": GenerateReport();      break;
                case "0":
                    Console.WriteLine("До побачення!");
                    return;
                default:
                    Console.WriteLine("⚠  Невірний вибір. Спробуйте ще раз.");
                    break;
            }

            Console.WriteLine();
            Console.Write("Натисніть Enter для продовження...");
            Console.ReadLine();
        }
    }

    // ─── Вивід інформації ───────────────────────────────────

    private void PrintHeader()
    {
        Console.Clear();
        Console.WriteLine(new string('═', 56));
        Console.WriteLine("  💰  ОБЛІК ФІНАНСІВ ФОП  |  FopFinance v1.0");
        Console.WriteLine(new string('═', 56));
        Console.WriteLine();
    }

    private void PrintMenu()
    {
        Console.Clear();
        var fop = _db.CurrentFop;
        Console.WriteLine($"  ФОП: {fop.Pib}  |  Баланс: {_svc.GetBalance():N2} грн");
        Console.WriteLine(new string('─', 56));
        Console.WriteLine("  1. Інформація про ФОП");
        Console.WriteLine("  2. Список транзакцій");
        Console.WriteLine("  3. Додати транзакцію");
        Console.WriteLine("  4. Контрагенти");
        Console.WriteLine("  5. Додати контрагента");
        Console.WriteLine("  6. Рахунки");
        Console.WriteLine("  7. Додати рахунок");
        Console.WriteLine("  8. Оплатити рахунок");
        Console.WriteLine("  9. Сформувати звіт");
        Console.WriteLine("  0. Вихід");
        Console.WriteLine(new string('─', 56));
        Console.Write("  Ваш вибір: ");
    }

    // ─── Пункти меню ────────────────────────────────────────

    /// <summary>1. Детальна інформація про поточного ФОП.</summary>
    private void ShowFopInfo()
    {
        var fop = _db.CurrentFop;
        Console.WriteLine(new string('═', 56));
        Console.WriteLine("  ІНФОРМАЦІЯ ПРО ФОП");
        Console.WriteLine(new string('─', 56));
        Console.WriteLine($"  ПІБ:             {fop.Pib}");
        Console.WriteLine($"  ІПН:             {fop.Ipn}");
        Console.WriteLine($"  Група ЄП:        {fop.GrupaYeP}");
        Console.WriteLine($"  Ставка податку:  {fop.StavkaPodatku}%");
        Console.WriteLine(new string('─', 56));
        Console.WriteLine($"  Баланс:          {_svc.GetBalance():N2} грн");
        Console.WriteLine($"  Сума податку:    {_svc.CalcTotalTax():N2} грн");
        Console.WriteLine($"  Транзакцій:      {_db.Tranzakcii.Count}");
        Console.WriteLine($"  Контрагентів:    {_db.Kontrahenty.Count}");
        Console.WriteLine(new string('═', 56));
    }

    /// <summary>2. Перелік транзакцій з фільтром за типом.</summary>
    private void ShowTransactions()
    {
        Console.Write("  Фільтр (1=дохід, 2=витрата, 3=всі): ");
        var f = Console.ReadLine()?.Trim();
        string? filter = f switch { "1" => "дохід", "2" => "витрата", _ => null };

        var list = _svc.GetTransactions(filter);

        Console.WriteLine(new string('─', 70));
        Console.WriteLine($"  {"#",-4} {"Дата",-12} {"Тип",-10} {"Сума (грн)",-14} {"Документ",-12} Опис");
        Console.WriteLine(new string('─', 70));

        foreach (var t in list)
        {
            string katNazva = t.Kategoria?.Nazva ?? "—";
            Console.WriteLine(
                $"  {t.Id,-4} {t.Data:dd.MM.yyyy}  {t.Typ,-10} {t.Suma,12:N2}  {t.NomerDoc,-12} {t.Opys}");
        }

        Console.WriteLine(new string('─', 70));

        // Підсумковий рядок
        double total = list.Sum(t => t.GetSuma());
        Console.WriteLine($"  Підсумок: {total:N2} грн  ({list.Count} записів)");
    }

    /// <summary>3. Форма додавання нової транзакції.</summary>
    private void AddTransaction()
    {
        Console.WriteLine("  НОВА ТРАНЗАКЦІЯ");
        Console.WriteLine(new string('─', 40));

        // Тип операції
        Console.Write("  Тип (1=дохід, 2=витрата): ");
        var typChoice = Console.ReadLine()?.Trim();
        string typ = typChoice == "1" ? "дохід" : "витрата";

        // Сума
        Console.Write("  Сума (грн): ");
        if (!double.TryParse(Console.ReadLine(), out double suma) || suma <= 0)
        {
            Console.WriteLine("  ⚠ Невірна сума.");
            return;
        }

        // Опис
        Console.Write("  Опис: ");
        string opys = Console.ReadLine() ?? "";

        // Номер документа
        Console.Write("  Номер документа: ");
        string nomerDoc = Console.ReadLine() ?? "";

        // Вибір контрагента (необов'язково)
        var kontrahenty = _svc.GetContragents();
        Kontrahent? selected = null;
        if (kontrahenty.Any())
        {
            Console.WriteLine("  Контрагент (0 = без контрагента):");
            for (int i = 0; i < kontrahenty.Count; i++)
                Console.WriteLine($"    {i + 1}. {kontrahenty[i].Nazva}");
            Console.Write("  Вибір: ");
            if (int.TryParse(Console.ReadLine(), out int ki) && ki >= 1 && ki <= kontrahenty.Count)
                selected = kontrahenty[ki - 1];
        }

        // Вибір категорії
        var kategorii = Kategoria.GetList()
            .Where(k => k.TypKategorii == typ + "и" || k.TypKategorii.StartsWith(typ[..3]))
            .ToList();
        Kategoria? kat = null;
        if (kategorii.Any())
        {
            Console.WriteLine("  Категорія:");
            for (int i = 0; i < kategorii.Count; i++)
                Console.WriteLine($"    {i + 1}. {kategorii[i].Nazva}");
            Console.Write("  Вибір: ");
            if (int.TryParse(Console.ReadLine(), out int ki) && ki >= 1 && ki <= kategorii.Count)
                kat = kategorii[ki - 1];
        }

        // Збираємо об'єкт та зберігаємо
        var t = new Tranzakcia
        {
            Data       = DateTime.Today,
            Suma       = suma,
            Typ        = typ,
            Opys       = opys,
            NomerDoc   = nomerDoc,
            Kontrahent = selected,
            Kategoria  = kat
        };

        try
        {
            _svc.AddTransaction(t);
            Console.WriteLine($"\n  ✓ Транзакцію #{t.Id} успішно додано!");
        }
        catch (InvalidOperationException ex)
        {
            Console.WriteLine($"\n  ✗ Помилка: {ex.Message}");
        }
    }

    /// <summary>4. Список контрагентів з їх транзакціями.</summary>
    private void ShowContragents()
    {
        var list = _svc.GetContragents();

        if (!list.Any())
        {
            Console.WriteLine("  Контрагентів немає.");
            return;
        }

        Console.WriteLine(new string('─', 60));
        foreach (var k in list)
        {
            Console.WriteLine($"  {k}");
            double oborot = k.Tranzakcii.Sum(t => t.GetSuma());
            Console.WriteLine($"    Транзакцій: {k.Tranzakcii.Count}  |  Оборот: {oborot:N2} грн");
        }
        Console.WriteLine(new string('─', 60));
    }

    /// <summary>5. Форма додавання нового контрагента.</summary>
    private void AddContragent()
    {
        Console.WriteLine("  НОВИЙ КОНТРАГЕНТ");
        Console.WriteLine(new string('─', 40));

        Console.Write("  Назва: ");
        string nazva = Console.ReadLine() ?? "";

        Console.Write("  ЄДРПОУ / ІПН: ");
        string edrpou = Console.ReadLine() ?? "";

        Console.Write("  Тип (1=юр.особа, 2=фіз.особа): ");
        string typ = Console.ReadLine()?.Trim() == "1" ? "юр.особа" : "фіз.особа";

        var k = new Kontrahent { Nazva = nazva, Edrpou = edrpou, Typ = typ };
        _svc.AddContragent(k);
        Console.WriteLine($"\n  ✓ Контрагента #{k.Id} успішно додано!");
    }

    /// <summary>6. Список рахунків з поточним статусом.</summary>
    private void ShowRahunky()
    {
        var list = _svc.GetRahunky();

        if (!list.Any())
        {
            Console.WriteLine("  Рахунків немає.");
            return;
        }

        Console.WriteLine(new string('─', 60));
        Console.WriteLine($"  {"#",-4} {"Номер",-16} {"Дата",-12} {"Сума",-14} Статус");
        Console.WriteLine(new string('─', 60));
        foreach (var r in list)
            Console.WriteLine($"  {r.Id,-4} {r.Nomer,-16} {r.Data:dd.MM.yyyy}  {r.Suma,12:N2}  {r.Status}");
        Console.WriteLine(new string('─', 60));
    }

    /// <summary>7. Форма додавання нового рахунку.</summary>
    private void AddRahunok()
    {
        Console.WriteLine("  НОВИЙ РАХУНОК");
        Console.WriteLine(new string('─', 40));

        Console.Write("  Номер рахунку: ");
        string nomer = Console.ReadLine() ?? $"РХ-{DateTime.Now:yyyyMMddHHmm}";

        Console.Write("  Сума (грн): ");
        if (!double.TryParse(Console.ReadLine(), out double suma) || suma <= 0)
        {
            Console.WriteLine("  ⚠ Невірна сума.");
            return;
        }

        // Вибір контрагента
        var kontrahenty = _svc.GetContragents();
        Kontrahent? selected = null;
        if (kontrahenty.Any())
        {
            Console.WriteLine("  Контрагент (0 = без контрагента):");
            for (int i = 0; i < kontrahenty.Count; i++)
                Console.WriteLine($"    {i + 1}. {kontrahenty[i].Nazva}");
            Console.Write("  Вибір: ");
            if (int.TryParse(Console.ReadLine(), out int ki) && ki >= 1 && ki <= kontrahenty.Count)
                selected = kontrahenty[ki - 1];
        }

        var r = new Rahunok { Nomer = nomer, Data = DateTime.Today, Suma = suma };
        _svc.AddRahunok(r, selected);
        Console.WriteLine($"\n  ✓ Рахунок '{nomer}' створено зі статусом «{r.Status}».");
    }

    /// <summary>8. Оплата вибраного рахунку.</summary>
    private void PayRahunok()
    {
        var list = _svc.GetRahunky()
            .Where(r => r.Status == "Виставлений")
            .ToList();

        if (!list.Any())
        {
            Console.WriteLine("  Немає рахунків зі статусом «Виставлений».");
            return;
        }

        Console.WriteLine("  Виберіть рахунок для оплати:");
        for (int i = 0; i < list.Count; i++)
            Console.WriteLine($"    {i + 1}. {list[i]}");

        Console.Write("  Вибір: ");
        if (!int.TryParse(Console.ReadLine(), out int idx) || idx < 1 || idx > list.Count)
        {
            Console.WriteLine("  ⚠ Невірний вибір.");
            return;
        }

        try
        {
            _svc.PayRahunok(list[idx - 1]);
            Console.WriteLine("  ✓ Рахунок оплачено. Транзакцію-дохід автоматично зареєстровано.");
        }
        catch (InvalidOperationException ex)
        {
            Console.WriteLine($"  ✗ {ex.Message}");
        }
    }

    /// <summary>9. Генерація та збереження фінансового звіту.</summary>
    private void GenerateReport()
    {
        Console.Write("  Звітний період (напр. 'Q1 2025', 'Рік 2024'): ");
        string period = Console.ReadLine() ?? "Поточний";

        var zvit = _svc.GenReport(period);
        zvit.Generate();

        Console.Write("\n  Зберегти звіт у файл? (y/n): ");
        if (Console.ReadLine()?.Trim().ToLower() == "y")
        {
            string path = zvit.ExportPdf();
            Console.WriteLine($"  ✓ Збережено: {path}");
        }
    }
}
