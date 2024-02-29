#include <iostream>
#include <vector>
#include <numeric>
using std::vector;
using std::string;

//(n x m) matrix over field F. immutable.
template<class T>
class Matrix {
    typedef vector<vector<T>> Mat_t;
    typedef vector<T> Row_t;
private:
    const int n; //height. Non-zero.
    const int m; //width. Non-zero.
    const vector<vector<T>> M;
    static Mat_t make_mat (int n, int m,  const T &v) {
        return std::move(vector<vector<T>>(n, vector<T>(m, v)));
    }
    [[nodiscard]] constexpr inline T M_0_0() const{ return M[0][0]; }
public:

    Matrix(int n, int m, const T &v = T()) : n(n), m(m), M(make_mat(n, m, v)) {}

    explicit Matrix(const vector<vector<T>> &M) : n(M.size()), m(M[0].size()), M(std::move(M)) {
        for (auto &row: M) {
            if (row.size() != m) {
                throw std::invalid_argument("Matrix Not Square in Matrix()");
            }
        }
    }

    [[nodiscard]] int get_n() const { return n; }

    [[nodiscard]] int get_m() const { return m; }

    [[nodiscard]] Mat_t to_vector() const{ return M; }

    [[nodiscard]] T get(int i, int j) const { return M[i][j]; }

    [[nodiscard]] Matrix<T> add(const Matrix<T> &B) const{
        if (m != B.m || n != B.n) {
            throw std::invalid_argument("Inconsistent Dimension for Matrix.add()");
        }
        Mat_t C{M};
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                C[i][j] = M[i][j] + B.M[i][j];
            }
        }
        return std::move(Matrix<T>{C});
    }

    [[nodiscard]] Matrix<T> transpose() const {
        Mat_t C{make_mat(m, n, M_0_0())};
        for (int i = 0; i < m; i++){
            for (int j = 0; j < m; j++){
                C[j][i] = M[i][j];
            }
        }
        return std::move(Matrix<T>{C});
    }

    [[nodiscard]] Matrix<T> multiply(const Matrix<T> &B) const{
        if (m != B.n){
            throw std::invalid_argument("Inconsistent Dimension for Matrix.multiply");
        }
        Mat_t C{make_mat(n, B.m, M_0_0())};
        for (int i = 0; i < n; i++){
            for (int j = 0; j < B.m; j++){
                T sum = M[i][0] * B.M[0][j];
                for (int k = 1; k < m; k++){
                    sum += M[i][k] * B.M[k][j];
                }
                C[i][j] = sum;
            }
        }
        return Matrix<T>{C};
    }


    [[nodiscard]] string to_string(std::function<string(T)> formatter) const{
        auto s =std::accumulate(M.begin(), M.end(), string(),
                        [&](const string &acc, const Row_t &r) -> string{
            auto s = std::accumulate(r.begin(), r.end(), string(),
                                     [&](const string &a, const T &b) -> string{
                return a + (a.empty() ? "" : " | ") + formatter(b);
            });
            return acc + (acc.empty() ? "" : "  ]\n[  ") + s;
        });
        return "[  " + s + "  ]";
    }

    friend Matrix<T> operator+ (const Matrix<T> &A, const Matrix<T> &B){
        return A.add(B);
    }

    friend Matrix<T> operator* (const Matrix<T> &A, const Matrix<T> &B){
        return A.multiply(B);
    }

    friend Matrix<T> operator~ (const Matrix<T> &A){
        return A.transpose();
    }
};

int main(){
    Matrix<double> A{2, 2, 1.5};
    Matrix<double> B{2, 2, 2.8};
    auto C = A * B;
    std::cout << C.to_string([](double d){ return std::to_string(d); }) << std::endl;
}