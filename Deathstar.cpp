#include <iostream>
#include <vector>
#include <cassert>

template<typename E>
class VectorExpression {
public:
    double operator[](size_t i) const {
        return static_cast<const E&>(*this)[i];
    }
    
    size_t size() const {
        return static_cast<const E&>(*this).size();
    }
};

class Vector : public VectorExpression<Vector> {
private:
    std::vector<double> data;
    
public:
    Vector(size_t n) : data(n) {}
    
    template<typename E>
    Vector(const VectorExpression<E>& expr) : data(expr.size()) {
        for(size_t i = 0; i < expr.size(); ++i)
            data[i] = expr[i];
    }
    
    double operator[](size_t i) const { return data[i]; }
    double& operator[](size_t i) { return data[i]; }
    size_t size() const { return data.size(); }
    
    template<typename E>
    Vector& operator=(const VectorExpression<E>& expr) {
        assert(size() == expr.size());
        for(size_t i = 0; i < expr.size(); ++i)
            data[i] = expr[i];
        return *this;
    }
};

template<typename E1, typename E2>
class VectorSum : public VectorExpression<VectorSum<E1, E2>> {
private:
    const E1& u;
    const E2& v;
    
public:
    VectorSum(const E1& u, const E2& v) : u(u), v(v) {
        assert(u.size() == v.size());
    }
    
    double operator[](size_t i) const { return u[i] + v[i]; }
    size_t size() const { return u.size(); }
};

template<typename E1, typename E2>
VectorSum<E1, E2> operator+(const VectorExpression<E1>& u, const VectorExpression<E2>& v) {
    return VectorSum<E1, E2>(static_cast<const E1&>(u), static_cast<const E2&>(v));
}

int main() {
    Vector v1(3), v2(3), v3(3);
    
    for(int i = 0; i < 3; ++i) {
        v1[i] = i;
        v2[i] = i * 2;
    }
    
    v3 = v1 + v2 + v1;  // No temporary vectors created!
    
    for(int i = 0; i < 3; ++i)
        std::cout << v3[i] << " ";
    std::cout << std::endl;
    
    return 0;
}
