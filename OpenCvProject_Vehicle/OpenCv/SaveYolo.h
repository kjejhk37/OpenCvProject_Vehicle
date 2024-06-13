#pragma once

#include "../framework.h"

// �� ���� ���� �Լ�
void saveModel(const string& modelConfiguration, const string& modelWeights, const string& savePath) {
    ofstream fout(savePath);
    fout << "modelConfiguration: " << modelConfiguration << endl;
    fout << "modelWeights: " << modelWeights << endl;
    fout.close();
}

// �� ���� �ε� �Լ�
void loadModel(string& modelConfiguration, string& modelWeights, const string& loadPath) {
    ifstream fin(loadPath);
    string line;
    while (getline(fin, line)) {
        size_t pos = line.find(": ");
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 2);
            if (key == "modelConfiguration") {
                modelConfiguration = value;
            }
            else if (key == "modelWeights") {
                modelWeights = value;
            }
        }
    }
    fin.close();
}

Net GetDarknet() {

    std::string modelConfiguration = "yolo/yolov3.cfg";
    std::string modelWeights = "yolo/yolov5.weights";
    string modelStateFile = "model_state.txt";

    loadModel(modelConfiguration, modelWeights, modelStateFile);

    // YOLOv3 �� �ε�
    cv::dnn::Net net = cv::dnn::readNet(modelWeights, modelConfiguration, "darknet");
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);

    // Ŭ���� �̸� �ε�
    string classesFile = "yolo/coco.names";

    ifstream ifs(classesFile.c_str());
    string line;
    while (getline(ifs, line)) {
        staticName::classes.push_back(line);
    }

    return net;
}
