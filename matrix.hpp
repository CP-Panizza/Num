//
// Created by Administrator on 2020/6/4.
//

#ifndef CONVELUTION_MATRIX_HPP
#define CONVELUTION_MATRIX_HPP
#include <fstream>
#include <cstdlib>
#include <memory.h>
#include <c++/vector>
#include <c++/iostream>

template <class Type>
Type **Create2dArray(int row, int col) {
    Type **array = new Type*[row];
    for (int i = 0; i < row; ++i) {
        array[i] = new Type[col];
        memset(array[i], 0, sizeof(Type) * col);
    }
    return array;
}

template <class Type>
class Matrix {
public:
    int width;
    int height;
    Type **data = nullptr;


    Matrix() {}

    ~Matrix() {
        for (int i = 0; i < this->height; ++i) {
            delete[](this->data[i]);
        }
        delete[](this->data);
    }

    Matrix(const std::initializer_list<std::initializer_list<Type >> &inList) {
        this->height = static_cast<int>(inList.size());
        this->width = static_cast<int>(inList.begin()->size());
        this->data = Create2dArray<Type>(this->height, this->width);
        int i = 0, j = 0;
        for (auto &x :inList) {
            for (auto &y: x) {
                this->data[i][j] = y;
                j++;
            }
            j = 0;
            i++;
        }
    }

    Matrix(const std::initializer_list<Type> &inList) {
        this->width = static_cast<int>(inList.size());
        this->height = 1;
        this->data = Create2dArray<Type>(this->height, this->width);
        int i = 0;
        for (auto &x :inList) {
            this->data[0][i] = x;
            i++;
        }
    }

    Matrix(int h, int w, Type *_data) {
        this->data = Create2dArray<Type>(h, w);
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                this->data[j][i] = _data[w * j + i];
            }
        }

        this->width = w;
        this->height = h;
    }

    Matrix(int h, int w, Type **_data) {
        this->width = w;
        this->height = h;
        this->data = _data;
    }

    void Out(){
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                std::cout << this->Get(i, j) << ", ";
            }
            std::cout << "\n";
        }
    }

    Matrix<Type> *Copy() {
        auto t = new Matrix;
        t->width = this->width;
        t->height = this->height;
        t->data = Create2dArray<Type>(this->height, this->width);
        for (int j = 0; j < this->height; ++j) {
            for (int i = 0; i < this->width; ++i) {
                t->Set(j, i, this->Get(j, i));
            }
        }
        return t;
    }

    void WriteImg(const std::string name) {
        std::ofstream f(name);
        if (f.is_open()) {
            f << "P2\n" << this->width << " " << this->height << "\n255\n";
            int k = 1;
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    if (k % 10) {
                        f << this->data[i][j] << " ";
                    } else {
                        f << this->data[i][j] << "\n";
                    }
                    k++;
                }
            }
        } else {
            perror("open outfile err!");
        }
        f.close();
    }

    bool Set(int row, int col, Type val) {
        if (col > this->width || row > this->height) return false;
        this->data[row][col] = val;
        return true;
    }

    Type Get(int row, int col) {
        if (col > this->width || row > this->height) return -1;
        return this->data[row][col];
    }

    Matrix<Type> *T() {
        auto t = Create2dArray<Type>(this->width, this->height);
        auto t_mat = new Matrix<Type>;
        t_mat->height = this->width;
        t_mat->width = this->height;
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                t[j][i] = this->Get(i, j);
            }
        }
        t_mat->data = t;
        return t_mat;
    }

    Matrix<Type> *Row(int index) {
        if (index >= this->height || index < 0) {
            printf("err: index %d beyond\n", index);
            exit(-1);
        }
        auto data = new Type[this->width];

        for (int i = 0; i < this->width; ++i) {
            data[i] = this->Get(index, i);
        }
        Matrix<Type> *m = new Matrix<Type>(1, this->width, data);
        delete[](data);
        return m;
    }


    Matrix<Type> *Col(int index) {
        if (index >= this->width || index < 0) {
            printf("err: index %d beyond\n", index);
            exit(-1);
        }
        auto data = new Type[this->height];
        for (int i = 0; i < this->height; ++i) {
            data[i] = this->Get(i, index);
        }
        Matrix<Type> *m = new Matrix<Type>(this->height, 1, data);
        delete[](data);
        return m;
    }

    Type *operator[](int index) {
        return this->data[index];
    }

    Matrix<Type> *operator+(Type a) {
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                m->data[i][j] = m->data[i][j] + a;
            }
        }
        return m;
    }

    Matrix<Type> *operator*(Type a) {
        Matrix<Type> *m = this->Copy();
        for (int i = 0; i < m->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                m->data[i][j] = m->data[i][j] * a;
            }
        }
        return m;
    }


    Matrix<Type> *operator+(Matrix<Type> *a) {
        if (a->height == this->height && a->width == this->width) {
            auto data = Create2dArray<Type>(this->height, this->width);
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    data[i][j] = this->Get(i, j) + a->Get(i, j);
                }
            }
            Matrix<Type> *t = new Matrix<Type>(this->height, this->width, data);
            return t;
        }

        if (a->height != 1 || a->width != this->width) {
            printf("dim not match: %d x %d --- %d x %d", this->height, this->width, a->height, a->width);
            exit(-1);
        }
        Matrix<Type> *m = this->Copy();
        Matrix<Type> *row = nullptr;
        for (int i = 0; i < m->height; ++i) {
            row = m->Row(i);
            for (int j = 0; j < this->width; ++j) {
                m->data[i][j] = m->Get(i, j) + a->Get(0, j);
            }
            delete (row);
        }
        return m;
    }


    Matrix<Type> *operator-(Matrix<Type> *a) {

        if (a->height == this->height && a->width == this->width) {
            auto data = Create2dArray<Type>(this->height, this->width);
            for (int i = 0; i < this->height; ++i) {
                for (int j = 0; j < this->width; ++j) {
                    data[i][j] = this->Get(i, j) - a->Get(i, j);
                }
            }
            Matrix<Type> *t = new Matrix<Type>(this->height, this->width, data);
            return t;
        }


        if (a->height != 1 || a->width != this->width) {
            printf("dim not match: %d x %d --- %d x %d", this->height, this->width, a->height, a->width);
            exit(-1);
        }
        Matrix<Type> *m = this->Copy();
        Matrix<Type> *row = nullptr;
        for (int i = 0; i < m->height; ++i) {
            row = m->Row(i);
            for (int j = 0; j < this->width; ++j) {
                m->data[i][j] = m->Get(i, j) - a->Get(0, j);
            }
            delete (row);
        }
        return m;
    }


    Matrix<Type> *Dot(Matrix<Type> *m) {
        if (this->width != m->height) {
            printf("dim not match: %d x %d --- %d x %d", this->height, this->width, m->height, m->width);
            exit(-1);
        }
        auto data = Create2dArray<Type>(this->height, m->width);
        double sum = 0.0;
        Matrix<Type> *row = nullptr;
        Matrix<Type> *col = nullptr;
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < m->width; ++j) {
                row = this->Row(i);
                col = m->Col(j)->T();
                for (int k = 0; k < row->width; ++k) {
                    sum += row->Get(0, k) * col->Get(0, k);
                }
                data[i][j] = sum;
                sum = 0.0;
                delete (row);
                delete (col);
            }
        }
        Matrix<Type> *result = new Matrix<Type>(this->height, m->width, data);
        return result;
    }

    Matrix<Type> *Reshape(int row, int col){
        if(row == -1){
            row = (this->width * this->height) / col;
        } else if(col == -1){
            col = (this->width * this->height) / row;
        }
        if(row * col != this->width * this->height){
            printf("element num is not match!\n");
            exit(-1);
        }
        Matrix<Type> *line = nullptr;
        auto all = new std::vector<Type>;
        for (int i = 0; i < this->height; ++i) {
            line = this->Row(i);
            for (int j = 0; j < line->width; ++j) {
                all->push_back(line->Get(0, j));
            }
            delete(line);
        }
        int index = 0;
        auto data = Create2dArray<Type>(row, col);
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                data[i][j] = (*all)[index++];
            }
        }
        delete(all);
        return new Matrix<Type>(row, col, data);
    }
};


template <class Type>
std::ostream &operator<<(std::ostream &os, Matrix<Type> *matrix) {
    for (int i = 0; i < matrix->height; ++i) {
        for (int j = 0; j < matrix->width; ++j) {
            os << matrix->Get(i, j) << ", ";
        }
        os << "\n";
    }
    return os;
}
#endif //CONVELUTION_MATRIX_HPP