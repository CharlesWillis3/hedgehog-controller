using System;
using System.Collections.Generic;

namespace hedgehog.controller.core.model
{
    public enum ActionKind : byte
    {
        unknown   = 0b0000,
        keyboard  = 0b0001,
        joybutton = 0b0010,
        mode      = 0b0100,
        fallback  = 0b1000
    }

    [Flags]
    public enum ActionFlags : byte
    {
        none  = 0b0000,
        chain = 0b0001,
        fire  = 0b0010,
    }

    public class Action
    {
        public ActionKind Kind {get; set;}
        public ActionFlags Flags {get; set;}
        public byte _0 {get; set;}
        public byte _1 {get; set;}
    }

    public class Mode
    {
        public byte Id {get; set;}

        public Dictionary<int, List<Action>> Digitals {get; set;}
    }

    public class Profile
    {
        public bool ModeFallback {get; set;}

        public List<Mode> Modes {get; set;}
    }
}