using System.Collections.Generic;
using hedgehog.controller.core.interop;

namespace hedgehog.controller.core.model
{
    public class HedgehogController
    {
        public string Name {get; set;}
        
        public Device Device {get; set;}
        
        public List<Joystick> Joysticks {get; set;} = new List<Joystick>(4);
    }
}