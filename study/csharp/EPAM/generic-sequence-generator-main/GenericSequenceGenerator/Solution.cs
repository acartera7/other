// !!! Predefined class name 'Solution' and method names !!!
//                  !!! DO NOT CHANGE !!!

public static class Solution
{
    /// <summary>
    /// Generates a sequence of characters using the specified count, previous character, and current character.
    /// This method will call the `CharSequenceGenerator` to generate the sequence.
    /// </summary>
    /// <param name="count">The number of characters to generate in the sequence.</param>
    /// <param name="previous">The previous character in the sequence.</param>
    /// <param name="current">The current character in the sequence.</param>
    /// <returns>A list of characters representing the generated sequence.</returns>
    public static IList<char> HowToUseCharSequenceGenerator(int count, char previous, char current)
    {
        ISequenceGenerator<char> generator = new CharSequenceGenerator(previous, current);

        var sequence = new List<char> { generator.Previous, generator.Current };

        for (int i = 1; i <= count; i++)
        {
            sequence.Add(generator.Next);
        }

        return sequence;
    }

    /// <summary>
    /// Generates a sequence of integers using the specified count, previous integer, and current integer.
    /// This method will call the `IntegerSequenceGenerator` to generate the sequence.
    /// </summary>
    /// <param name="count">The number of integers to generate in the sequence.</param>
    /// <param name="previous">The previous integer in the sequence.</param>
    /// <param name="current">The current integer in the sequence.</param>
    /// <returns>A list of integers representing the generated sequence.</returns>
    public static IList<int> HowToUseIntegerSequenceGenerator(int count, int previous, int current)
    {
        // Add your code here similar to the `HowToUseCharSequenceGenerator` method.

        ISequenceGenerator<int> generator = new IntegerSequenceGenerator(previous, current);

        var sequence = new List<int> { generator.Previous, generator.Current };

        for (int i = 1; i <= count; i++)
        {
            sequence.Add(generator.Next);
        }

        return sequence;
    }

    /// <summary>
    /// Generates a sequence of integers using the specified count, previous integer, and current integer.
    /// This method will call the `FibonacciSequenceGenerator` to generate the sequence.
    /// </summary>
    /// <param name="count">The number of integers to generate in the sequence.</param>
    /// <param name="previous">The previous integer in the sequence.</param>
    /// <param name="current">The current integer in the sequence.</param>
    /// <returns>A list of integers representing the generated sequence.</returns>
    public static IList<int> HowToUseFibonacciSequenceGenerator(int count, int previous, int current)
    {
        // Add your code here similar to the `HowToUseCharSequenceGenerator` method.
        ISequenceGenerator<int> generator = new FibonacciSequenceGenerator(previous, current);

        var sequence = new List<int> { generator.Previous, generator.Current };

        for (int i = 1; i <= count; i++)
        {
            sequence.Add(generator.Next);
        }

        return sequence;
    }

    /// <summary>
    /// Generates a sequence of doubles using the specified count, previous double, and current double.
    /// This method will call the `DoubleSequenceGenerator` to generate the sequence.
    /// </summary>
    /// <param name="count">The number of doubles to generate in the sequence.</param>
    /// <param name="previous">The previous double in the sequence.</param>
    /// <param name="current">The current double in the sequence.</param>
    /// <returns>A list of doubles representing the generated sequence.</returns>
    public static IList<double> HowToUseDoubleSequenceGenerator(int count, double previous, double current)
    {
        // Add your code here similar to the `HowToUseCharSequenceGenerator` method.
        ISequenceGenerator<double> generator = new DoubleSequenceGenerator(previous, current);

        var sequence = new List<double> { generator.Previous, generator.Current };

        for (int i = 1; i <= count; i++)
        {
            sequence.Add(generator.Next);
        }

        return sequence;
    }


    /// <summary>
    /// Generates a sequence of elements using the specified count, previous element, current element, and a delegate function.
    /// This method will call the `DelegateSequenceGenerator` to generate the sequence.
    /// </summary>
    /// <typeparam name="T">The type of elements in the sequence.</typeparam>
    /// <param name="count">The number of elements to generate in the sequence.</param>
    /// <param name="previous">The previous element in the sequence.</param>
    /// <param name="current">The current element in the sequence.</param>
    /// <param name="nextFunc">The delegate function to calculate the next element in the sequence.</param>
    /// <returns>A list of elements representing the generated sequence.</returns>
    public static IList<T> HowToUseDelegateSequenceGenerator<T>(int count, T previous, T current,
        Func<T, T, T> nextFunc)
    {
        // Add your code here similar to the `HowToUseCharSequenceGenerator` method.

        ISequenceGenerator<T> generator = new DelegateSequenceGenerator<T>(previous, current, nextFunc);

        var sequence = new List<T> { generator.Previous, generator.Current };

        for (int i = 1; i <= count; i++)
        {
            sequence.Add(generator.Next);
        }

        return sequence;
    }
}

// TODO: Implement the ISequenceGenerator interface.
public interface ISequenceGenerator<T>
{
    T Previous { get; }
    T Current { get; }
    T Next { get; }
}

// TODO: Implement the SequenceGenerator class.
public abstract class SequenceGenerator<T> : ISequenceGenerator<T>
{
    private T _previous;
    private T _current;

    public T Previous => _previous;
    public T Current => _current;
    public int Count { get; private set; }

    public T Next
    {
        get
        {
            T result = GetNext();

            _previous = _current;
            _current = result;
            Count++;

            return result;
        }
    }

    public SequenceGenerator(T first, T second)
    {
        _previous = first;
        _current = second;
        Count = 2;
    }

    protected abstract T GetNext();
}

// TODO: Implement the CharSequenceGenerator class.
public class CharSequenceGenerator : SequenceGenerator<char>
{
    public CharSequenceGenerator(char first, char second) : base(first, second) { }

    protected override char GetNext()
    {
        var result = (((Current - 'A') + (Previous - 'A')) % 26) + 'A';
        return (char)result;
    }
}

// TODO: Implement the IntegerSequenceGenerator class.
public class IntegerSequenceGenerator : SequenceGenerator<int>
{
    public IntegerSequenceGenerator(int first, int second) : base(first, second) { }

    protected override int GetNext()
    {
        return 6 * Current - 8 * Previous;
    }
}

// TODO: Implement the DelegateSequenceGenerator class.
public class DelegateSequenceGenerator<T> : SequenceGenerator<T>
{
    private readonly Func<T, T, T> _del;

    public DelegateSequenceGenerator(T first, T second, Func<T, T, T> del)
        : base(first, second)
    {
        _del = del;
    }

    protected override T GetNext()
    {
        return _del(Previous, Current);
    }
}

// TODO: Implement the DoubleSequenceGeneratorD class.
public class DoubleSequenceGenerator : SequenceGenerator<double>
{
    public DoubleSequenceGenerator(double first, double second) : base(first, second) { }

    protected override double GetNext()
    {
        return Current + (Previous / Current);
    }
}

// TODO: Implement the FibonacciSequenceGenerator class.
public class FibonacciSequenceGenerator : SequenceGenerator<int>
{
    public FibonacciSequenceGenerator(int first, int second) : base(first, second) { }

    protected override int GetNext()
    {
        return Previous + Current;
    }
}
