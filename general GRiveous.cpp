#include <iostream>

template<typename Derived>
class Shape {
public:
    void draw() {
        static_cast<Derived*>(this)->drawImpl();
    }
    
    double area() {
        return static_cast<Derived*>(this)->areaImpl();
    }
};

class Circle : public Shape<Circle> {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    void drawImpl() {
        std::cout << "Drawing a circle with radius " << radius << std::endl;
    }
    
    double areaImpl() {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape<Rectangle> {
private:
    double width, height;
    
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    
    void drawImpl() {
        std::cout << "Drawing a rectangle " << width << "x" << height << std::endl;
    }
    
    double areaImpl() {
        return width * height;
    }
};

int main() {
    Circle c(5);
    Rectangle r(4, 6);
    
    c.draw();
    std::cout << "Area: " << c.area() << std::endl;
    
    r.draw();
    std::cout << "Area: " << r.area() << std::endl;
    
    return 0;
}
