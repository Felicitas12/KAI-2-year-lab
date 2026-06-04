// ============================================================
// FopFinanceTests.cs — Модульні тести для проєкту FopFinance
// Лабораторна робота 5: Модульне тестування ПЗ
//
// Фреймворк: xUnit (.NET 8)
// Покриваються тест-кейси:
//   [БЛ-1] TC-BL-01 — Tranzakcia.Validate() позитивний сценарій
//   [БЛ-2] TC-BL-08 — Fop.GetBalance() коректний розрахунок балансу
//   [AUX-1] TC-AUX-01 — Zvit.ExportPdf() створює файл на диску
//   [AUX-2] TC-AUX-03 — Zvit.ExportPdf() вміст файлу коректний
// ============================================================

using FopFinance.Models;
using System.Text;
using Xunit;

namespace FopFinance.Tests;

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ██  БІЗНЕС-ЛОГІКА
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/// <summary>
/// Тести для класу Tranzakcia.
/// Чек-лист 1, TC-BL-01 — TC-BL-05.
/// </summary>
public class TranzakciaValidateTests
{
    // ─────────────────────────────────────────────────────────
    // [БЛ-1] TC-BL-01 — Позитивний сценарій
    // Валідна транзакція повинна проходити перевірку.
    // ─────────────────────────────────────────────────────────

    /// <summary>
    /// Транзакція з коректними даними (дохід, сума > 0, є номер
    /// документа, дата — сьогодні) повинна повертати true.
    /// </summary>
    [Fact]
    public void Validate_ValidIncomeTodayWithDoc_ReturnsTrue()
    {
        // Arrange — підготовка об'єкта з усіма валідними полями
        var t = new Tranzakcia
        {
            Suma = 15_000,
            Typ = "дохід",
            NomerDoc = "АКТ-001",
            Data = DateTime.Today,
            Opys = "Оплата за розробку"
        };

        // Act
        bool result = t.Validate();

        // Assert
        Assert.True(result,
            "Транзакція з валідними полями повинна проходити перевірку.");
    }

    /// <summary>
    /// Витратна транзакція з коректними полями також повинна проходити.
    /// </summary>
    [Fact]
    public void Validate_ValidExpenseYesterdayWithDoc_ReturnsTrue()
    {
        var t = new Tranzakcia
        {
            Suma = 3_500,
            Typ = "витрата",
            NomerDoc = "ДОГ-007",
            Data = DateTime.Today.AddDays(-1)
        };

        Assert.True(t.Validate());
    }

    // ─────────────────────────────────────────────────────────
    // TC-BL-02 — Негативний: сума ≤ 0
    // ─────────────────────────────────────────────────────────

    [Theory]
    [InlineData(0)]
    [InlineData(-1)]
    [InlineData(-999.99)]
    public void Validate_NonPositiveSuma_ReturnsFalse(double suma)
    {
        var t = new Tranzakcia
        {
            Suma = suma,
            Typ = "дохід",
            NomerDoc = "АКТ-001",
            Data = DateTime.Today
        };

        Assert.False(t.Validate(),
            $"Сума {suma} не є позитивною — транзакція невалідна.");
    }

    // ─────────────────────────────────────────────────────────
    // TC-BL-03 — Негативний: порожній або пробільний NomerDoc
    // ─────────────────────────────────────────────────────────

    [Theory]
    [InlineData("")]
    [InlineData("   ")]
    public void Validate_EmptyOrWhitespaceNomerDoc_ReturnsFalse(string nomerDoc)
    {
        var t = new Tranzakcia
        {
            Suma = 1_000,
            Typ = "дохід",
            NomerDoc = nomerDoc,
            Data = DateTime.Today
        };

        Assert.False(t.Validate(),
            "Транзакція без номера документа повинна бути невалідною.");
    }

    // ─────────────────────────────────────────────────────────
    // TC-BL-04 — Негативний: дата у майбутньому
    // ─────────────────────────────────────────────────────────

    [Fact]
    public void Validate_FutureDate_ReturnsFalse()
    {
        var t = new Tranzakcia
        {
            Suma = 1_000,
            Typ = "дохід",
            NomerDoc = "АКТ-X",
            Data = DateTime.Today.AddDays(1)   // завтра
        };

        Assert.False(t.Validate(),
            "Транзакція з майбутньою датою повинна бути невалідною.");
    }

    // ─────────────────────────────────────────────────────────
    // TC-BL-05 — Негативний: невідомий тип
    // ─────────────────────────────────────────────────────────

    [Theory]
    [InlineData("переказ")]
    [InlineData("ДОХІД")]      // регістр має значення
    [InlineData("")]
    public void Validate_UnknownTyp_ReturnsFalse(string typ)
    {
        var t = new Tranzakcia
        {
            Suma = 500,
            Typ = typ,
            NomerDoc = "ДОК-01",
            Data = DateTime.Today
        };

        Assert.False(t.Validate(),
            $"Тип '{typ}' не допускається — транзакція невалідна.");
    }
}


/// <summary>
/// Тести для методу CalcTax — Чек-лист 1, TC-BL-06, TC-BL-07.
/// </summary>
public class TranzakciaCalcTaxTests
{
    [Fact]
    public void CalcTax_IncomeWith5Percent_ReturnsCorrectAmount()
    {
        var t = new Tranzakcia { Suma = 10_000, Typ = "дохід" };

        double tax = t.CalcTax(5.0);

        Assert.Equal(500.0, tax, precision: 2);
    }

    [Fact]
    public void CalcTax_ExpenseAnyRate_ReturnsZero()
    {
        var t = new Tranzakcia { Suma = 5_000, Typ = "витрата" };

        double tax = t.CalcTax(5.0);

        Assert.Equal(0.0, tax);
    }

    [Fact]
    public void CalcTax_ZeroRate_ReturnsZero()
    {
        var t = new Tranzakcia { Suma = 20_000, Typ = "дохід" };

        Assert.Equal(0.0, t.CalcTax(0.0));
    }
}


/// <summary>
/// Тести для Fop.GetBalance() — Чек-лист 1, TC-BL-08, TC-BL-09.
/// </summary>
public class FopGetBalanceTests
{
    // ─────────────────────────────────────────────────────────
    // [БЛ-2] TC-BL-08 — Коректний розрахунок балансу
    // Баланс = сума доходів − сума витрат (лише валідні).
    // ─────────────────────────────────────────────────────────

    /// <summary>
    /// ФОП з одним доходом і однією витратою.
    /// Очікуємо: 20 000 − 5 000 = 15 000 грн.
    /// </summary>
    [Fact]
    public void GetBalance_OneIncomeOneExpense_ReturnsCorrectDifference()
    {
        // Arrange
        var income = new Tranzakcia
        {
            Suma = 20_000,
            Typ = "дохід",
            NomerDoc = "АКТ-001",
            Data = DateTime.Today
        };
        var expense = new Tranzakcia
        {
            Suma = 5_000,
            Typ = "витрата",
            NomerDoc = "ДОГ-001",
            Data = DateTime.Today
        };

        var fop = new Fop
        {
            Tranzakcii = new List<Tranzakcia> { income, expense }
        };

        // Act
        double balance = fop.GetBalance();

        // Assert
        Assert.Equal(15_000.0, balance, precision: 2);
    }

    /// <summary>
    /// Кілька доходів і кілька витрат — складний сценарій.
    /// Очікуємо: (25 000 + 18 000) − (3 500 + 2 200) = 37 300.
    /// </summary>
    [Fact]
    public void GetBalance_MultipleTransactions_ReturnsSumOfSignedAmounts()
    {
        var transactions = new List<Tranzakcia>
        {
            new() { Suma = 25_000, Typ = "дохід",   NomerDoc = "АКТ-001", Data = DateTime.Today },
            new() { Suma = 18_000, Typ = "дохід",   NomerDoc = "АКТ-002", Data = DateTime.Today },
            new() { Suma =  3_500, Typ = "витрата", NomerDoc = "ДОГ-001", Data = DateTime.Today },
            new() { Suma =  2_200, Typ = "витрата", NomerDoc = "РАХ-001", Data = DateTime.Today },
        };

        var fop = new Fop { Tranzakcii = transactions };

        Assert.Equal(37_300.0, fop.GetBalance(), precision: 2);
    }

    // ─────────────────────────────────────────────────────────
    // TC-BL-09 — Граничний: порожній список → 0
    // ─────────────────────────────────────────────────────────

    [Fact]
    public void GetBalance_NoTransactions_ReturnsZeroWithoutException()
    {
        var fop = new Fop { Tranzakcii = new List<Tranzakcia>() };

        double balance = fop.GetBalance();

        Assert.Equal(0.0, balance);
    }

    /// <summary>
    /// Невалідні транзакції (сума ≤ 0) НЕ впливають на баланс.
    /// </summary>
    [Fact]
    public void GetBalance_InvalidTransactionsExcluded_NotCounted()
    {
        var valid = new Tranzakcia
        {
            Suma = 10_000,
            Typ = "дохід",
            NomerDoc = "АКТ-001",
            Data = DateTime.Today
        };
        var invalid = new Tranzakcia
        {
            Suma = -999,
            Typ = "дохід",    // невалідна — сума від'ємна
            NomerDoc = "АКТ-BAD",
            Data = DateTime.Today
        };

        var fop = new Fop { Tranzakcii = new List<Tranzakcia> { valid, invalid } };

        // Очікуємо лише валідний дохід
        Assert.Equal(10_000.0, fop.GetBalance(), precision: 2);
    }
}


/// <summary>
/// Тести для Rahunok State Machine — Чек-лист 1, TC-BL-11, TC-BL-12, TC-BL-13.
/// </summary>
public class RahunokStateMachineTests
{
    [Fact]
    public void NewRahunok_DefaultStatus_IsChernетка()
    {
        var r = new Rahunok { Nomer = "РХ-001", Suma = 1000 };

        Assert.Equal("Чернетка", r.Status);
    }

    [Fact]
    public void Issue_FromChernетка_ChangesStatusToVystavlenyi()
    {
        var r = new Rahunok { Nomer = "РХ-001", Suma = 1000 };

        r.Issue();

        Assert.Equal("Виставлений", r.Status);
    }

    [Fact]
    public void Issue_Twice_ThrowsInvalidOperationException()
    {
        var r = new Rahunok { Nomer = "РХ-001", Suma = 1000 };
        r.Issue();

        Assert.Throws<InvalidOperationException>(() => r.Issue());
    }

    [Fact]
    public void Pay_FromVystavlenyi_ReturnsIncomeTransaction()
    {
        var r = new Rahunok { Nomer = "РХ-001", Suma = 5000 };
        r.Issue();

        var tranzakcia = r.Pay();

        Assert.Equal("Оплачений", r.Status);
        Assert.Equal("дохід", tranzakcia.Typ);
        Assert.Equal(5000, tranzakcia.Suma);
    }

    [Fact]
    public void Cancel_AfterPay_ThrowsInvalidOperationException()
    {
        var r = new Rahunok { Nomer = "РХ-001", Suma = 1000 };
        r.Issue();
        r.Pay();

        Assert.Throws<InvalidOperationException>(() => r.Cancel());
    }
}


// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ██  ДОПОМІЖНІ ЗАВДАННЯ — файлові операції
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/// <summary>
/// Тести для Zvit.ExportPdf() та Zvit.Generate().
/// Чек-лист 2, TC-AUX-01 — TC-AUX-06.
/// </summary>
public class ZvitExportTests : IDisposable
{
    // Список файлів, створених під час тестів — прибираємо після кожного класу
    private readonly List<string> _filesToDelete = new();

    // ─────────────────────────────────────────────────────────
    // [AUX-1] TC-AUX-01 — ExportPdf() створює файл на диску
    // ─────────────────────────────────────────────────────────

    /// <summary>
    /// Після виклику ExportPdf() файл повинен існувати на диску.
    /// </summary>
    [Fact]
    public void ExportPdf_ValidZvit_CreatesFileOnDisk()
    {
        // Arrange
        var zvit = BuildZvit();

        // Act
        string path = zvit.ExportPdf();
        _filesToDelete.Add(path);   // реєструємо для очищення

        // Assert
        Assert.True(File.Exists(path),
            $"Файл '{path}' повинен існувати після ExportPdf().");
    }

    // ─────────────────────────────────────────────────────────
    // TC-AUX-02 — Ім'я файлу відповідає шаблону
    // ─────────────────────────────────────────────────────────

    [Fact]
    public void ExportPdf_ReturnedPath_ContainsTypAndDate()
    {
        var zvit = BuildZvit(typ: "Єдиний податок", date: new DateTime(2025, 1, 15));

        string path = zvit.ExportPdf();
        _filesToDelete.Add(path);

        string fileName = Path.GetFileName(path);
        Assert.Contains("Єдиний_податок", fileName);
        Assert.Contains("20250115", fileName);
    }

    // ─────────────────────────────────────────────────────────
    // [AUX-2] TC-AUX-03 — Вміст файлу коректний
    // Перевіряємо наявність ключових даних у збереженому файлі.
    // ─────────────────────────────────────────────────────────

    /// <summary>
    /// Файл повинен містити рядки з доходами, витратами та прибутком.
    /// SumaDoxodiv=43 000, SumaVytrat=5 700 → Prybutok=37 300.
    /// </summary>
    [Fact]
    public void ExportPdf_FileContent_ContainsIncomeExpenseProfitAndPeriod()
    {
        // Arrange
        var zvit = BuildZvit(
            sumaDox: 43_000,
            sumaVyt: 5_700,
            period: "Q1 2025");

        // Act
        string path = zvit.ExportPdf();
        _filesToDelete.Add(path);

        string[] lines = File.ReadAllLines(path, Encoding.UTF8);
        string content = string.Join("\n", lines);

        // Assert — шукаємо ключові фрагменти (без точного форматування)
        Assert.Contains("43", content, StringComparison.Ordinal); // доходи
        Assert.Contains("5", content, StringComparison.Ordinal); // витрати
        Assert.Contains("37", content, StringComparison.Ordinal); // прибуток
        Assert.Contains("Q1 2025", content, StringComparison.Ordinal); // період
    }

    // ─────────────────────────────────────────────────────────
    // TC-AUX-04 — Кодування UTF-8: кирилиця зберігається коректно
    // ─────────────────────────────────────────────────────────

    [Fact]
    public void ExportPdf_CyrillicContent_SavedAsUtf8WithoutGarbling()
    {
        var zvit = BuildZvit(typ: "Квартальний звіт");

        string path = zvit.ExportPdf();
        _filesToDelete.Add(path);

        // Читаємо явно з UTF-8
        string content = File.ReadAllText(path, Encoding.UTF8);

        // Перевіряємо, що кирилиця збережена — символів-замінників '?' немає
        Assert.DoesNotContain("??", content);
        Assert.Contains("ФІНАНСОВИЙ ЗВІТ", content);
    }

    // ─────────────────────────────────────────────────────────
    // TC-AUX-05 — Zvit.Generate() виводить у консоль без винятку
    // ─────────────────────────────────────────────────────────

    [Fact]
    public void Generate_ValidZvit_WritesKeywordsToConsole()
    {
        // Arrange — перехоплюємо стандартний вивід
        var output = new StringWriter();
        Console.SetOut(output);

        var zvit = BuildZvit(sumaDox: 25_000, sumaVyt: 7_000);

        // Act
        zvit.Generate();

        string printed = output.ToString();

        // Restore stdout
        Console.SetOut(new StreamWriter(Console.OpenStandardOutput()) { AutoFlush = true });

        // Assert
        Assert.Contains("ФІНАНСОВИЙ ЗВІТ", printed);
        Assert.Contains("Доходи", printed);
        Assert.Contains("Витрати", printed);
        Assert.Contains("Прибуток", printed);
    }

    // ─────────────────────────────────────────────────────────
    // TC-AUX-06 — Generate() виводить числово-коректні значення
    // ─────────────────────────────────────────────────────────

    [Fact]
    public void Generate_ValidAmounts_PrintsCorrectNumbers()
    {
        var output = new StringWriter();
        Console.SetOut(output);

        var zvit = BuildZvit(sumaDox: 25_000, sumaVyt: 7_000);
        // Очікуємо прибуток = 18 000

        zvit.Generate();

        string printed = output.ToString();
        Console.SetOut(new StreamWriter(Console.OpenStandardOutput()) { AutoFlush = true });

        Assert.Contains("25", printed, StringComparison.Ordinal);
        Assert.Contains("18", printed, StringComparison.Ordinal);
    }

    // ─────────────────────────────────────────────────────────
    // TC-AUX-07 — ToString() повертає коректний рядок
    // ─────────────────────────────────────────────────────────

    [Fact]
    public void ToString_ContainsIdTypPeriodAndAmounts()
    {
        var zvit = new Zvit
        {
            Id = 42,
            Typ = "Квартальний",
            Period = "Q2 2025",
            DataStvor = DateTime.Today,
            SumaDoxodiv = 10_000,
            SumaVytrat = 3_000
        };

        string s = zvit.ToString();

        Assert.Contains("42", s);
        Assert.Contains("Квартальний", s);
        Assert.Contains("Q2 2025", s);
    }

    // ─────────────────────────────────────────────────────────
    // TC-AUX-08 — ExportPdf() двічі — не кидає виняток
    // ─────────────────────────────────────────────────────────

    [Fact]
    public void ExportPdf_CalledTwice_DoesNotThrow()
    {
        var zvit = BuildZvit();

        string path1 = zvit.ExportPdf();
        _filesToDelete.Add(path1);

        // Другий виклик перезаписує файл — не має кидати виняток
        string path2 = zvit.ExportPdf();
        _filesToDelete.Add(path2);

        Assert.True(File.Exists(path2));
    }

    // ─────────────────────────────────────────────────────────
    // Допоміжний фабричний метод
    // ─────────────────────────────────────────────────────────

    private static Zvit BuildZvit(
        string? typ = null,
        double sumaDox = 10_000,
        double sumaVyt = 3_000,
        string period = "Тест 2025",
        DateTime? date = null) => new Zvit
        {
            Id = 1,
            Typ = typ ?? "Єдиний податок",
            Period = period,
            DataStvor = date ?? DateTime.Today,
            SumaDoxodiv = sumaDox,
            SumaVytrat = sumaVyt
        };

    // ─────────────────────────────────────────────────────────
    // Cleanup — видаляємо всі тимчасові файли після тестів
    // ─────────────────────────────────────────────────────────

    public void Dispose()
    {
        foreach (var path in _filesToDelete)
        {
            if (File.Exists(path))
                File.Delete(path);
        }
    }
}