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
        // 선택한 파일의 경로를 메시지 박스로 출력
        MessageBox(hwnd, ofn.lpstrFile, L"Selected File", MB_OK);

        resultWString = ConvertLPWSTRToString(ofn.lpstrFile);
        staticName::SelectFileName = ConvertWideStringToString(resultWString);
    }

    return resultWString;
}

std::wstring BrowseForFolder(HWND hWndParent) {
    // BROWSEINFO 구조체 설정
    BROWSEINFO bi = { 0 };
    bi.hwndOwner = hWndParent;
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
    bi.lpfn = NULL;

    // 폴더를 선택할 수 있는 다이얼로그 열기
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl == NULL) {
        return L""; // 사용자가 취소를 선택한 경우 빈 문자열 반환
    }

    // 선택한 폴더의 경로를 얻기
    wchar_t selectedPath[MAX_PATH];
    if (SHGetPathFromIDList(pidl, selectedPath) == FALSE) {
        return L""; // 경로를 얻지 못한 경우 빈 문자열 반환
    }

    // 메모리 해제
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
    // 레이블 생성
    HWND g_hWndLabel = CreateWindowW(
        L"STATIC", // 클래스 이름
        Text, // 텍스트
        WS_CHILD | WS_VISIBLE | SS_CENTER, // 스타일
        x,y,width,height, // 위치 및 크기
        hwnd, // 부모 윈도우 핸들
        NULL, // 메뉴 핸들
        NULL, // 인스턴스 핸들
        NULL); // 추가 파라미터
    
    staticName::TextHWNDLabelDatas.push_back(g_hWndLabel);
}


int CreateTrackBar(HWND hwnd, LPWSTR TrackBarText, int x, int y,int rangeStart = 0, int rangeEnd = 100,int startPos = 0, int width = 300,int textWidth = 150, int height = 30) {
    // 트랙바 생성
    HWND g_hWndTrackbar = CreateWindowW(
        L"msctls_trackbar32", // 클래스 이름
        L"Frame 조절", // 텍스트
        WS_CHILD | WS_VISIBLE | 0x0001 | 0x00000800, // 스타일
        x, y, width, height, // 위치 및 크기
        hwnd, // 부모 윈도우 핸들
        (HMENU)staticName::HWNDDatasIdNums, // 컨트롤 ID
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), // 인스턴스 핸들
        NULL); // 추가 파라미터

    
    // 트랙바 범위 설정
    SendMessage(g_hWndTrackbar, TBM_SETRANGE, TRUE, MAKELONG(rangeStart, rangeEnd)); // 0부터 100까지의 범위로 설정
    SendMessage(g_hWndTrackbar, TBM_SETPOS, TRUE, startPos); // 초기 위치를 50으로 설정합니다.
    SendMessage(g_hWndTrackbar, TBM_SETPAGESIZE, 1, 0); // 페이지 사이즈 설정
    SendMessage(g_hWndTrackbar, TBM_SETTICFREQ, 10, 0); // 눈금 간격 설정
    

    // 레이블 생성
    HWND g_hWndLabel = CreateWindowW(
        L"STATIC", // 클래스 이름
        TrackBarText, // 텍스트
        WS_CHILD | WS_VISIBLE | SS_CENTER, // 스타일
        10 + x + width, y, textWidth, height, // 위치 및 크기
        hwnd, // 부모 윈도우 핸들
        NULL, // 메뉴 핸들
        NULL, // 인스턴스 핸들
        NULL); // 추가 파라미터

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