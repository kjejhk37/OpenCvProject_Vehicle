#pragma once

#include "../framework.h"

void VideoPlay() {
	if (staticName::SelectFileName == "") return;
	staticName::VideoStart = true;

	VideoCapture cap(staticName::SelectFileName);
	Mat img;

	staticName::curVideoFrameAllNum = cap.get(CAP_PROP_FRAME_COUNT);
	staticName::curVideoFrame = 0;

	int width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

	while (true)
	{
		cap.read(img);
		if (img.empty()) break;

		Mat findImg = ObjectFind(img);

		height = img.rows;
		width = img.cols;

		height *= (float)staticName::VideoSize / 10;
		if (height <=  0) height = 1;
		width *= (float)staticName::VideoSize / 10;
		if (width <= 0) width = 1;

		cv::Mat resizedImg;
		cv::resize(findImg, resizedImg, cv::Size(width, height));

		// Imshow("name",대상) 해당 이미지을 해당 이름의 창으로 출력
		imshow("Video", resizedImg);

		// 대기 밀리 초만큼 대기임
		if (waitKey(staticName::VideoFrameSpeed) == 'q') break;;
		staticName::curVideoFrame++;

		std::wstring SelectPath = to_wstring(staticName::curVideoFrame) + L" / " + to_wstring(staticName::curVideoFrameAllNum);
		SelectPath = L" 현재 프레임 / 총 프레임 : " + SelectPath;
		SetWindowText(staticName::TextHWNDLabelDatas[0], SelectPath.c_str());
	}

	destroyWindow("Video");
	staticName::VideoStart = false;
}

void VideoPlay_WithSave() {
	if (staticName::SelectFileName == "") return;
	if (staticName::SaveVideoName == "") return;
	if (staticName::SelectFolderName == "") return;
	staticName::VideoStart = true;

	VideoCapture cap(staticName::SelectFileName);
	Mat img;

	staticName::curVideoFrameAllNum = cap.get(CAP_PROP_FRAME_COUNT);
	staticName::curVideoFrame = 0;

	int width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

	string FileName = staticName::SelectFolderName + "/" + staticName::SaveVideoName + ".mp4";
	VideoWriter video(FileName, VideoWriter::fourcc('X', '2', '6', '4'), 25, Size(width, height));

	if (!video.isOpened())
	{
		staticName::VideoStart = false;
		return;
	}


	while (true)
	{
		cap.read(img);
		if (img.empty()) break;

		Mat findImg = ObjectFind(img);

		height = img.rows;
		width = img.cols;

		height *= (float)staticName::VideoSize / 10;
		if (height <= 0) height = 1;
		width *= (float)staticName::VideoSize / 10;
		if (width <= 0) width = 1;

		cv::Mat resizedImg;
		cv::resize(findImg, resizedImg, cv::Size(width, height));

		video.write(resizedImg);
		
		// Imshow("name",대상) 해당 이미지을 해당 이름의 창으로 출력
		imshow("Video", resizedImg);

		// 대기 밀리 초만큼 대기임
		if (waitKey(staticName::VideoFrameSpeed) == 'q') break;;
		staticName::curVideoFrame++;

		std::wstring SelectPath = to_wstring(staticName::curVideoFrame) + L" / " + to_wstring(staticName::curVideoFrameAllNum);
		SelectPath = L" 현재 프레임 / 총 프레임 : " + SelectPath;
		SetWindowText(staticName::TextHWNDLabelDatas[0], SelectPath.c_str());
	}
	video.release();
	destroyWindow("Video");
	staticName::VideoStart = false;
}

void WebCamPlay() {

	staticName::VideoStart = true;

	// 0이면 0번 카메라 카메라가 더 있으면 1 ~ 가능
	VideoCapture cap(0);
	Mat img;

	staticName::curVideoFrame = 0;

	int width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

	while (true)
	{
		cap.read(img);
		if (img.empty()) break;
		Mat findImg = ObjectFind(img);

		height = img.rows;
		width = img.cols;

		height *= (float)staticName::VideoSize / 10;
		if (height <= 0) height = 1;
		width *= (float)staticName::VideoSize / 10;
		if (width <= 0) width = 1;

		cv::Mat resizedImg;
		cv::resize(findImg, resizedImg, cv::Size(width, height));


		imshow("WebCam", resizedImg);

		if (waitKey(1) == 'q') break;;
		staticName::curVideoFrame++;

		std::wstring SelectPath = to_wstring(staticName::curVideoFrame);
		SelectPath = L" 현재 프레임 : " + SelectPath;
		SetWindowText(staticName::TextHWNDLabelDatas[0], SelectPath.c_str());
	}

	destroyWindow("WebCam");
	staticName::VideoStart = false;
}

void WebCamPlay_WithSave() {
	if (staticName::SaveVideoName == "") return;
	if (staticName::SelectFolderName == "") return;

	staticName::VideoStart = true;

	// 0이면 0번 카메라 카메라가 더 있으면 1 ~ 가능
	VideoCapture cap(0);
	Mat img;

	staticName::curVideoFrame = 0;

	int width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
	int height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

	string FileName = staticName::SelectFolderName + "/" + staticName::SaveVideoName + ".mp4";
	VideoWriter video(FileName, VideoWriter::fourcc('X', '2', '6', '4'), 25, Size(width, height));

	if (!video.isOpened())
	{
		staticName::VideoStart = false;
		return;
	}

	while (true)
	{
		cap.read(img);
		if (img.empty()) break;
		Mat findImg = ObjectFind(img);

		height = img.rows;
		width = img.cols;

		height *= (float)staticName::VideoSize / 10;
		if (height <= 0) height = 1;
		width *= (float)staticName::VideoSize / 10;
		if (width <= 0) width = 1;

		cv::Mat resizedImg;
		cv::resize(findImg, resizedImg, cv::Size(width, height));

		video.write(resizedImg);

		imshow("WebCam", resizedImg);

		if (waitKey(1) == 'q') break;;
		staticName::curVideoFrame++;

		std::wstring SelectPath = to_wstring(staticName::curVideoFrame);
		SelectPath = L" 현재 프레임 : " + SelectPath;
		SetWindowText(staticName::TextHWNDLabelDatas[0], SelectPath.c_str());
	}

	video.release();
	destroyWindow("WebCam");
	staticName::VideoStart = false;
}
