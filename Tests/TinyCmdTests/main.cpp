#include <cstdio>
#include <cstdlib>
#include "Resource.h"

static void Fail(const char* msg)
{
    std::printf("FAIL %s\n", msg);
    std::exit(1);
}

int main()
{
    if (IDD_ABOUTBOX != 100)
        Fail("about");
    if (IDD_CMDUI_DIALOG != 102)
        Fail("dialog");
    if (IDC_CMD_RESULT != 1000)
        Fail("result");
    if (IDC_COMMAND != 1001)
        Fail("command");
    if (IDC_RUN_CMD != 1006)
        Fail("run");
    std::printf("OK TinyCmdTests\n");
    return 0;
}
