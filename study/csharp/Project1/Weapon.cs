using System;

namespace Project1;

public interface IWeapon {
   void Use(string attackerName);
}

public class Sword : IWeapon {
   public void Use(string attackerName) {
      Console.WriteLine($"{attackerName} swings a sword");
   }
}

public class Staff : IWeapon {
   public void Use(string attackerName) {
      Console.WriteLine($"{attackerName} casts a spell with a staff!");
   }
}

public class Bow : IWeapon {
   public void Use(string attackerName) {
      Console.WriteLine($"{attackerName} looses an arrow with a bow!");
   }
}