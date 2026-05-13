// ============================================================
// Models/Zvit.cs — Клас «Звіт»
// Фінансовий звіт за певний період діяльності ФОП.
// ============================================================

namespace FopFinance.Models;

/// <summary>
/// Звіт формується з об'єкта Fop та містить підсумкові показники
/// за вибраний звітний період.
/// </summary>
public class Zvit
{
    public int Id { get; set; }

    /// <summary>Тип звіту (напр. "Єдиний податок", "Квартальний").</summary>
    public string Typ { get; set; } = string.Empty;

    /// <summary>Дата створення звіту.</summary>
    public DateTime DataStvor { get; set; }

    /// <summary>Звітний період у текстовому форматі (напр. "Q1 2025").</summary>
    public string Period { get; set; } = string.Empty;

    /// <summary>Загальна сума доходів за період.</summary>
    public double SumaDoxodiv { get; set; }

    /// <summary>Загальна сума витрат за період.</summary>
    public double SumaVytrat { get; set; }

    /// <summary>Прибуток = доходи − витрати.</summary>
    public double Prybutok => SumaDoxodiv - SumaVytrat;

    // --- Методи ---

    /// <summary>
    /// Генерує текстовий вміст звіту та виводить у консоль.
    /// </summary>
    public void Generate()
    {
        Console.WriteLine(new string('═', 52));
        Console.WriteLine($"  ФІНАНСОВИЙ ЗВІТ ФОП");
        Console.WriteLine($"  Тип:    {Typ}");
        Console.WriteLine($"  Період: {Period}");
        Console.WriteLine($"  Дата:   {DataStvor:dd.MM.yyyy}");
        Console.WriteLine(new string('─', 52));
        Console.WriteLine($"  Доходи:    {SumaDoxodiv,14:N2} грн");
        Console.WriteLine($"  Витрати:   {SumaVytrat,14:N2} грн");
        Console.WriteLine($"  Прибуток:  {Prybutok,14:N2} грн");
        Console.WriteLine(new string('═', 52));
    }

    /// <summary>
    /// Експортує звіт у текстовий файл (імітація PDF-експорту).
    /// У продакшн-реалізації тут використовувався б iTextSharp/QuestPDF.
    /// </summary>
    /// <returns>Шлях до створеного файлу.</returns>
    public string ExportPdf()
    {
        // Формуємо ім'я файлу на основі типу та дати
        string fileName = $"Zvit_{Typ.Replace(" ", "_")}_{DataStvor:yyyyMMdd}.txt";
        string path = Path.Combine(Directory.GetCurrentDirectory(), fileName);

        // Записуємо вміст звіту у файл
        var lines = new[]
        {
            $"ФІНАНСОВИЙ ЗВІТ ФОП",
            $"Тип:    {Typ}",
            $"Період: {Period}",
            $"Дата:   {DataStvor:dd.MM.yyyy}",
            new string('-', 40),
            $"Доходи:   {SumaDoxodiv:N2} грн",
            $"Витрати:  {SumaVytrat:N2} грн",
            $"Прибуток: {Prybutok:N2} грн",
        };

        File.WriteAllLines(path, lines, System.Text.Encoding.UTF8);
        return path;
    }

    public override string ToString() =>
        $"Звіт #{Id} | {Typ} | {Period} | Дохід: {SumaDoxodiv:N2} | Витрати: {SumaVytrat:N2}";
}
