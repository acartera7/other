using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Dynamic;
using System.Linq;

namespace Project1;

class Product {
    public int ID { get; }
    public string Name { get; } 
    public decimal Price { get; }

    Product(int id, string name, decimal price) {
        ID = id;
        Name = name;
        Price = price;
    }
}

class OrderItem {
    public Product Product { get; }
    public int Quantity { get; }

    public OrderItem(Product product, int quantity) {
        Product = product;
        Quantity = quantity;
    }
}

class Order {
    public int ID { get; }
    private List<OrderItem> _items { get; } = new();

    public IReadOnlyList<OrderItem> Items() => _items;

    public Order(int id) {
        ID = id;
    }

    decimal GetTotal() {
        if (Items.Count != 0) {
            return Items.Sum(item => item.Quantity * item.Product.Price);
        }

        return 0.0m;
    }
}

class OrderService {
    private List<Order> Orders { get; set; }
}

class Program {
    public static void Main(string[] args) { }
}