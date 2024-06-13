#pragma once

#include "../framework.h"

void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame, vector<string>& classes);
/*
Mat ObjectFind(Mat img) {

	if (img.empty()) return img;
	if (staticName::net.empty()) return img;
    if (staticName::FindObject == false) return img;

    Mat frame = img;

    // YOLO 입력 설정
    Mat blob = blobFromImage(frame, 1 / 255.0, Size(416, 416), Scalar(0, 0, 0), true, false);
    staticName::net.setInput(blob);

    // YOLO 추론
    vector<Mat> outs;
    staticName::net.forward(outs, staticName::net.getUnconnectedOutLayersNames());

    // 객체 탐지 결과 처리
    float confThreshold = 0.7; // 신뢰도 임계값
    float nmsThreshold = 0.5; // 겹치는 박스 제거 임계값

    std::vector<cv::Rect> boxes;
    std::vector<float> confidences;
    vector<Point> classIdPoints;

    for (size_t i = 0; i < outs.size(); ++i) {
        float* data = (float*)outs[i].data;

        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
            // int scores = *outs[i].row(j).col(5).data;
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

            if (confidence > confThreshold) {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;
                boxes.push_back(cv::Rect(left, top, width, height));
                confidences.push_back((float)confidence);
                classIdPoints.push_back(classIdPoint);
            }
        }
    }

    // 겹치는 박스 제거
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

    // 겹치지 않는 사각형 그리기
    for (int i = 0; i < indices.size(); ++i) {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        float confidence = confidences[idx];
        Point classPoint = classIdPoints[idx];
        drawPred(classPoint.x, (float)confidence, box.x, box.y, box.x + box.width, box.y + box.height, frame, staticName::classes);
    }

    return frame;
}
*/

Mat ObjectFind(Mat img) {

    if (img.empty()) return img;
    if (staticName::net.empty()) return img;
    if (staticName::FindObject == false) return img;

    Mat frame = img;

    // YOLO 입력 설정
    Mat blob = blobFromImage(frame, 1 / 255.0, Size(416, 416), Scalar(0, 0, 0), true, false);
    staticName::net.setInput(blob);

    // YOLO 추론
    vector<Mat> outs;
    staticName::net.forward(outs, staticName::net.getUnconnectedOutLayersNames());

    // 객체 탐지 결과 처리
    float confThreshold = 0.7; // 신뢰도 임계값
    float nmsThreshold = 0.5; // 겹치는 박스 제거 임계값

    std::vector<cv::Rect> boxes;
    std::vector<float> confidences;
    vector<Point> classIdPoints;

    for (size_t i = 0; i < outs.size(); ++i) {
        float* data = (float*)outs[i].data;

        for (int j = 0; j < outs[i].rows; j++, data += outs[i].cols) {
            float confidence = data[4]; // 신뢰도
            if (confidence <= confThreshold) continue;

            cv::Point classIdPoint((int)data[6], (int)data[5]);

            int centerX = (int)(data[0] * frame.cols);
            int centerY = (int)(data[1] * frame.rows);
            int width = (int)(data[2] * frame.cols);
            int height = (int)(data[3] * frame.rows);
            int left = centerX - width / 2;
            int top = centerY - height / 2;
            boxes.push_back(cv::Rect(left, top, width, height));
            confidences.push_back((float)confidence);
            classIdPoints.push_back(classIdPoint);
        }
    }

    // 겹치는 박스 제거
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

    // 겹치지 않는 사각형 그리기
    for (int i = 0; i < indices.size(); ++i) {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        float confidence = confidences[idx];
        Point classPoint = classIdPoints[idx];
        drawPred(classPoint.x, (float)confidence, box.x, box.y, box.x + box.width, box.y + box.height, frame, staticName::classes);
    }

    return frame;
}




// 객체 탐지 결과를 프레임에 그리는 함수
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame, vector<string>& classes) {
    if (classes.size() > classId)
    {
        if (classes[classId] != "car" && classes[classId] != "truck") return;
    }

    int boxSize_w = right - left;
    int boxSize_h = bottom - top;

    int height = frame.rows;
    int width = frame.cols;

    Scalar BoxColor = Scalar(0, 255, 0);

    if (boxSize_w >= (width / 3) || boxSize_h >= (height / 3)) BoxColor = Scalar(0, 0, 255);

    rectangle(frame, Point(left, top), Point(right, bottom), BoxColor, 1);

    if (boxSize_w < 50 && boxSize_h < 50) return;

    string label = format("%.2f", conf);
    if (!classes.empty()) {
        label = classes[classId] + ": " + label;
    }

    boxSize_w /= 2;
    boxSize_h /= 2;

    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.4, 0.7, &baseLine);
    top = max(top, labelSize.height);
    rectangle(frame, 
        Point(left + boxSize_w - (labelSize.width / 2), top - labelSize.height + boxSize_h),
        Point(left + (labelSize.width / 2) + boxSize_w, top + baseLine + boxSize_h),
        Scalar(255, 255, 255), 
        FILLED);
    putText(frame, label, Point(left + boxSize_w - (labelSize.width / 2), top + boxSize_h), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 0), 0.7);
}

