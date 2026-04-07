using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace GameLeaderboard {
    internal class Program {
        static void Main(string[] args) {
            var players = new List<Player> {
                new Player(1, "Alice"),
                new Player(2, "Bob"),
                new Player(3, "Charlie")
            };

            var scores = new List<Score> {
                new Score(1, 100, DateTime.Now.AddDays(-1)),
                new Score(2, 150, DateTime.Now.AddDays(-10)),
                new Score(3, 120, DateTime.Now.AddDays(-13)),
                new Score(1, 130, DateTime.Now.AddDays(-14)),
                new Score(2, 110, DateTime.Now.AddDays(-24))
            };

            var leaderboard = scores
                .GroupBy(s => s.PlayerId)
                .Select(g => new {
                    PlayerId = g.Key,
                    TotalScore = g.Sum(x => x.Points)
                })
                .OrderByDescending(x => x.TotalScore);

            foreach (var player in leaderboard) {
                Console.WriteLine($"Player ID: {player.PlayerId}, Total Score: {player.TotalScore}");
            }
        }   
    }

    public class Player {
        public int Id { get; set; }
        public string Name { get; set; }

        public Player(int id, string name) {
            Id = id; Name = name;   
        }
    }

    public class Score {
        public int PlayerId { get; set; }
        public int Points { get; set; }
        public DateTime Date { get; set; }

        public Score(int playerId, int points, DateTime date) {
            PlayerId = playerId; Points = points; Date = date;
        }

    }

}
