#pragma once

#include "../framework.h"

std::wstring ShowFileOpenDialog(HWND hwnd)
{
    OPENFILENAME ofn;       // common dialog box structure
    wchar_t szFile[260];       // buffer for file name

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    wstring resultWString;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        // ������ ������ ��θ� �޽��� �ڽ��� ���
        MessageBox(hwnd, ofn.lpstrFile, L"Selected File", MB_OK);

        resultWString = ConvertLPWSTRToString(ofn.lpstrFile);
        staticName::SelectFileName = ConvertWideStringToString(resultWString);
    }

    return resultWString;
}

std::wstring BrowseForFolder(HWND hWndParent) {
    // BROWSEINFO ����ü ����
    BROWSEINFO bi = { 0 };
    bi.hwndOwner = hWndParent;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    bi.lpfn = NULL;

    // ������ ������ �� �ִ� ���̾�α� ����
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl == NULL) {
        return L""; // ����ڰ� ��Ҹ� ������ ��� �� ���ڿ� ��ȯ
    }

    // ������ ������ ��θ� ���
    wchar_t selectedPath[MAX_PATH];
    if (SHGetPathFromIDList(pidl, selectedPath) == FALSE) {
        return L""; // ��θ� ���� ���� ��� �� ���ڿ� ��ȯ
    }

    // �޸� ����
    IMalloc* pMalloc;
    if (SUCCEEDED(SHGetMalloc(&pMalloc))) {
        pMalloc->Free(pidl);
        pMalloc->Release();
    }

    return selectedPath;
}


void CreateButton(HWND hwnd,int Id,LPWSTR ButtonText, int x, int y, int width = 100, int height = 30) {
    HWND hWndButton = CreateWindowW(
        L"BUTTON",  // Predefined class; Unicode assumed
        ButtonText, // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        x, y, width, height,  // x, y, width, height
        hwnd, // Parent window
        (HMENU)Id, // Button ID
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed

    staticName::ButtonHWNDDatas.push_back(hWndButton);
}

void CreateText(HWND hwnd, LPWSTR Text, int x, int y, int width = 500, int height = 30) {
    // ���̺� ����
    HWND g_hWndLabel = CreateWindowW(
        L"STATIC", // Ŭ���� �̸�
        Text, // �ؽ�Ʈ
        WS_CHILD | WS_VISIBLE | SS_CENTER, // ��Ÿ��
        x,y,width,height, // ��ġ �� ũ��
        hwnd, // �θ� ������ �ڵ�
        NULL, // �޴� �ڵ�
        NULL, // �ν��Ͻ� �ڵ�
        NULL); // �߰� �Ķ����
    
    staticName::TextHWNDLabelDatas.push_back(g_hWndLabel);
}


int CreateTrackBar(HWND hwnd, LPWSTR TrackBarText, int x, int y,int rangeStart = 0, int rangeEnd = 100,int startPos = 0, int width = 300,int textWidth = 150, int height = 30) {
    // Ʈ���� ����
    HWND g_hWndTrackbar = CreateWindowW(
        L"msctls_trackbar32", // Ŭ���� �̸�
        L"Frame ����", // �ؽ�Ʈ
        WS_CHILD | WS_VISIBLE | 0x0001 | 0x00000800, // ��Ÿ��
        x, y, width, height, // ��ġ �� ũ��
        hwnd, // �θ� ������ �ڵ�
        (HMENU)staticName::HWNDDatasIdNums, // ��Ʈ�� ID
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), // �ν��Ͻ� �ڵ�
        NULL); // �߰� �Ķ����

    
    // Ʈ���� ���� ����
    SendMessage(g_hWndTrackbar, TBM_SETRANGE, TRUE, MAKELONG(rangeStart, rangeEnd)); // 0���� 100������ ������ ����
    SendMessage(g_hWndTrackbar, TBM_SETPOS, TRUE, startPos); // �ʱ� ��ġ�� 50���� �����մϴ�.
    SendMessage(g_hWndTrackbar, TBM_SETPAGESIZE, 1, 0); // ������ ������ ����
    SendMessage(g_hWndTrackbar, TBM_SETTICFREQ, 10, 0); // ���� ���� ����
    

    // ���̺� ����
    HWND g_hWndLabel = CreateWindowW(
        L"STATIC", // Ŭ���� �̸�
        TrackBarText, // �ؽ�Ʈ
        WS_CHILD | WS_VISIBLE | SS_CENTER, // ��Ÿ��
        10 + x + width, y, textWidth, height, // ��ġ �� ũ��
        hwnd, // �θ� ������ �ڵ�
        NULL, // �޴� �ڵ�
        NULL, // �ν��Ͻ� �ڵ�
        NULL); // �߰� �Ķ����

    staticName::TrackHWNDLabelDatas.push_back(g_hWndLabel);
    staticName::TrackHWNDDatas.push_back(g_hWndTrackbar);
    staticName::HWNDDatasIdNums++;
    return staticName::TrackHWNDDatas.size() - 1;
}

void CreateEDITBox(HWND hwnd,int Buttonid, int textBoxId, LPWSTR text, int x, int y, int width = 500, int height = 30) {
    HWND hWndTextBox = CreateWindow(
        L"EDIT",
        L">>",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        x, y,width, height,
        hwnd,
        (HMENU)textBoxId,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);

    HWND hWndButton = CreateWindow(
        L"BUTTON",
        text,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x + 10 + width, y, 200, 30,
        hwnd,
        (HMENU)Buttonid,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);

    staticName::EditBoxHWNDDatas.push_back(hWndTextBox);
}