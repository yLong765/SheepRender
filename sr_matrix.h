//
// Created by yangyinlong01 on 2021/11/15.
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
            for (int j = 0; j < col; j++) {
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

        matrix<row, col> set_translation(vec3 pos) {
            assert(row == 4 && col == 4);
            *this = identity();
            (*this)[3][0] = pos[0];
            (*this)[3][1] = pos[1];
            (*this)[3][2] = pos[2];
            return *this;
        }

        matrix<row, col> set_scale(vec3 pos) {
            assert(row == 4 && col == 4);
            *this = identity();
            (*this)[0][0] = pos[0];
            (*this)[1][1] = pos[1];
            (*this)[2][2] = pos[2];
            return *this;
        }

        matrix<row, col> set_rotation(vec3 pos, float theta) {
            assert(row == 4 && col == 4);
            float qsin = std::sin(theta);
            float qcos = std::cos(theta);
            *this = identity();
            (*this)[0][0] = pos[0];
            (*this)[1][1] = pos[1];
            (*this)[2][2] = pos[2];
            return *this;
        }

        static matrix<row, col> look_at(vec3 from, vec3 to, vec3 up) {
            assert(row == 4 && col == 4);
            vec3 zaxis = (to - from).normalize();
            vec3 xaxis = vec3::cross(up, zaxis).normalize();
            vec3 yaxis = vec3::cross(zaxis, xaxis);

            return {{xaxis.x,                 yaxis.x,                 zaxis.x,                 0},
                    {xaxis.y,                 yaxis.y,                 zaxis.y,                 0},
                    {xaxis.z,                 yaxis.z,                 zaxis.z,                 0},
                    {-vec3::dot(xaxis, from), -vec3::dot(yaxis, from), -vec3::dot(zaxis, from), 1}};
        }

        static matrix<row, col> orthographic(float width, float height, float near, float far) {
            assert(row == 4 && col == 4);
            return {{2 / width, 0,          0,                   0},
                    {0,         2 / height, 0,                   0},
                    {0,         0,          1 / (far - near),    0},
                    {0,         0,          near / (far - near), 1}};
        }

        static matrix<row, col> perspective(float width, float height, float near, float far, float fov) {
            assert(row == 4 && col == 4);
            float ys = 1 / std::tan(fov / 2);
            float xs = ys * (height / width);
            return {{xs, 0,  0,                            0},
                    {0,  ys, 0,                            0},
                    {0,  0,  far / (far - near),           0},
                    {0,  0,  (-near * far) / (far - near), 1}};
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
                ret[i][j] = lm[i] * rm.get_col(j);
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
