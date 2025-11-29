using System;

namespace lab_4
{
    class Program
    {
        delegate int MatrixSumDelegate(int[,] matrix);

        static void Main(string[] args)
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            int[,] myMatrix = {
                { 1, 2, 3 },
                { 4, 5, 6 },
                { 7, 8, 9 }
            };

            MatrixSumDelegate sumCalculator = (mat) =>
            {
                int sum = 0;
                foreach (int item in mat)
                {
                    sum += item;
                }
                return sum;
            };

            int totalSum = sumCalculator(myMatrix);
            Console.WriteLine($"Сума елементів масиву: {totalSum}\n");

            IntegerArithmetic calculator = new IntegerArithmetic();

            calculator.GridOverflow += OnGridOverflowDetected;

            Console.WriteLine("Спроба: 100 * 50");
            int res1 = calculator.Multiply(100, 50);
            Console.WriteLine($"Результат: {res1}");

            Console.WriteLine();

            int bigNumber = 2000000000;
            Console.WriteLine($"Спроба: {bigNumber} * 2");

            int res2 = calculator.Multiply(bigNumber, 2);

            Console.WriteLine($"Результат після обробки: {res2}");

            Console.ReadKey();
        }

        private static void OnGridOverflowDetected(object sender, OverflowEventArgs e)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine($"[УВАГА] Сталася подія: Переповнення розрядної сітки!");
            Console.WriteLine($" -> Операція: {e.OperationName}");
            Console.WriteLine($" -> Фактичний результат (long): {e.ActualResult}");
            Console.WriteLine($" -> Максимально допустимий int: {int.MaxValue}");
            Console.ResetColor();
        }
    }
}