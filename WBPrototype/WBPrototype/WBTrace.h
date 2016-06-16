#pragma once

namespace WBPrototype
{
    struct CWBTrace
    {
        DELETE_COMPILER_DEFAULT(CWBTrace);
        CWBTrace() {}
    public:
        static int Initialise(const std::string s);
        static void Cleanup();
        static std::ostream& GetStream();
    };
}
