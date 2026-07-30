namespace Project1;

public class Item {
    public string Name { get; }
    public int Value { get; }

    public Item(string name, int value) {
        Name = name;
        Value = value;
    }

    public override string ToString() {
        return $"{Name} (Value: {Value})";
    }
}