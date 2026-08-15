using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
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
     public List<OrderItem> Items { get; } = new();

     public Order(int id) {
         ID = id;
     }

     decimal GetTotal() {
        decimal total = 0.0m;
        if (Items.Count != 0) {
            foreach (var order in Items) {
                total += order.Quantity * order.Product.Price;
            }
        }
        
        return total;
     }
}

class OrderService {
    List<>
}

class Program {
    
    public static void Main(string[] args) {
                
    }
    
}

