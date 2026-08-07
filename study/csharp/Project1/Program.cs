using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.Swift;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Reflection;
using System.Reflection.Metadata.Ecma335;

namespace Project1;

class Program {
   public static async Task<string> DownloadAsync(string url) {
      await Task.Delay(1000);
      return $"Downloaded content from {url}";
   }

   public static async Task<List<string>> DownloadAllAsync(IEnumerable<string> urls) {
      var tasks = urls.Select(url => DownloadAsync(url));
      var results = await Task.WhenAll(tasks);
      return results.ToList();
   }

   public static async Task MainAsync() {
      var urls = new List<string> {
            "https://example.com/a",
            "https://example.com/b",
            "https://example.com/c"
      };

      var results = await DownloadAllAsync(urls);

      foreach (var r in results) {
         Console.WriteLine(r);
      }
   }

   public static void Main(string[] args) {
      // ========== Exercise 1 ==============================
      // C# Syntax & Types

      /*
      Player p1 = new Player("p1",10, 100);
      Player p2 = new Player("p2", 5, null);
      Player p3 = new Player("p3", 7, 80);

      List<Player> players = new List<Player>();
      players.Add(p1);
      players.Add(p2);
      players.Add(p3);

      p2.Level = 5;
      p2.Health = 50;

      foreach (Player p in players) {
          p.PrintInfo();
      }
      */

      // ========== Exercise 2 ==============================
      // OOP & SOLID Foundations
      /*
      IWeapon sword = new Sword();
      IWeapon staff = new Staff();
      IWeapon bow = new Bow();

      List<ICharacter> characters = new List<ICharacter> {
          new Warrior("Joe", 100, sword),
          new Mage("Mara", 50, staff),
          new Archer("Clyde", 70, bow)
      };

      foreach (var c in characters) {
          c.Attack();
      }
      */

      // ========== Exercise 3 ==============================
      // Collections Generics, extension methods
      /*
      Inventory<Item> inv = new Inventory<Item>();

      inv.Add(new Item("Potion", 10));
      inv.Add(new Item("Sword", 100));
      inv.Add(new Item("Shield", 75));

      //inv.PrintAll();

      var item = in v.FindByName("Sword");
      if (item is not null)
      {
          Console.WriteLine(item.Name);
      }
      */

      // ========== Exercise 4 ==============================
      // LINQ
      /*
      Inventory<Item> inv = new Inventory<Item>();

      inv.Add(new Item("Health Potion", 15));
      inv.Add(new Item("Sword", 100));
      inv.Add(new Item("Shield", 75));
      inv.Add(new Item("Flimsy Dagger", 25));
      inv.Add(new Item("War Hammer", 85));
      inv.Add(new Item("Strength Potion", 30));

      var expensive = inv.WhereValueAbove(50);
      var sorted = inv.SortedByValue();
      sorted = sorted.Reverse();
      var projected = inv.ProjectNameValue();
      var grouped = inv.GroupByCost();
      int total = inv.TotalValue();

      Console.WriteLine("Where value above 50:");
      foreach (Item x in expensive) {
        Console.WriteLine(x.ToString());
      }

      Console.WriteLine("");
      Console.WriteLine("Sorted by value");
      foreach (Item x in sorted) {
        Console.WriteLine(x.ToString());
      }

      Console.WriteLine("");
      Console.WriteLine("Projected to object:");
      foreach (var x in projected) {
        Console.WriteLine(x.ToString());
      }

      Console.WriteLine("");
      Console.WriteLine("Group by cost:");
      foreach (var group in grouped) {
        Console.WriteLine(group.Key);

        foreach (Item item in group) {
          Console.WriteLine(item.ToString());
        }
        Console.WriteLine("");
      }

      Console.WriteLine($"Total Inventory Cost: {total}");

      // ========== Exercise 5 ==============================
      Async/Await
      var watch = System.Diagnostics.Stopwatch.StartNew();
      MainAsync().Wait();
      watch.Stop();
      var elapsedMs = watch.ElapsedMillisecoands;

      Console.WriteLine($"Elapsed time: {elapsedMs}");
    */

      // ========== Exercise 6 ==============================
      // IDisposable, using‑blocks, and resource management.

      using (var file = new FakeFile("log.txt")) {
         file.Write("Hello");
         file.Write("World");
      }
      
      var file = new FakeFile("no_using.txt");
      file.Write("Oops");
      file.Dispose();
      
      file.Write("This should throw");

   }
}