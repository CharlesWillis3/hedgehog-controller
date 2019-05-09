using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using hedgehog.controller.core;

namespace hedgehog.controller.cli
{
    class Program
    {
        public static string SelectedPort { get; private set; }
        public static Configuration SelectedConfig { get; private set; }

        static void Main(string[] args)
        {
            Carte carte = new Carte("Hedgehog Controller Configurator");
            bool result = true;
            while (result)
            {
                var menu = new Dictionary<char, (string, Action)>()
                {
                    {'0', ($"Load Configuration from File [{SelectedConfig?.Controller.Name}]", Program.RunLoadConfig)},
                    {'1', ($"Select Hedgehog Port [{SelectedPort}]", () => Program.RunSelectPort())},
                    {'2', ("Tickle the Hedgehog", Program.RunProgrammer)},
                    {'D', ("Debug the Hedgehog", Program.RunDebugger)},
                    {'S', ("Write Sample Configuration File", Program.RunWriteSampleConfig)},
                    {'R', ("Reset the Hedgehog", Program.RunReset)}
                };

                result = carte.ShowMenu(menu);
            }
        }

        private static void RunLoadConfig()
        {
            Console.Write("Enter the path with filename: ");
            string path = Console.ReadLine();

            SelectedConfig = Configuration.Deserialize(path);
            SelectedConfig.Controller.PrettyPrint();
        }

        static void RunProgrammer()
        {
            Configuration config = SelectedConfig ?? throw new ArgumentException("A configuration has not been loaded. Try option '0'.");

            if (Program.SelectedPort is null && !Program.RunSelectPort()) return;

            Console.WriteLine("Starting programming...");


            using (var programmer = new Programmer(SelectedPort))
            {
                if (programmer.Start(SelectedConfig))
                {
                    Console.WriteLine("Programming complete.");
                    Console.WriteLine(programmer.NumDigitals);
                    Console.WriteLine(programmer.NumAnalogs);
                    Console.WriteLine(programmer.NumNeoPixels);
                }
                else
                {
                    Console.WriteLine("Programming failed. :-(");
                }
            }
        }

        private static void RunDebugger()
        {
            if (Program.SelectedPort is null && !Program.RunSelectPort()) return;

            Console.WriteLine("Starting Debugging...");

            using (var debugger = new Debugger(SelectedPort))
            {
                if (debugger.Start())
                {
                    Console.WriteLine("Debugging started.");
                }
                else
                {
                    Console.WriteLine("Debugging failed. :-o");
                }

                var menu = new Dictionary<char, (string, Action)>()
                {
                    {'D', ("Get Device Info", debugger.ReadDevice)},
                    {'J', ("Get Joystick Configuration", debugger.ReadJoysticks)},
                    {'B', ("Start Button ID", debugger.ReportButtons)}
                };

                Carte carte = new Carte("Debugging");

                while (carte.ShowMenu(menu))
                {
                }
            }
        }

        private static void RunWriteSampleConfig()
        {
            Console.WriteLine();
            Console.Write("Enter the output path with filename: ");
            string path = Console.ReadLine();

            Configuration.SerializeSampleConfiguration(path);
        }

        private static bool RunSelectPort()
        {
            string[] ports = Programmer.GetPorts();

            Carte carte = new Carte("Serial Ports");

            var menu = new Dictionary<char, (string, Action)>();

            for (int x = 0; x < ports.Length; x++)
            {
                string p = ports[x];
                menu.Add(x.ToString()[0], (p, () => SelectPort(p)));
            }

            return carte.ShowMenu(menu);

            void SelectPort(string port)
            {
                SelectedPort = port;
                Console.WriteLine($"Selected port: {SelectedPort}");
            }
        }

        private static void RunReset()
        {
            if (Program.SelectedPort is null && !Program.RunSelectPort()) return;

            using (var s = new Serial(SelectedPort))
            {
                s.Reset();
            }

            SelectedPort = null;
        }
    }
}
