using System;

namespace Project1;

public class FakeFile : IDisposable {
   private readonly string _name;
   private bool _disposed = false;

   public FakeFile(string name) {
      _name = name;
      Console.WriteLine($"Opening {_name}");
   }

   public void Write(string text) {
      if (_disposed)
         throw new ObjectDisposedException(nameof(FakeFile));

      Console.WriteLine($"Writing to {_name}: {text}");
   }

   public void Dispose() {
      if (_disposed) return;

      Console.WriteLine($"Closing {_name}");

      _disposed = true;
   }
}