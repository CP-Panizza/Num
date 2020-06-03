#ifndef _LOAD_DATA_
#define _LOAD_DATA_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include<cinttypes>
#include "matrix.hpp"
using namespace std;


//把大端数据转换为我们常用的小端数据
uint32_t swap_endian(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}


void readAndSave(const string& mnist_img_path, const string& mnist_label_path)
{
    //以二进制格式读取mnist数据库中的图像文件和标签文件
    ifstream mnist_image(mnist_img_path, ios::in | ios::binary);
    ifstream mnist_label(mnist_label_path, ios::in | ios::binary);
    if (!mnist_image.is_open())
    {
        cout << "open mnist image file error!" << endl;
        return;
    }
    if (!mnist_label.is_open())
    {
        cout << "open mnist label file error!" << endl;
        return;
    }

    uint32_t magic;//文件中的魔术数(magic number)
    uint32_t num_items;//mnist图像集文件中的图像数目
    uint32_t num_label;//mnist标签集文件中的标签数目
    uint32_t rows;//图像的行数
    uint32_t cols;//图像的列数
    //读魔术数
    mnist_image.read(reinterpret_cast<char*>(&magic), 4);
    magic = swap_endian(magic);
    if (magic != 2051)
    {
        cout << "this is not the mnist image file" << endl;
        return;
    }
    mnist_label.read(reinterpret_cast<char*>(&magic), 4);
    magic = swap_endian(magic);
    if (magic != 2049)
    {
        cout << "this is not the mnist label file" << endl;
        return;
    }
    //读图像/标签数
    mnist_image.read(reinterpret_cast<char*>(&num_items), 4);
    num_items = swap_endian(num_items);
    mnist_label.read(reinterpret_cast<char*>(&num_label), 4);
    num_label = swap_endian(num_label);
    //判断两种标签数是否相等
    if (num_items != num_label)
    {
        cout << "the image file and label file are not a pair" << endl;
    }
    //读图像行数、列数
    mnist_image.read(reinterpret_cast<char*>(&rows), 4);
    rows = swap_endian(rows);
    mnist_image.read(reinterpret_cast<char*>(&cols), 4);
    cols = swap_endian(cols);
    //读取图像
    for (int i = 0; i != num_items; i++)
    {
        char* pixels = new char[rows * cols];
        mnist_image.read(pixels, rows * cols);
        char label;
        mnist_label.read(&label, 1);
        auto data = Create2dArray(rows, cols);
        for (int m = 0; m != rows; m++)
        {
            for (int n = 0; n != cols; n++)
            {
                if (pixels[m * cols + n] == 0)
                    data[m][n] = 0;
                else
                    data[m][n] = 255;
            }
        }
        Matrix *image = new Matrix;
        image->width = cols;
        image->height = rows;
        image->data = data;
        string saveFile = "C:\\Users\\Administrator\\Desktop\\Work_bench\\cpp\\Convelution\\cmake-build-debug\\data" + to_string((unsigned int)label) + "_" + to_string(i) + ".pgm";
        image->WriteImg(saveFile);
        delete(image);
    }
}

void load(const string& mnist_img_path, const string& mnist_label_path, vector<Matrix *> *imgs, vector<unsigned int> *labels){
    //以二进制格式读取mnist数据库中的图像文件和标签文件
    ifstream mnist_image(mnist_img_path, ios::in | ios::binary);
    ifstream mnist_label(mnist_label_path, ios::in | ios::binary);
    if (!mnist_image.is_open())
    {
        cout << "open mnist image file error!" << endl;
        return;
    }
    if (!mnist_label.is_open())
    {
        cout << "open mnist label file error!" << endl;
        return;
    }

    uint32_t magic;//文件中的魔术数(magic number)
    uint32_t num_items;//mnist图像集文件中的图像数目
    uint32_t num_label;//mnist标签集文件中的标签数目
    uint32_t rows;//图像的行数
    uint32_t cols;//图像的列数
    //读魔术数
    mnist_image.read(reinterpret_cast<char*>(&magic), 4);
    magic = swap_endian(magic);
    if (magic != 2051)
    {
        cout << "this is not the mnist image file" << endl;
        return;
    }
    mnist_label.read(reinterpret_cast<char*>(&magic), 4);
    magic = swap_endian(magic);
    if (magic != 2049)
    {
        cout << "this is not the mnist label file" << endl;
        return;
    }
    //读图像/标签数
    mnist_image.read(reinterpret_cast<char*>(&num_items), 4);
    num_items = swap_endian(num_items);
    mnist_label.read(reinterpret_cast<char*>(&num_label), 4);
    num_label = swap_endian(num_label);
    //判断两种标签数是否相等
    if (num_items != num_label)
    {
        cout << "the image file and label file are not a pair" << endl;
    }
    //读图像行数、列数
    mnist_image.read(reinterpret_cast<char*>(&rows), 4);
    rows = swap_endian(rows);
    mnist_image.read(reinterpret_cast<char*>(&cols), 4);
    cols = swap_endian(cols);
    //读取图像
    std::cout << "loading..." << std::endl;
    for (int i = 0; i != num_items; i++)
    {
        char* pixels = new char[rows * cols];
        mnist_image.read(pixels, rows * cols);
        char label;
        mnist_label.read(&label, 1);
        auto data = Create2dArray(rows, cols);
        for (int m = 0; m != rows; m++)
        {
            for (int n = 0; n != cols; n++)
            {
                if (pixels[m * cols + n] == 0)
                    data[m][n] = 0;
                else
                    data[m][n] = 255;
            }
        }
        Matrix *image = new Matrix;
        image->width = cols;
        image->height = rows;
        image->data = data;
        imgs->push_back(image);
        labels->push_back((unsigned int)label);
    }
}

#endif