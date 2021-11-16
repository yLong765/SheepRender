//
// Created by yangyinlong01 on 2021/11/15.
//

#ifndef SHEEPRENDER_SR_MATRIX_H
#define SHEEPRENDER_SR_MATRIX_H

#include <string>
#include <cassert>

namespace SR {
    template<int row, int col>
    struct matrix {
        vector <col> m[row] = {{}};

        matrix() = default;

        vector <col> &operator[](const int i) {
            assert(i >= 0 && i < row);
            return m[i];
        }

        const vector <col> &operator[](const int i) const {
            assert(i >= 0 && i < row);
            return m[i];
        }

        void set_row(int i, const vector <col> &v) {
            assert(i >= 0 && i < col);
            m[i] = v;
        }

        void set_col(int i, const vector <row> &v) {
            assert(i >= 0 && i < row);
            for (int j = 0; j < row; j++) {
                m[j][i] = v[j];
            }
        }

        matrix<row, col> inverse() const {

        }

        matrix<col, row> transpose() const {
            matrix<col, row> ret;
            for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                    ret[j][i] = (*this)[i][j];
            return ret;
        }

        static matrix<row, col> identity() {
            matrix<row, col> ret;
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    ret[i][j] = i == j;
                }
            }
            return ret;
        }

        static matrix<row, col> zero() {
            matrix<row, col> ret;
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    ret[i][j] = 0;
                }
            }
            return ret;
        }
    };

    template<int row, int col>
    std::ostream &operator<<(std::ostream &out, const matrix<row, col> &m) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col - 1; j++) {
                out << m[i][j] << " ";
            }
            out << m[i][col - 1] << "\n";
        }
        return out;
    }

    template<int row, int col>
    matrix<row, col> operator+(const matrix<row, col> &lm, const matrix<row, col> &rm) {
        matrix<row, col> ret;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                ret[i][j] = lm[i][j] + rm[i][j];
        return ret;
    }

    template<int row, int col>
    matrix<row, col> operator-(const matrix<row, col> &lm, const matrix<row, col> &rm) {
        matrix<row, col> ret;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                ret[i][j] = lm[i][j] - rm[i][j];
        return ret;
    }

    template<int row, int col>
    matrix<row, col> operator*(const matrix<row, col> &m, const float val) {
        matrix<row, col> ret;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                ret[i][j] = m[i][j] * val;
        return ret;
    }

    template<int r1, int c1, int c2>
    matrix<r1, c2> operator*(const matrix<r1, c1> &lm, const matrix<c1, c2> rm) {
        matrix<r1, c2> ret;
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c2; j++)
                for (int k = 0; k < c1; k++)
                    ret[i][j] += lm[i][k] * rm[k][j];
        return ret;
    }

    template<int row, int col>
    matrix<row, col> operator/(const matrix<row, col> &m, const float val) {
        matrix<row, col> ret;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                ret[i][j] = m[i][j] / val;
        return ret;
    }

    typedef matrix<2, 2> mat2x2;
    typedef matrix<3, 3> mat3x3;
    typedef matrix<4, 4> mat4x4;
}


#endif //SHEEPRENDER_SR_MATRIX_H
