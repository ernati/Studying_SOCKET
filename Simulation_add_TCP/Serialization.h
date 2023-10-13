#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "../../simulation/2D/header_background/myVector2D.h"

// ����ȭ �Լ�
void SerializeVector2D(std::vector<Vector2D>& data, char* buffer, size_t bufferSize) {
    size_t dataSize = sizeof(Vector2D) * data.size();
    if (dataSize <= bufferSize) {
        char* src = reinterpret_cast<char*>(data.data());
        memcpy(buffer, src, dataSize);
    }
}

// ������ȭ �Լ�
void DeserializeVector2D(char* buffer, size_t bufferSize, std::vector<Vector2D>& data) {
    size_t dataSize = bufferSize / sizeof(Vector2D);
    data.resize(dataSize);
    char* dest = reinterpret_cast<char*>(data.data());
    memcpy(dest, buffer, bufferSize);
}

void Serial_DeSerial_Test() {
    // ���� ������ ����
    std::vector<Vector2D> originalData;
    for (int i = 0; i < 8000; ++i) {
        Vector2D v = Vector2D((double)(0.1 * (8000 - i)), (double)(0.2 * i));
        originalData.push_back(v);
        std::cout << "x: " << v.X << ", y: " << v.Y << std::endl;
    }

    std::cout << "============================================" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "============================================" << std::endl;


    // ����ȭ �� ������ȭ �׽�Ʈ
    size_t bufferSize = originalData.size() * sizeof(Vector2D);
    char* buffer = (char*)malloc(sizeof(char*) * bufferSize);
    SerializeVector2D(originalData, buffer, bufferSize);

    std::vector<Vector2D> receivedData;
    DeserializeVector2D(buffer, bufferSize, receivedData);

    // ��� Ȯ��
    for (const Vector2D& v : receivedData) {
        std::cout << "x: " << v.X << ", y: " << v.Y << std::endl;
    }


    free(buffer);

    return;
}
