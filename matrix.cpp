#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
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
    [[nodiscard]] constexpr T M_0_0() const{ return M[0][0]; }

    [[nodiscard]] constexpr T zero() const{ return M[0][0] - M[0][0]; }

    [[nodiscard]] constexpr T one() const{ return M[0][0] / M[0][0]; }

    [[nodiscard]] Matrix<T> remove_row(int i) const{
        Mat_t C{M} ;
        C.erase(C.begin() + i);
        return Matrix<T>(C);
    }

    [[nodiscard]] Matrix<T> I(int l) const{
        Mat_t C{make_mat(l, l, zero())};
        for (int i = 0; i < l; i++){
            C[i][i] = one();
        }
        return Matrix<T>(C);
    }

    static Row_t subtract(const Row_t &r1, const T &factor, const Row_t &r2){
        Row_t r3{r1};
        for (int i = 0; i < r3.size(); i++){
            r3[i] = r3[i] - factor * r2[i];
        }
        return r3;
    }

    static Row_t scale(const Row_t &r1, const T &c){
        Row_t r3{r1};
        for (int i = 0; i < r3.size(); i++){
            r3[i] = r3[i] * c;
        }
        return r3;
    }

    static Mat_t swap(const Mat_t &A, int i, int j){
        Mat_t C{A};
        std::iter_swap(C.begin() + i, C.begin() + j);
        return C;
    }
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
                C[i][j] = M[i][j] + B[i][j];
            }
        }
        return std::move(Matrix<T>{C});
    }

    [[nodiscard]] Matrix<T> transpose() const {
        Mat_t C{make_mat(m, n, M_0_0())};
        for (int i = 0; i < n; i++){
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
                T sum = M[i][0] * B[0][j];
                for (int k = 1; k < m; k++){
                    sum += M[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
        return Matrix<T>{C};
    }

    [[nodiscard]] const T &det() const{
        if (n != m){
            throw std::invalid_argument("Inconsistent Dimension for Matrix.det()");
        }
        if (n == 1 && m == 1){
            return M_0_0();
        }
        auto C = remove_row(0).transpose();
        T sum = M[0][0] * C.remove_row(0).transpose().det();
        for (int i = 1; i < m; i++){
            T v = M[0][i] * C.remove_row(i).det();
            if (i % 2 == 1){
                sum = sum - v;
            }
            else{
                sum = sum + v;
            }
        }
        return sum;
    }

    [[nodiscard]] Matrix<T> rref() const{
        Mat_t C{M};
        int rank = 0;
        for (int i = 0; i < (m < n ? m : n); i++){
            //outer loop: eliminate 1 at nth column
            bool flag = false;
            int id;
            for (int j = i; j < n; j++){
                //find first nonzero row
                if (C[j][i] != zero()){
                    id = j;
                    flag = true;
                    break;
                }
            }
            if (flag){
                //if found, swap, scale and subtract all other rows
                C = swap(C, id, rank);
                C[i] = scale(C[rank], C[rank][i]);
                rank++;
                for (int k = 0; k < n; k++){
                    if (k == rank){
                        continue;
                    }
                    C[k] = subtract(C[k], C[k][i], C[rank]);
                }
            }
        }
        return Matrix<T>{C};
    }

    [[nodiscard]] Matrix<T> concat_right(const Matrix<T>& B) const{
        if (n != B.n){
            throw std::invalid_argument("Inconsistent Dimension for Matrix.concat_right()");
        }
        Mat_t C{make_mat(n, m + B.m, one())};
        for (int i = 0; i < n; i++){
            for (int j = 0; j < m; j++){
                C[i][j] = M[i][j];
                C[i][j + m] = B[i][j];
            }
        }
        return Matrix<T>{C};
    }

    [[nodiscard]] Matrix<T> inverse() const{
        if (n != m){
            throw std::invalid_argument("Matrix Not Square");
        }
        auto I_n = I(n);
        auto C = concat_right(I_n).rref();
        Mat_t A_inv{make_mat(n, n, zero())};
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (C[i][j] == I_n[i][j]){
                    throw std::invalid_argument("Matrix Not Invertible");
                }
                A_inv[i][j] = C[i][j + n];
            }
        }
        return Matrix<T>{A_inv};
    }

    [[nodiscard]] string to_string(std::function<string(T)> formatter) const{
        auto s = std::accumulate(M.begin(), M.end(), string(),
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

    friend Matrix<T> operator! (const Matrix<T> &A){
        return A.inverse();
    }

    const Row_t& operator[] (int i) const{
        return M[i];
    }
};

int main(){
    Matrix<double> A{vector<vector<double>>{vector<double>{1, 2, 3}, vector<double>{4, 5, 6}, vector<double>{7, 8, 9}}};
    Matrix<double> B{2, 2, 2.8};
    std::cout << A.det() << std::endl;
    auto C = !A;
    std::cout << C.to_string([](double d){ return std::to_string(d); }) << std::endl;
}