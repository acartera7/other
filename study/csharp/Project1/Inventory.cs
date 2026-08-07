using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;

namespace Project1;

public class Inventory<T> {
   private readonly List<T> _items = new List<T>();

   public void Add(T item) {
      _items.Add(item);
   }

   public bool Remove(T item) {
      return _items.Remove((item));
   }

   public IReadOnlyList<T> Items => _items;
}

public static class InventoryExtensions {
   public static void PrintAll<T>(this Inventory<T> inventory) {
      foreach (T item in inventory.Items) {
         Console.WriteLine(item);
      }
   }

   public static T? FindByName<T>(
         this Inventory<T> inventory,
         string name) where T : IHasName {
      foreach (T item in inventory.Items) {
         if (item.Name == name) {
            return item;
         }
      }

      return default;
   }

   // FILTER
   public static IEnumerable<T> WhereValueAbove<T>(
         this Inventory<T> inventory, int threshold)
         where T : Item {
      return inventory.Items.Where((i => i.Value > threshold));
   }

   // SORT
   public static IEnumerable<T> SortedByValue<T>(
         this Inventory<T> inventory)
         where T : Item {
      return inventory.Items.OrderBy(i => i.Value);
   }

   // TRANSFORM
   public static IEnumerable<object> ProjectNameValue<T>(
         this Inventory<T> inventory)
         where T : Item {
      return inventory.Items.Select(i => new { i.Name, i.Value });
   }

   public static IEnumerable<IGrouping<string, T>> GroupByCost<T>(
         this Inventory<T> inventory)
         where T : Item {
      return inventory.Items.GroupBy(i => i.Value > 50 ? "Expensive" : "Cheap");
   }

   public static int TotalValue<T>(this Inventory<T> inventory)
         where T : Item {
      return inventory.Items.Sum(i => i.Value);
   }
}