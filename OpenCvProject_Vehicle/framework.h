// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commdlg.h>
#include <Commctrl.h>
#include <ShlObj.h>

#include <string>
#include <fstream>
#include <locale>
#include <codecvt>

#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>

// #include <torch/script.h>

#include "MyFile/Utility.h"

using namespace std;
using namespace cv;
using namespace dnn;

#define ID_FILE_SELECT 1
#define ID_VIDEOPLAY_BUTTON 2
#define ID_WEBCAMPLAY_BUTTON 3
#define ID_MY_VIDEO_FRAME 4
#define ID_FOLDER_SELECT 5
#define ID_SAVE_VIDEO 6
#define ID_FIND_OBJECT 7
#define ID_EDIT_FILE_NAME 8
#define ID_EDIT_FILE_NAME_TEXTBOX 9
#define CLASS_NAME L"Sample Window Class"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

namespace staticName
{
	string SelectFileName = "";
	string SelectFolderName = "";
	int VideoFrameSpeed = 1;
	int VideoSize = 10;
	int curVideoFrameAllNum = 1;
	int curVideoFrame = 0;
	bool VideoStart = false;
	bool SaveVideo = true;
	bool FindObject = false;

	string SaveVideoName = "";

	vector<HWND> TextHWNDLabelDatas;
	vector<HWND> TrackHWNDDatas;
	vector<HWND> TrackHWNDLabelDatas;
	vector<HWND> ButtonHWNDDatas;
	vector<HWND> EditBoxHWNDDatas;
	int HWNDDatasIdNums = 1001;

	vector<thread> threadData;

	Net net;

	vector<string> classes;
}


#include "OpenCv/SaveYolo.h"
#include "OpenCv/ObjectFind.h"
#include "OpenCv/VideoPlay.h"
#include "MyFile/CreateMenu.h"
