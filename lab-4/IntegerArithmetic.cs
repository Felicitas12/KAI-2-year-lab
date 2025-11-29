using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab_4
{
    public class IntegerArithmetic
    {
        public delegate void OverflowHandler(object sender, OverflowEventArgs e);

        public event OverflowHandler GridOverflow;

        public int Add(int a, int b)
        {
            long res = (long)a + b;

            if (res > int.MaxValue || res < int.MinValue)
            {
                GridOverflow?.Invoke(this, new OverflowEventArgs("Додавання", res));
                return 0;
            }

            return (int)res;
        }

        public int Multiply(int a, int b)
        {
            long res = (long)a * b;

            if (res > int.MaxValue || res < int.MinValue)
            {
                GridOverflow?.Invoke(this, new OverflowEventArgs("Множення", res));
                return 0;
            }

            return (int)res;
        }
    }
}
