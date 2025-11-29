using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KAI_2_year_lab
{
    public class BinaryTree<T> : IEnumerable<T> where T : class, IComparable<T>
    {
        private Node<T> root;

        public void Add(T data)
        {
            if (root == null)
            {
                root = new Node<T>(data);
                return;
            }
            AddRecursive(root, data);
        }

        private void AddRecursive(Node<T> current, T data)
        {
            if (data.CompareTo(current.Data) < 0)
            {
                if (current.Left == null) current.Left = new Node<T>(data);
                else AddRecursive(current.Left, data);
            }
            else
            {
                if (current.Right == null) current.Right = new Node<T>(data);
                else AddRecursive(current.Right, data);
            }
        }

        public IEnumerator<T> GetEnumerator()
        {
            return new PreOrderEnumerator(root);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        private class PreOrderEnumerator : IEnumerator<T>
        {
            private Node<T> _root;
            private Stack<Node<T>> _stack;
            private Node<T> _current;

            public PreOrderEnumerator(Node<T> root)
            {
                _root = root;
                _stack = new Stack<Node<T>>();
                Reset();
            }

            public T Current => _current.Data;

            object IEnumerator.Current => Current;

            public bool MoveNext()
            {
                if (_stack.Count == 0) return false;

                _current = _stack.Pop();

                if (_current.Right != null) _stack.Push(_current.Right);
                if (_current.Left != null) _stack.Push(_current.Left);

                return true;
            }

            public void Reset()
            {
                _stack.Clear();
                if (_root != null) _stack.Push(_root);
                _current = null;
            }

            public void Dispose() { }
        }
    }
}
