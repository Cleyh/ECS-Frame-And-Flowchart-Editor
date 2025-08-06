static IGlobalSystem *GetSystem()
{
    static IGlobalSystem systemInstance;
    return &systemInstance;
}
