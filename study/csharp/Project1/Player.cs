using System;
using System.Collections.Generic;
using System.IO;

//using System.Linq;
//using System.Net.Http;
//using System.Threading;
//using System.Threading.Tasks;

namespace Project1;

public class Player {
   public string Name { get; set; }
   public int Level { get; set; }
   public int? Health { get; set; }

   public Player(string name, int level, int? health) {
      Name = name;
      Level = level;
      Health = health;
   }

   public void PrintInfo() {
      string healthText = Health.HasValue ? Health.Value.ToString() : "Unknown";
      Console.WriteLine($"{Name} (Level {Level}, Health: {healthText})");
   }
}