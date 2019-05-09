using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace hedgehog.controller.cli
{
    public class Carte
    {
        private readonly string header;
        private readonly bool extendedDebug;

        public Carte(string header, bool extendedDebug = false)
        {
            this.header = header;
            this.extendedDebug = extendedDebug;
        }

        public bool ShowMenu(IDictionary<char, (string description, Action action)> menu)
        {
            if (menu.ContainsKey('X') || menu.ContainsKey('x'))
            {
                throw new ArgumentException($"{nameof(menu)} must not contain an entry for 'X'");
            }

            var normalizedMenu = menu.ToDictionary(x => char.ToLowerInvariant(x.Key));

            StringBuilder display = new StringBuilder()
                .AppendLine(this.header)
                .AppendLine(new string('=', this.header.Length));

            foreach (var entry in normalizedMenu)
            {
                display.AppendLine($"{entry.Value.Key}: {entry.Value.Value.description}");
            }

            display.AppendLine("X: Exit");

            Console.WriteLine();
            Console.WriteLine(display);
            Console.Write("Press the key for the desired option: ");
            var option = Console.ReadKey().KeyChar;
            var normalizedOption = char.ToLowerInvariant(option);

            Console.WriteLine();

            if (normalizedOption == 'x')
            {
                return false;
            }

            if (normalizedMenu.TryGetValue(normalizedOption, out var result))
            {
                try
                {
                    result.Value.action();
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine($"ERROR: {ex.Message}");
                    if (this.extendedDebug)
                    {
                        Console.Error.WriteLine(ex.StackTrace);
                    }
                }
            }
            else
            {
                Console.Error.WriteLine($"The selected option is not valid: {option}");
            }

            return true;
        }
    }
}