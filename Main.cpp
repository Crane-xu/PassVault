#include "PasswordManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 初始化通用控件
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icc);

    hInst = hInstance;

    // 注册窗口类
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"PasswordManagerClass";
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"窗口注册失败!", L"错误", MB_ICONERROR);
        return 1;
    }

    // 创建窗口
    hMainWindow = CreateWindowEx(
        0,
        L"PasswordManagerClass",
        L"密码管理器",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hMainWindow) {
        MessageBox(NULL, L"窗口创建失败!", L"错误", MB_ICONERROR);
        return 1;
    }

    // 创建字体
    hFont = CreateFont(
        10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI"
    );

    hBoldFont = CreateFont(
        10, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI"
    );

    // 显示窗口
    ShowWindow(hMainWindow, nCmdShow);
    UpdateWindow(hMainWindow);

    // 加载默认密码文件
    wchar_t documentsPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, documentsPath))) {
        currentFilePath = std::wstring(documentsPath) + L"\\passwords.json";
        if (GetFileAttributes(currentFilePath.c_str()) != INVALID_FILE_ATTRIBUTES) {
            LoadPasswordsFromFile(currentFilePath);
        }
    }

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 清理资源
    DeleteObject(hFont);
    DeleteObject(hBoldFont);

    return (int)msg.wParam;
}