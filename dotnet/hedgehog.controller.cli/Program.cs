using System;
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
            while (true)
            {
                Console.WriteLine("Hedgehog Controller Configurator");
                Console.WriteLine("================================");

                Console.WriteLine($"0: Load Configuration from File [{SelectedConfig?.Controller.Name}]");
                Console.WriteLine($"1: Select Hedgehog Port [{SelectedPort}]");
                Console.WriteLine("2: Tickle the Hedgehog");
                Console.WriteLine("D: Debug the Hedgehog");
                Console.WriteLine("S: Write Sample Configuration File");
                Console.WriteLine("R: Reset the Hedgehog");
                Console.WriteLine("X: Exit");


                Console.Write("Press the key corresponding to the desired option: ");
                var option = Console.ReadKey();
                Console.WriteLine();

                try
                {
                    switch (option.KeyChar)
                    {
                        case '0':
                            Program.RunLoadConfig();
                            break;
                        case '1':
                            Program.RunSelectPort();
                            break;
                        case '2':
                            Program.RunProgrammer();
                            break;
                        case 'd':
                        case 'D':
                            Program.RunDebugger();
                            break;
                        case 's':
                        case 'S':
                            Program.RunWriteSampleConfig();
                            break;
                        case 'r':
                        case 'R':
                            Program.RunReset();
                            break;
                        case 'x':
                        case 'X':
                            Console.WriteLine("Okaybye");
                            return;
                        default:
                            Console.WriteLine("Computer Says 'No'");
                            break;
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("ERROR: " + ex.Message);
                }

                Console.WriteLine();
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
                    Console.WriteLine("Done debugging.");
                }
                else
                {
                    Console.WriteLine("Debugging failed. :-o");
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

            Console.WriteLine("Serial Ports");
            Console.WriteLine("============");

            for (int x = 0; x < ports.Length; x++)
            {
                Console.WriteLine($"{x}: {ports[x]}");
            }

            Console.WriteLine();
            Console.Write("Which port?: ");
            var selection = Console.ReadLine();

            if (int.TryParse(selection, out int s) && s >= 0 && s < ports.Length)
            {
                SelectedPort = ports[s];
                Console.WriteLine($"Selected port: {SelectedPort}");
                return true;
            }
            else
            {
                Console.WriteLine($"ERROR: Could not parse port selection: {selection}");
                return false;
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
