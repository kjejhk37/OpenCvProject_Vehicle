// OpenCvProject_Vehicle.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "OpenCvProject_Vehicle.h"

//HWND g_hWndTrackbar;
//HWND g_hWndLabel;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
wstring ShowFileOpenDialog(HWND hwnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    /*======================================================================*/


    /*======================================================================*/
    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OPENCVPROJECTVEHICLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENCVPROJECTVEHICLE));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OPENCVPROJECTVEHICLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OPENCVPROJECTVEHICLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // 메뉴 생성
   HMENU hMenu = CreateMenu();
   HMENU hFileMenu = CreateMenu();


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   staticName::net =  GetDarknet();
   if (staticName::net.empty()) {
       return FALSE;
   }

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int b = sizeof(char);
    int a = sizeof(wchar_t);

    std::wstring SelectPath;
    LPWSTR Temp = new WCHAR[256];

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_FILE_SELECT:
            if (staticName::VideoStart == true) break;
            SelectPath = ShowFileOpenDialog(hWnd);
            SelectPath = L" 파일 경로 : " + SelectPath;
            SetWindowText(staticName::TextHWNDLabelDatas[1], SelectPath.c_str());
            break;
        case ID_VIDEOPLAY_BUTTON:
            if (staticName::VideoStart == false) {
                if(staticName::SaveVideo == false)
                    staticName::threadData.emplace_back(VideoPlay);
                else
                    staticName::threadData.emplace_back(VideoPlay_WithSave);
            }
            break;
        case ID_WEBCAMPLAY_BUTTON:
            if (staticName::VideoStart == false) {
                if (staticName::SaveVideo == false)
                    staticName::threadData.emplace_back(WebCamPlay);
                else
                    staticName::threadData.emplace_back(WebCamPlay_WithSave);
            }
            break;
        case ID_SAVE_VIDEO:
            if (staticName::VideoStart == true) break;

            if (staticName::SaveVideo == false) {
                staticName::SaveVideo = true;

                SelectPath = L" 폴더 경로 : " + stringToWString(staticName::SelectFolderName);
                SetWindowText(staticName::TextHWNDLabelDatas[2], SelectPath.c_str());
                SelectPath = L" 저장할 파일 이름 : " + stringToWString(staticName::SaveVideoName);
                SetWindowText(staticName::TextHWNDLabelDatas[3], SelectPath.c_str());
            }
            else {
                staticName::SaveVideo = false;
                SetWindowText(staticName::TextHWNDLabelDatas[2], LPWSTR());
                SetWindowText(staticName::TextHWNDLabelDatas[3], LPWSTR());
            }
            break;
        case ID_FIND_OBJECT:
            staticName::FindObject = !staticName::FindObject;

            if (staticName::FindObject == false) {
                SelectPath = L"객체 찾기";
                SetWindowText(staticName::ButtonHWNDDatas[5], SelectPath.c_str());
            }
            else
            {
                SelectPath = L"영상만 보기";
                SetWindowText(staticName::ButtonHWNDDatas[5], SelectPath.c_str());
            }
            
            break;
        case ID_EDIT_FILE_NAME:
            if (staticName::VideoStart == true) break;

            GetWindowText(staticName::EditBoxHWNDDatas[0], Temp, 256);
            staticName::SaveVideoName = LPWSTRToString(Temp);

            SelectPath = L" 저장할 파일 이름 : " + ConvertLPWSTRToString(Temp);
            SetWindowText(staticName::TextHWNDLabelDatas[3], SelectPath.c_str());
            break;
        case ID_FOLDER_SELECT:
            if (staticName::VideoStart == true) break;
            if (staticName::SaveVideo == false) break;
            
            SelectPath = BrowseForFolder(hWnd);
            staticName::SelectFolderName = ConvertWideStringToString(SelectPath);

            SelectPath = L" 폴더 경로 : " + SelectPath;
            SetWindowText(staticName::TextHWNDLabelDatas[2], SelectPath.c_str());
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

    }
    break;
    case WM_HSCROLL:
    {
        staticName::VideoFrameSpeed = SendMessage(staticName::TrackHWNDDatas[0], 0x0400, 0, 0); // 트랙바의 현재 위치 가져오기
        if (staticName::VideoFrameSpeed == 0) staticName::VideoFrameSpeed = 1;
        std::wstring videoFrameString = L"VideoSpeed : " + std::to_wstring(staticName::VideoFrameSpeed);
        // 문자열을 레이블에 설정
        SetWindowText(staticName::TrackHWNDLabelDatas[0], videoFrameString.c_str());

        staticName::VideoSize = SendMessage(staticName::TrackHWNDDatas[1], 0x0400, 0, 0); // 트랙바의 현재 위치 가져오기
        std::wstring videoSizeString = L"VideoSize : " + std::to_wstring(staticName::VideoSize);
        // 문자열을 레이블에 설정
        SetWindowText(staticName::TrackHWNDLabelDatas[1], videoSizeString.c_str());

    }
    break;
    case WM_CREATE:
        CreateButton(hWnd, ID_FILE_SELECT, (LPWSTR)L"파일 선택", 10, 10);
        CreateButton(hWnd, ID_FOLDER_SELECT, (LPWSTR)L"폴더 선택", 120, 10);
        CreateButton(hWnd, ID_VIDEOPLAY_BUTTON, (LPWSTR)L"비디오 플레이", 10, 50, 210, 30);
        CreateButton(hWnd, ID_WEBCAMPLAY_BUTTON, (LPWSTR)L"웹 캠 플레이", 230, 50, 210, 30);
        CreateButton(hWnd, ID_SAVE_VIDEO, (LPWSTR)L"비디오 저장", 230, 10);
        CreateButton(hWnd, ID_FIND_OBJECT, (LPWSTR)L"객체 찾기", 340, 10);


        CreateText(hWnd, (LPWSTR)L"비디오 플레이 퍼센트 : ", 10, 100, 460, 20);

        CreateText(hWnd, (LPWSTR)L"선택 파일 경로 : ", 10, 130, 460, 20);
        CreateText(hWnd, (LPWSTR)L"선택 폴더 경로 : ", 10, 160, 460, 20);
        CreateText(hWnd, (LPWSTR)L"저장할 파일 이름 : ", 10, 190, 460, 20);
        CreateEDITBox(hWnd, ID_EDIT_FILE_NAME, ID_EDIT_FILE_NAME_TEXTBOX, (LPWSTR)L"저장할 파일 이름",10,220);


        CreateTrackBar(hWnd, (LPWSTR)L"VideoFrame : 1", 10, 260,1,100,1);
        CreateTrackBar(hWnd, (LPWSTR)L"VideoSize : 10", 10, 310,1,30,10);


        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


