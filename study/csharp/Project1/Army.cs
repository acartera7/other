using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Project1;

public enum Rank {
   E1_PV1,
   E2_PV2,
   E3_PFC,
   E4_SPC,
   E4_CPL,
   E5_SGT,
   E6_SSG,
   E7_SFC,
   E8_1SG,
   E9_SGM,
   E9_CGM,
   E9_SMA,
   W1_WO1,
   W2_CW2,
   W3_CW3,
   W4_CW4,
   W5_CW5,
   O1_2LT,
   O2_1LT,
   O3_CPT,
   O4_MAJ,
   O5_LTC,
   O6_COL,
   O7_BG,
   O8_MG,
   O9_LTG,
   O10_GEN,
   GOA
   
}
public interface ISoldier {
   string LastName { get; }
   string FirstName { get; }
   (int,int,int) DateOfBirth { get; }
   Rank Rank { get; }
}

public class Soldier : ISoldier {
   public string LastName { get; }
   public string FirstName { get; }
   public (int,int,int) DateOfBirth { get; }
   public Rank Rank { get; }

   public Soldier(string lastName, string firstName, (int, int, int) dateOfBirth, Rank rank) {
      LastName = lastName;
      FirstName = firstName;
      DateOfBirth = dateOfBirth;
      Rank = rank;
   }
   
   public string GetAssignment() {
      return "";
   }
   
   public string GetFullAssignment() {
      return "";
   }
}

public class Team {
   public enum Role {
      Rifle, 
      Grenadier,
      Medic
   }
   public List<Soldier> Soldiers { get; set; } = new();
   public Soldier? TeamLeader { get; set; }
}

public class Squad {
   public enum Role {
      Rifle, 
      Weapons,
      AntiArmor,
      Sniper
   }
   public List<Team> Teams { get; set; } = new();
   public Soldier? SquadLeader { get; set; }
}

public class Platoon {
   public enum Role {
      Rifle, 
      Weapons,
      AntiArmor,
      Sniper,
      Mixed
   }
   public List<Squad> Squads { get; set; } = new();
   public Soldier? PlatoonLeader { get; set; }
}

public class Company {
   public List<Platoon> Platoons { get; set; } = new();
   public Soldier? CompanyCommander { get; set; }
}

public class Battalion {
   public List<Company> Companies { get; set; } = new();
   public Soldier? BattalionCommander { get; set; }
}

public class Brigade {
   public List<Battalion> Battalions { get; set; } = new();
   public Soldier? BrigadeCommander { get; set; }
}

public class Division {
   public List<Brigade> Brigades { get; set; } = new();
   public Soldier? DivisionCommander { get; set; }
}

public class Corp {
   public List<Division> Divisions { get; set; } = new();
   public Soldier? CorpsCommander { get; set; }
}

public class Army {
   public List<Corp> Corps { get; set; } = new();
   public Soldier? ArmyCommander { get; set; }
}