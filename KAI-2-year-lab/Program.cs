using System;
using System.Collections;
using System.Collections.Generic;

namespace KAI_2_year_lab
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            Vector v1 = new Vector(1, 2, 2);
            Vector v2 = new Vector(0, 3, 4);
            Vector v3 = new Vector(4, 4, 2);

            Console.WriteLine("--- Collections Demo ---");

            Vector[] array = new Vector[3];
            array[0] = v1; array[1] = v2; array[2] = v3;
            Console.WriteLine($"Array[1]: {array[1]}");

            ArrayList arrayList = new ArrayList();
            arrayList.Add(v1);
            arrayList.Add(v2);
            arrayList.Add(v3);
            arrayList.Remove(v2);
            Console.WriteLine($"ArrayList Count: {arrayList.Count}");

            List<Vector> genericList = new List<Vector>();
            genericList.Add(v1);
            genericList.Add(v2);
            genericList.Add(v3);
            genericList.Sort();

            Console.WriteLine("Sorted List<Vector>:");
            foreach (var item in genericList)
            {
                Console.WriteLine(item);
            }

            Console.WriteLine("\n--- Binary Tree Demo (Preorder) ---");

            BinaryTree<Vector> tree = new BinaryTree<Vector>();
            tree.Add(new Vector(3, 3, 3));
            tree.Add(new Vector(1, 1, 1));
            tree.Add(new Vector(5, 5, 5));
            tree.Add(new Vector(2, 2, 2));

            foreach (var vec in tree)
            {
                Console.WriteLine(vec);
            }

            Console.ReadLine();
        }
    }
}