using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices.Swift;

namespace Project1;

class Program {
    public static void Main(string[] args) {
        // ========== Exercise 1 ==============================
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
        /*
        Inventory<Item> inv = new Inventory<Item>();
        
        inv.Add(new Item("Potion", 10));
        inv.Add(new Item("Sword", 100));
        inv.Add(new Item("Shield", 75));

        //inv.PrintAll();

        var item = inv.FindByName("Sword");
        if (item is not null)
        {
            Console.WriteLine(item.Name);
        }
        */
        
        // ========== Exercise 4 ==============================
        
        
    }
}