//
// Created by long on 2021/11/15.
//

#ifndef SHEEPRENDER_SR_MATRIX_H
#define SHEEPRENDER_SR_MATRIX_H

namespace SR {
    template<int row, int col>
    struct matrix {
        sr_vector <col> m[row] = {{}};

        matrix() = default;

        sr_vector <col> &operator[](const int i) {
            assert(i >= 0 && i < row);
            return m[i];
        }

        const sr_vector <col> &operator[](const int i) const {
            assert(i >= 0 && i < row);
            return m[i];
        }

        void set_row(int i, const sr_vector <col> &v) {
            assert(i >= 0 && i < col);
            m[i] = v;
        }

        void set_col(int i, const sr_vector <row> &v) {
            assert(i >= 0 && i < row);
            for (int j = 0; j < row; j++) {
                m[j][i] = v[j];
            }
        }

        sr_vector <col> get_row(int i) const {
            assert(i >= 0 && i < col);
            return m[i];
        }

        sr_vector <row> get_col(int i) const {
            sr_vector<row> ret;
            for (int j = 0; j < row; j++) {
                ret[j] = (*this)[j][i];
            }
            return ret;
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

        static matrix<row, col> get_translation(vec3 val) {
            assert(row == 4 && col == 4);
            matrix<4, 4> m = identity();
            m[3][0] = val[0];
            m[3][1] = val[1];
            m[3][2] = val[2];
            return m;
        }

        static matrix<row, col> get_scale(vec3 val) {
            assert(row == 4 && col == 4);
            matrix<4, 4> m = identity();
            m[0][0] = val[0];
            m[1][1] = val[1];
            m[2][2] = val[2];
            return m;
        }

        static matrix<row, col> get_x_rotation(float angle) {
            assert(row == 4 && col == 4);
            float c = std::cos(angle);
            float s = std::sin(angle);
            matrix<4, 4> m = identity();
            m[1][1] = c;
            m[1][2] = -s;
            m[2][1] = s;
            m[2][2] = c;
            return m;
        }

        static matrix<row, col> get_y_rotation(float angle) {
            assert(row == 4 && col == 4);
            float c = std::cos(angle);
            float s = std::sin(angle);
            matrix<4, 4> m = identity();
            m[0][0] = c;
            m[0][2] = s;
            m[2][0] = -s;
            m[2][2] = c;
            return m;
        }

        static matrix<row, col> get_z_rotation(float angle) {
            assert(row == 4 && col == 4);
            float c = std::cos(angle);
            float s = std::sin(angle);
            matrix<4, 4> m = identity();
            m[0][0] = c;
            m[0][1] = -s;
            m[1][0] = s;
            m[1][1] = c;
            return m;
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
    matrix<row, col> operator*(const matrix<row, col> &m, const float &val) {
        matrix<row, col> ret;
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                ret[i][j] = m[i][j] * val;
        return ret;
    }

    template<int r1, int c1, int c2>
    matrix<r1, c2> operator*(const matrix<r1, c1> &lm, const matrix<c1, c2> &rm) {
        matrix<r1, c2> ret;
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c2; j++)
                ret[i][j] = lm[i] * rm.get_col(j);
        return ret;
    }

    template<int c1, int c2>
    sr_vector <c2> operator*(const sr_vector <c2> &lv, const matrix<c1, c2> &rm) {
        sr_vector<c2> ret;
        for (int j = 0; j < c2; j++) { ret[j] = lv * rm.get_col(j); }
        return ret;
    }

    template<int row, int col>
    matrix<row, col> operator/(const matrix<row, col> &m, const float &val) {
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
