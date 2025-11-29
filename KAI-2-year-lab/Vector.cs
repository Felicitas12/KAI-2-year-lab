using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KAI_2_year_lab
{
    public class Vector : IComparable<Vector>
    {
        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }

        public Vector(double x, double y, double z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public double CalculateLength()
        {
            return Math.Sqrt(X * X + Y * Y + Z * Z);
        }

        public int CompareTo(Vector other)
        {
            if (other == null) return 1;
            return this.CalculateLength().CompareTo(other.CalculateLength());
        }

        public override string ToString()
        {
            return $"Vector({X}, {Y}, {Z}) Length: {CalculateLength():F2}";
        }
    }
}
