
void defaultHandler()                   { Serial1.println("ERROR"); }
void defaultSetHandler(const String&)   { Serial1.println("ERROR"); }

void gomno_get()              { Serial1.println("OK"); }
void gomno_set(const String&) { Serial1.println("OK"); }

struct CommandHandlers
{
    typedef void(*DefHandler)();
    typedef void(*SetHandler)(const String&);

    String      name;
    DefHandler  getHandler;
    SetHandler  setHandler;
    DefHandler  infoHandler;
    DefHandler  testHandler;
};

static const CommandHandlers HANDLERS[] = {
        { "gomno", &gomno_get, &gomno_set, &defaultHandler, &defaultHandler },
        { "start", &start_get, &start_set, &defaultHandler, &defaultHandler },
        { "cgmi", &defaultHandler, &defaultSetHandler, &defaultHandler, &cgmi_get },
        { "gain", &defaultHandler, &gain_set, &defaultHandler, &defaultHandler },
        { "arfcn", &defaultHandler, &arfcn_set, &defaultHandler, &defaultHandler },
};

static const int HANDLER_COUNT = sizeof(HANDLERS) / sizeof(HANDLERS[0]);

void handleGetCommand(const String& aName)
{
    for (int i = 0; i < HANDLER_COUNT; ++i)
    {
        if (HANDLERS[i].name.compareTo(aName))
            continue;

        HANDLERS[i].getHandler();
        return;
    }

    defaultHandler();
}

void handleSetCommand(const String& aName, const String& aValue)
{
    for (int i = 0; i < HANDLER_COUNT; ++i)
    {
        if (HANDLERS[i].name != aName)
            continue;

        HANDLERS[i].setHandler(aValue);
        return;
    }

    defaultSetHandler(aValue);
}

void handleInfoCommand(const String& aName)
{
    for (int i = 0; i < HANDLER_COUNT; ++i)
    {
        if (HANDLERS[i].name != aName)
            continue;

        HANDLERS[i].infoHandler();
        return;
    }

    defaultHandler();
}

void handleTestCommand(const String& aName)
{
    for (int i = 0; i < HANDLER_COUNT; ++i)
    {
        if (HANDLERS[i].name != aName)
            continue;

        HANDLERS[i].testHandler();
        return;
    }

    defaultHandler();
}

void PasreComand(String Cmd)
{
    const int cmdStart = Cmd.indexOf("at+");
    if (cmdStart < 0)
    {
        debug_println("prefix not found");
        return;
    }

    const int cmdName = cmdStart + 3;
    if (Cmd.length() < 3)
    {
        debug_println("invalid command type");
        return;
    }

    debug_print("Comand type ");

    int cmdEndName = Cmd.indexOf("=?", cmdName);
    if (cmdEndName > 0)
    {
        debug_println("GET");
        handleGetCommand(Cmd.substring(cmdName, cmdEndName));
        return;
    }

    cmdEndName = Cmd.indexOf("=", cmdName);
    if (cmdEndName > 0)
    {
        debug_println(" SET");
        handleSetCommand(Cmd.substring(cmdName, cmdEndName), Cmd.substring(cmdEndName + 1));
        return;
    }

    cmdEndName = Cmd.indexOf("?", cmdName);
    if (cmdEndName > 0)
    {
        debug_println(" Info");
        handleInfoCommand(Cmd.substring(cmdName, cmdEndName));
        return;
    }

    debug_println(" Test");
    handleTestCommand(Cmd.substring(cmdName));
    return;
}
