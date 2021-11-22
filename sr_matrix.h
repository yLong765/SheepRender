//
// Created by long on 2021/11/15.
//

#ifndef SHEEPRENDER_SR_MATRIX_H
#define SHEEPRENDER_SR_MATRIX_H

namespace SR {
    template<size_t ROW, size_t COL, typename T>
    struct matrix {
        sr_vector <COL, T> m[ROW] = {{}};

        inline matrix() = default;

        inline sr_vector <COL, T> &operator[](const size_t i) {
            assert(i < ROW);
            return m[i];
        }

        inline const sr_vector <COL, T> &operator[](const size_t i) const {
            assert(i < ROW);
            return m[i];
        }

        inline void set_row(size_t i, const sr_vector <COL, T> &v) {
            assert(i < COL);
            m[i] = v;
        }

        inline void set_col(size_t i, const sr_vector <ROW, T> &v) {
            assert(i < ROW);
            for (size_t j = 0; j < ROW; j++) {
                m[j][i] = v[j];
            }
        }

        inline sr_vector <COL, T> row(size_t i) const {
            assert(i < COL);
            return m[i];
        }

        inline sr_vector <ROW, T> col(size_t i) const {
            sr_vector<ROW, T> ret;
            for (size_t j = 0; j < ROW; j++) {
                ret[j] = (*this)[j][i];
            }
            return ret;
        }

        inline matrix<ROW, COL, T> inverse() const {

        }

        inline matrix<COL, ROW, T> transpose() const {
            matrix<COL, ROW, T> ret;
            for (size_t i = 0; i < ROW; i++)
                for (size_t j = 0; j < COL; j++)
                    ret[j][i] = (*this)[i][j];
            return ret;
        }

        static matrix<ROW, COL, T> identity() {
            matrix<ROW, COL, T> ret;
            for (size_t i = 0; i < ROW; i++) {
                for (size_t j = 0; j < COL; j++) {
                    ret[i][j] = i == j;
                }
            }
            return ret;
        }

        static matrix<ROW, COL, T> zero() {
            matrix<ROW, COL, T> ret;
            for (size_t i = 0; i < ROW; i++) {
                for (size_t j = 0; j < COL; j++) {
                    ret[i][j] = 0;
                }
            }
            return ret;
        }

        static matrix<ROW, COL, T> get_translation(vec3f val) {
            assert(ROW == 4 && COL == 4);
            matrix<4, 4, T> ret = identity();
            ret[3][0] = val[0];
            ret[3][1] = val[1];
            ret[3][2] = val[2];
            return ret;
        }

        static matrix<ROW, COL, T> get_scale(vec3f val) {
            assert(ROW == 4 && COL == 4);
            matrix<4, 4, T> ret = identity();
            ret[0][0] = val[0];
            ret[1][1] = val[1];
            ret[2][2] = val[2];
            return ret;
        }

        static matrix<ROW, COL, T> get_x_rotation(float angle) {
            assert(ROW == 4 && COL == 4);
            float c = std::cos(angle);
            float s = std::sin(angle);
            matrix<4, 4, T> ret = identity();
            ret[1][1] = c;
            ret[1][2] = -s;
            ret[2][1] = s;
            ret[2][2] = c;
            return ret;
        }

        static matrix<ROW, COL, T> get_y_rotation(float angle) {
            assert(ROW == 4 && COL == 4);
            float c = std::cos(angle);
            float s = std::sin(angle);
            matrix<4, 4, T> ret = identity();
            ret[0][0] = c;
            ret[0][2] = s;
            ret[2][0] = -s;
            ret[2][2] = c;
            return ret;
        }

        static matrix<ROW, COL, T> get_z_rotation(float angle) {
            assert(ROW == 4 && COL == 4);
            float c = std::cos(angle);
            float s = std::sin(angle);
            matrix<4, 4, T> ret = identity();
            ret[0][0] = c;
            ret[0][1] = -s;
            ret[1][0] = s;
            ret[1][1] = c;
            return ret;
        }

        static matrix<ROW, COL, T> get_rotation(float x, float y, float z) {
            assert(ROW == 4 && COL == 4);
            matrix<4, 4, T> ret = identity();
            ret = ret * get_z_rotation(z);
            ret = ret * get_y_rotation(y);
            ret = ret * get_x_rotation(x);
            return ret;
        }
    };

    template<size_t ROW, size_t COL, typename T>
    inline std::ostream &operator<<(std::ostream &out, const matrix<ROW, COL, T> &m) {
        for (size_t i = 0; i < ROW; i++) {
            for (size_t j = 0; j < COL - 1; j++) {
                out << m[i][j] << " ";
            }
            out << m[i][COL - 1] << "\n";
        }
        return out;
    }

    template<size_t ROW, size_t COL, typename T>
    inline matrix<ROW, COL, T> operator+(const matrix<ROW, COL, T> &lm, const matrix<ROW, COL, T> &rm) {
        matrix<ROW, COL, T> ret;
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                ret[i][j] = lm[i][j] + rm[i][j];
        return ret;
    }

    template<size_t ROW, size_t COL, typename T>
    inline matrix<ROW, COL, T> operator-(const matrix<ROW, COL, T> &lm, const matrix<ROW, COL, T> &rm) {
        matrix<ROW, COL, T> ret;
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                ret[i][j] = lm[i][j] - rm[i][j];
        return ret;
    }

    template<size_t ROW, size_t COL, typename T>
    inline matrix<ROW, COL, T> operator*(const matrix<ROW, COL, T> &m, const float &val) {
        matrix<ROW, COL, T> ret;
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                ret[i][j] = m[i][j] * val;
        return ret;
    }

    template<size_t ROW1, size_t COL1, size_t COL2, typename T>
    inline matrix<ROW1, COL2, T> operator*(const matrix<ROW1, COL1, T> &lm, const matrix<COL1, COL2, T> &rm) {
        matrix<ROW1, COL2, T> ret;
        for (size_t i = 0; i < ROW1; i++)
            for (size_t j = 0; j < COL2; j++)
                ret[i][j] = vec_dot(lm[i], rm.col(j));
        return ret;
    }

    template<size_t ROW, size_t COL, typename T>
    inline sr_vector <COL, T> operator*(const sr_vector <ROW, T> &lv, const matrix<ROW, COL, T> &rm) {
        sr_vector<COL, T> ret;
        for (size_t j = 0; j < COL; j++) ret[j] = vec_dot(lv, rm.col(j));
        return ret;
    }

    template<size_t ROW, size_t COL, typename T>
    inline sr_vector <ROW, T> operator*(const matrix<ROW, COL, T> &lm, const sr_vector <COL, T> &rv) {
        sr_vector<ROW, T> ret;
        for (size_t j = 0; j < ROW; j++) ret[j] = vec_dot(lm[j] * rv);
        return ret;
    }

    template<size_t ROW, size_t COL, typename T>
    matrix<ROW, COL, T> operator/(const matrix<ROW, COL, T> &m, const float &x) {
        matrix<ROW, COL, T> ret;
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                ret[i][j] = m[i][j] / x;
        return ret;
    }

    typedef matrix<3, 3, float> mat3x3f;
    typedef matrix<4, 3, float> mat4x3f;
    typedef matrix<3, 4, float> mat3x4f;
    typedef matrix<4, 4, float> mat4x4f;
}


#endif //SHEEPRENDER_SR_MATRIX_H
