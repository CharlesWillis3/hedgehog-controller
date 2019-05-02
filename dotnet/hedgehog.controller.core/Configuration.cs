using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using hedgehog.controller.core.interop;
using hedgehog.controller.core.model;
using YamlDotNet.Serialization;
using Action = hedgehog.controller.core.model.Action;

namespace hedgehog.controller.core
{
    public class Configuration
    {
        private const string Tag = "!HedgehogController";
        
        private const string Extension = "hhcont.yaml";

        private static IDeserializer Deserializer = new DeserializerBuilder()
                .WithTagMapping(Tag, typeof(HedgehogController))
                .WithNamingConvention(new YamlDotNet.Serialization.NamingConventions.NullNamingConvention())
                .Build();
        
        private static ISerializer Serializer = new SerializerBuilder()
                .WithTagMapping(Tag, typeof(HedgehogController))
                .WithNamingConvention(new YamlDotNet.Serialization.NamingConventions.NullNamingConvention())
                .EmitDefaults()
                .EnsureRoundtrip()
                .Build();

        public  HedgehogController Controller { get; }
        
        public Configuration(HedgehogController config)
        {
            this.Controller = config ?? throw new ArgumentNullException(nameof(config));
        }

        public static void SerializeSampleConfiguration(string path)
        {
            var joystick0 = new Joystick
            {
                n_buttons = 64,
                n_hats = 0,
                f_axes = AxesFlags.xyz
            };

            var joystick1 = new Joystick
            {
                n_buttons = 50,
                f_axes = AxesFlags.none
            };

            // const int nModes = 2;
            // const int nModeHeaderBytes = nModes * (byte)HHDataSize.MODEHEADER;
            // const int nProfileBytes = HHOffset.LAST - HHOffset.MODE0;
            // const int nActionBytes = nProfileBytes - nModeHeaderBytes;
            // const int nTotalActions = nActionBytes / (byte)HHDataSize.ACTION;

            var device = new Device
            {
                f_flags = DeviceFlags.keyboard | DeviceFlags.joystick,
                n_joysticks = 2,
                n_modes = 2
            };

            var config = new HedgehogController
            {
                Name = "Sample Configuration",
                Device = device,
                Joysticks = { joystick0, joystick1 }
            };

            new Configuration(config).Serialize(path);
        }

        public static string EnsurePathExtension(string path)
        {
            if (!Path.GetExtension(path).Equals(Extension))
            {
                path = Path.ChangeExtension(path, Extension);
            }

            return path;
        }

        public static Configuration Deserialize(string path)
        {
            var yaml = File.ReadAllText(EnsurePathExtension(path));

            HedgehogController config = Deserializer.Deserialize<HedgehogController>(yaml);
            Console.WriteLine($"Read configuration '{config.Name}' from: '{path}'");
            return new Configuration(config);
        }

        public void Serialize(string path)
        {
            path = EnsurePathExtension(path);
            var yaml = Serializer.Serialize(this.Controller);
            Console.WriteLine($"Writing configuration '{this.Controller.Name}' to: '{path}'");
            File.WriteAllText(path, yaml, Encoding.UTF8);
        }
    }
}