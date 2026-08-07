using System.Reflection.Metadata.Ecma335;

namespace Project1;

public interface ICharacter {
   void Attack();
}

public abstract class Character : ICharacter {
   public string Name { get; set; }
   public int Health { get; private set; }

   protected Character(string name, int health) {
      Name = name;
      Health = health;
   }

   public abstract void Attack();
}

public class Warrior : Character {
   private readonly IWeapon _weapon;

   public Warrior(string name, int health, IWeapon weapon)
         : base(name, health) {
      _weapon = weapon;
   }

   public override void Attack() {
      _weapon.Use(Name);
   }
}

public class Mage : Character {
   private readonly IWeapon _weapon;

   public Mage(string name, int health, IWeapon weapon)
         : base(name, health) {
      _weapon = weapon;
   }

   public override void Attack() {
      _weapon.Use(Name);
   }
}

public class Archer : Character {
   private readonly IWeapon _weapon;

   public Archer(string name, int health, IWeapon weapon)
         : base(name, health) {
      _weapon = weapon;
   }

   public override void Attack() {
      _weapon.Use(Name);
   }
}