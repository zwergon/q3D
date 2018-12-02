#ifndef Q3D_MODEL_MATRIX_H
#define Q3D_MODEL_MATRIX_H

#include <q3D/model/model_scope.h>

#include <vector>

namespace Q3D {

template<class T>
class MODELSHARED_EXPORT Matrix
{
public:
    Matrix(size_t rows, size_t cols)
        : rows_(rows),
          cols_(cols),
          data_(rows * cols)
    {
    }

    void setSize( size_t rows, size_t cols ){
        rows_ = rows;
        cols_ = cols;
    }

    size_t getRows() const {
        return rows_;
    }

    size_t getCols() const  {
        return cols_;
    }

    double& operator()(size_t i, size_t j){
        return data_[i * cols_ + j];
    }

    double operator()(size_t i, size_t j) const{
        return data_[i * cols_ + j];
    }

    std::vector<T>& data() {
        return data_;
    }

private:
    size_t rows_;
    size_t cols_;
    std::vector<T> data_;
};


class Matrix3d : public Matrix<double> {
public:
    Matrix3d() : Matrix<double>(3, 3) {}

    double det(){
        return (*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(2, 1) * (*this)(1, 2)) -
               (*this)(0, 1) * ((*this)(1, 0) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 0)) +
               (*this)(0, 2) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0));

    }
};

}


#endif // Q3D_MODEL_MATRIX_H
