//
// Created by long on 2021/11/15.
//

#ifndef SHEEPRENDER_SR_MATRIX_H
#define SHEEPRENDER_SR_MATRIX_H

namespace SR {
    // 矩阵类
    template<size_t ROW, size_t COL, typename T>
    struct sr_matix {
        sr_vector <COL, T> m[ROW] = {{}};

        inline sr_matix() = default;

        inline sr_vector <COL, T> &operator[](const size_t i) {
            assert(i < ROW);
            return m[i];
        }

        inline const sr_vector <COL, T> &operator[](const size_t i) const {
            assert(i < ROW);
            return m[i];
        }

        // 设置行
        inline void set_row(size_t i, const sr_vector <COL, T> &v) {
            assert(i < COL);
            m[i] = v;
        }

        // 设置列
        inline void set_col(size_t i, const sr_vector <ROW, T> &v) {
            assert(i < ROW);
            for (size_t j = 0; j < ROW; j++) {
                m[j][i] = v[j];
            }
        }

        // 获取行
        inline sr_vector <COL, T> row(size_t i) const {
            assert(i < COL);
            return m[i];
        }

        // 获取列
        inline sr_vector <ROW, T> col(size_t i) const {
            sr_vector<ROW, T> ret;
            for (size_t j = 0; j < ROW; j++) {
                ret[j] = (*this)[j][i];
            }
            return ret;
        }

        // 取得删除某行和某列的子矩阵：子式
        inline sr_matix<ROW - 1, COL - 1, T> get_minor(size_t row, size_t col) const {
            sr_matix<ROW - 1, COL - 1, T> ret;
            for (size_t i = 0; i < ROW - 1; i++) {
                for (size_t j = 0; j < COL - 1; j++) {
                    ret[i][j] = m[i < row ? i : i + 1][j < col ? j : j + 1];
                }
            }
            return ret;
        }

        // 取得转置矩阵
        inline sr_matix<COL, ROW, T> transpose() const {
            sr_matix<COL, ROW, T> ret;
            for (size_t i = 0; i < ROW; i++) {
                for (size_t j = 0; j < COL; j++)
                    ret.m[j][i] = m[i][j];
            }
            return ret;
        }

        // 取得 0 矩阵
        inline static sr_matix<ROW, COL, T> zero() {
            sr_matix<ROW, COL, T> ret;
            for (size_t i = 0; i < ROW; i++) {
                for (size_t j = 0; j < COL; j++)
                    ret[i][j] = 0;
            }
            return ret;
        }

        // 取得单位矩阵
        inline static sr_matix<ROW, COL, T> identity() {
            sr_matix<ROW, COL, T> ret;
            for (size_t i = 0; i < ROW; i++) {
                for (size_t j = 0; j < COL; j++)
                    ret[i][j] = i == j;
            }
            return ret;
        }
    };

    // 输出矩阵
    template<size_t ROW, size_t COL, typename T>
    inline std::ostream &operator<<(std::ostream &out, const sr_matix<ROW, COL, T> &m) {
        for (size_t i = 0; i < ROW; i++) {
            for (size_t j = 0; j < COL - 1; j++) {
                out << m[i][j] << " ";
            }
            out << m[i][COL - 1] << "\n";
        }
        return out;
    }

    // lm + rm
    template<size_t ROW, size_t COL, typename T>
    inline sr_matix<ROW, COL, T> operator+(const sr_matix<ROW, COL, T> &lm, const sr_matix<ROW, COL, T> &rm) {
        sr_matix<ROW, COL, T> ret;
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                ret[i][j] = lm[i][j] + rm[i][j];
        return ret;
    }

    // lm - rm
    template<size_t ROW, size_t COL, typename T>
    inline sr_matix<ROW, COL, T> operator-(const sr_matix<ROW, COL, T> &lm, const sr_matix<ROW, COL, T> &rm) {
        sr_matix<ROW, COL, T> ret;
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                ret[i][j] = lm[i][j] - rm[i][j];
        return ret;
    }

    // m * x
    template<size_t ROW, size_t COL, typename T>
    inline sr_matix<ROW, COL, T> operator*(const sr_matix<ROW, COL, T> &m, const T &x) {
        sr_matix<ROW, COL, T> ret;
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                ret[i][j] = m[i][j] * x;
        return ret;
    }

    // m / x
    template<size_t ROW, size_t COL, typename T>
    inline sr_matix<ROW, COL, T> operator/(const sr_matix<ROW, COL, T> &m, const T &x) {
        sr_matix<ROW, COL, T> ret;
        for (size_t i = 0; i < ROW; i++)
            for (size_t j = 0; j < COL; j++)
                ret[i][j] = m[i][j] * x;
        return ret;
    }

    // lm * rm
    template<size_t ROW1, size_t COL1, size_t COL2, typename T>
    inline sr_matix<ROW1, COL2, T> operator*(const sr_matix<ROW1, COL1, T> &lm, const sr_matix<COL1, COL2, T> &rm) {
        sr_matix<ROW1, COL2, T> ret;
        for (size_t i = 0; i < ROW1; i++)
            for (size_t j = 0; j < COL2; j++)
                ret[i][j] = vec_dot(lm[i], rm.col(j));
        return ret;
    }

    // lv * rm
    template<size_t ROW, size_t COL, typename T>
    inline sr_vector <COL, T> operator*(const sr_vector <ROW, T> &lv, const sr_matix<ROW, COL, T> &rm) {
        sr_vector<COL, T> ret;
        for (size_t j = 0; j < COL; j++) ret[j] = vec_dot(lv, rm.col(j));
        return ret;
    }

    // 三阶向量lv * 四阶矩阵rm
    template<typename T>
    inline sr_vector<3, T> operator*(const sr_vector<3, T> &lv, const sr_matix<4, 4, T> &rm) {
        return (lv.xyz1() * rm).xyz();
    }

    // lm * rv
    template<size_t ROW, size_t COL, typename T>
    inline sr_vector <ROW, T> operator*(const sr_matix<ROW, COL, T> &lm, const sr_vector <COL, T> &rv) {
        sr_vector<ROW, T> ret;
        for (size_t j = 0; j < ROW; j++) ret[j] = vec_dot(lm[j] * rv);
        return ret;
    }

    //// 矩阵函数 ////

    // 一阶行列式求值
    template<typename T>
    inline T mat_det(const sr_matix<1, 1, T> &m) {
        return m[0][0];
    }

    // 二阶行列式求值
    template<typename T>
    inline T mat_det(const sr_matix<2, 2, T> &m) {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }

    // 多阶行列式求值：即第一行同他们的余子式相乘求和
    template<size_t N, typename T>
    inline T mat_det(const sr_matix<N, N, T> &m) {
        T sum = 0;
        for (size_t i = 0; i < N; i++) sum += m[0][i] * mat_cofactor(m, 0, i);
        return sum;
    }

    // 一阶余子式
    template<typename T>
    inline T mat_cofactor(const sr_matix<1, 1, T> &m, size_t row, size_t col) {
        return 0;
    }

    // 多阶余子式：删除特定行列的子式的行列式值
    template<size_t N, typename T>
    inline T mat_cofactor(const sr_matix<N, N, T> &m, size_t row, size_t col) {
        return mat_det(m.get_minor(row, col)) * (((row + col) % 2) ? -1 : 1);
    }

    // 伴随矩阵：即余子式矩阵的转置
    template<size_t N, typename T>
    inline sr_matix<N, N, T> mat_adjoint(const sr_matix<N, N, T> &m) {
        sr_matix<N, N, T> ret;
        for (size_t j = 0; j < N; j++) {
            for (size_t i = 0; i < N; i++) ret[j][i] = mat_cofactor(m, i, j);
        }
        return ret;
    }

    // 求逆矩阵：使用伴随矩阵除以行列式的值得到
    template<size_t N, typename T>
    inline sr_matix<N, N, T> mat_invert(const sr_matix<N, N, T> &m) {
        sr_matix<N, N, T> ret = mat_adjoint(m);
        T det = vec_dot(m[0], ret.col(0));
        return ret / det;
    }

    // 位移矩阵
    template<typename T>
    sr_matix<4, 4, T> mat_translation(sr_vector<3, T> v) {
        sr_matix<4, 4, T> ret = sr_matix<4, 4, T>::identity();
        ret[3][0] = v[0];
        ret[3][1] = v[1];
        ret[3][2] = v[2];
        return ret;
    }

    // 缩放矩阵
    template<typename T>
    sr_matix<4, 4, T> mat_scale(sr_vector<3, T> v) {
        sr_matix<4, 4, T> ret = sr_matix<4, 4, T>::identity();
        ret[0][0] = v[0];
        ret[1][1] = v[1];
        ret[2][2] = v[2];
        return ret;
    }

    // 绕x轴旋转矩阵
    template<typename T>
    sr_matix<4, 4, T> mat_x_rotation(T angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        sr_matix<4, 4, T> ret = sr_matix<4, 4, T>::identity();
        ret[1][1] = c;
        ret[1][2] = -s;
        ret[2][1] = s;
        ret[2][2] = c;
        return ret;
    }

    // 绕y轴旋转矩阵
    template<typename T>
    sr_matix<4, 4, T> mat_y_rotation(T angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        sr_matix<4, 4, T> ret = sr_matix<4, 4, T>::identity();
        ret[0][0] = c;
        ret[0][2] = s;
        ret[2][0] = -s;
        ret[2][2] = c;
        return ret;
    }

    // 绕z轴旋转矩阵
    template<typename T>
    sr_matix<4, 4, T> mat_z_rotation(T angle) {
        float c = std::cos(angle);
        float s = std::sin(angle);
        sr_matix<4, 4, T> ret = sr_matix<4, 4, T>::identity();
        ret[0][0] = c;
        ret[0][1] = -s;
        ret[1][0] = s;
        ret[1][1] = c;
        return ret;
    }

    // 旋转矩阵：先转z轴
    template<typename T>
    sr_matix<4, 4, T> mat_rotation(T x, T y, T z) {
        sr_matix<4, 4, T> ret = sr_matix<4, 4, T>::identity();
        ret = ret * mat_z_rotation(z);
        ret = ret * mat_y_rotation(y);
        ret = ret * mat_x_rotation(x);
        return ret;
    }

    typedef sr_matix<3, 2, float> mat3x2f;
    typedef sr_matix<2, 3, float> mat2x3f;
    typedef sr_matix<3, 3, float> mat3x3f;
    typedef sr_matix<4, 3, float> mat4x3f;
    typedef sr_matix<3, 4, float> mat3x4f;
    typedef sr_matix<4, 4, float> mat4x4f;
}


#endif //SHEEPRENDER_SR_MATRIX_H
