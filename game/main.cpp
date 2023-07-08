#include <libmaria.h>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("please execute with a project.yaml file");
        return -1;
    }

#ifdef _WIN32
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif

    maria::application app(argv[1]);
    app.run();

    return 0;
}