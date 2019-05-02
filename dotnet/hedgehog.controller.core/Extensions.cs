using System.Runtime.InteropServices;
using YamlDotNet.Serialization;

namespace hedgehog.controller.core
{
    public static class Extensions
    {
        public static void PrettyPrint(this object o)
        {
            var serializer = new SerializerBuilder()
                .EmitDefaults()
                .Build();
            System.Console.WriteLine(serializer.Serialize(o));
        }
    }
}