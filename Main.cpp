#include "PasswordManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // ��ʼ��ͨ�ÿؼ�
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icc);

    hInst = hInstance;

    // ע�ᴰ����
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
        MessageBox(NULL, L"����ע��ʧ��!", L"����", MB_ICONERROR);
        return 1;
    }

    // ��������
    hMainWindow = CreateWindowEx(
        0,
        L"PasswordManagerClass",
        L"���������",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hMainWindow) {
        MessageBox(NULL, L"���ڴ���ʧ��!", L"����", MB_ICONERROR);
        return 1;
    }

    // ��������
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

    // ��ʾ����
    ShowWindow(hMainWindow, nCmdShow);
    UpdateWindow(hMainWindow);

    // ����Ĭ�������ļ�
    wchar_t documentsPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, documentsPath))) {
        currentFilePath = std::wstring(documentsPath) + L"\\passwords.json";
        if (GetFileAttributes(currentFilePath.c_str()) != INVALID_FILE_ATTRIBUTES) {
            LoadPasswordsFromFile(currentFilePath);
        }
    }

    // ��Ϣѭ��
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // ������Դ
    DeleteObject(hFont);
    DeleteObject(hBoldFont);

    return (int)msg.wParam;
}