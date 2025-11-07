#include <cassert>
#include <stdexcept>
#include <vector>
#include <iostream>



template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;
private:
    T * data;
    size_type y_size, x_size;

    Grid(T *data, size_type y_size, size_type x_size):
    data(data), y_size(y_size), x_size(x_size) {}


    //Задание 2, класс строки для записи
    class Row {
    private:
        T * row_data;
        size_type row_size;

    public:
        Row(T * data, size_type size) :
            row_data{data}, row_size{size} {}

        T& operator[](size_type x_idx) {
            if (x_idx >= row_size)
                throw std::out_of_range("index out of range");
            return row_data[x_idx];
        }

        const T& operator[](size_type x_idx) const {
            if (x_idx >= row_size)
                throw std::out_of_range("index out of range");
            return row_data[x_idx];
        }
    };

    //Задание 2, класс строки для чтения
    class ConstRow {
    private:
        T * row_data;
        size_type row_size;

    public:
        ConstRow(T * data, size_type size) :
            row_data{data}, row_size{size} {}

        const T& operator[](size_type x_idx) const {
            if (x_idx >= row_size)
                throw std::out_of_range("index out of range");
            return row_data[x_idx];
        }
    };



public:
    Grid<T>& operator=(T const &t) {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it)
            *it = t;
        return *this;
    }   


    //Задание 1, пункт 1 (неявное преобразование)
    Grid(T const &t) : data{new T[1]}, y_size{1}, x_size{1} {
        *data = t;
    }

    //Задание 1, пункт 2 (конструктор с размером)
    Grid(size_type y_size,size_type x_size) : data{new T[x_size * y_size]}, y_size{y_size}, x_size{x_size} {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it)
            *it = T();
    }

    //Задание 1, пункт 3 (конструктор с размером и значением)
    Grid(size_type y_size,size_type x_size, const T& t) : data{new T[x_size * y_size]}, y_size{y_size}, x_size{x_size} {
        for (auto it = data, end = data + x_size * y_size; it != end; ++it)
            *it = t;
    }

    //Задание 1, конструктор копирования
    Grid(const Grid<T>& other) :
        data{new T[other.y_size * other.x_size]},
        y_size{other.y_size},
        x_size{other.x_size} {
        for (size_type i = 0; i < y_size * x_size; ++i)
            data[i] = other.data[i]; 
        }

    //Задание 1, конструктор перемещения
    Grid(Grid<T>&& other) noexcept : 
        data{other.data},
        y_size{other.y_size},
        x_size{other.x_size} {
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }

    //Задание 1, оператор присваивания копированием
    Grid<T>& operator=(const Grid<T>& other) {
        if (&other == this) {
            return *this;
        }
        Grid<T> tmp{other};
        std::swap(data, tmp.data);
        std::swap(y_size, tmp.y_size);
        std::swap(x_size, tmp.x_size);
        return *this;
    }

    //Задание 1, оператор присваивания перемещением
    Grid<T>& operator=(Grid<T>&& other) noexcept {
        if (&other == this) {
            return *this;
        }
        delete[] data;
        y_size = other.y_size;
        x_size = other.x_size;
        data = other.data;
        other.y_size = 0;
        other.x_size = 0;
        other.data = nullptr;
        return *this;
    }

    //Задание 1, деструктор
    ~Grid() {
        delete[] data;
    }

    



    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }

    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }

    //Задание 2, оператор индексации [][]
    //для записи
    Row operator[](size_type y_idx) {
        if (y_idx >= y_size) {
            throw std::out_of_range("index out of range");
        }
        return Row(data + y_idx * x_size, x_size);
    }
    //для чтения
    ConstRow operator[](size_type y_idx) const {
        if (y_idx >= y_size) {
            throw std::out_of_range("index out of range");
        }
        return ConstRow(data + y_idx * x_size, x_size);
    }



    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }
};








//Задание 3
//базовый случай: сетка размерности 1
template<typename T, size_t Dimension>
class MultiGrid;

template<typename T>
class MultiGrid<T, 1> {
public:
    using value_type = T;
    using size_type = size_t;

private:
    T* data;
    size_type x_size;

public:
    //конструкторы
    
    //конструктор по умолчанию
    MultiGrid() : data(nullptr), x_size(0) {}

    //неявное преобразование
    MultiGrid(T const &t) : data{new T[1]}, x_size{1} {
        *data = t;
    }
    
    //конструктор с размером
    MultiGrid(size_type size) :
        data{new T[size]}, x_size{size} {
            for (size_type i = 0; i < x_size; ++i)
                data[i] = T();
        }

    //конструктор с размером и значением
    MultiGrid(size_type size, const T& t) :
        data{new T[size]}, x_size{size} {
            for (size_type i = 0; i < x_size; ++i)
                data[i] = t;
        }
    
    //конструктор копирования
    MultiGrid(const MultiGrid<T, 1>& other) :
        data{new T[other.x_size]}, x_size{other.x_size} {
            for (size_type i = 0; i < x_size; ++i)
                data[i] = other.data[i];
        }
    
    //конструктор перемещения
    MultiGrid(MultiGrid<T, 1>&& other) noexcept :
    data(other.data), x_size(other.x_size) {
        other.data = nullptr;
        other.x_size = 0;
    }
    
    //оператор присваивания копированием
    MultiGrid<T, 1>& operator=(const MultiGrid<T, 1>& other) {
        if (&other == this) {
            return *this;
        }
        MultiGrid<T, 1> tmp = other;
        std::swap(data, tmp.data);
        std::swap(x_size, tmp.x_size);
        return *this;
    }
    
    //оператор присваивания перемещением
    MultiGrid& operator=(MultiGrid&& other) noexcept {
        if (&other == this) {
            return *this;
        }
        delete[] data;
        x_size = other.x_size;
        data = other.data;
        other.x_size = 0;
        other.data = nullptr;
        return *this;
    }
    
    //деструктор
    ~MultiGrid() {
        delete[] data;
    }
    


    //индексация
    
    //оператор круглых скобок
    const T& operator()(size_type x_idx) const {
        if (x_idx >= x_size) throw std::out_of_range("Index out of range");
        return data[x_idx];
    }
    
    T& operator()(size_type x_idx) {
        if (x_idx >= x_size) throw std::out_of_range("Index out of range");
        return data[x_idx];
    }
    
    //оператор квадратных скобок (для одномерного - то же, что и круглые)
    const T& operator[](size_type x_idx) const {
        if (x_idx >= x_size) throw std::out_of_range("Index out of range");
        return data[x_idx];
    }
    
    T& operator[](size_type x_idx) {
        if (x_idx >= x_size) throw std::out_of_range("Index out of range");
        return data[x_idx];
    }
};



//общий случай: сетка размерности > 1
template<typename T, size_t Dimension>
class MultiGrid {
public:
    using value_type = T;
    using size_type = size_t;

private:
    MultiGrid<T, Dimension - 1>* data;
    size_type first_size;

public:
    //конструкторы
    
    //конструктор по умолчанию
    MultiGrid() : data{nullptr}, first_size{0} {}
    
    //конструктор с размерами
    template<typename... Args>
    MultiGrid(size_type first_dim, Args... other_dims) :
        data{new MultiGrid<T, Dimension - 1>[first_dim]}, first_size{first_dim} {
            for (size_type i = 0; i < first_size; ++i) {
                data[i] = MultiGrid<T, Dimension - 1>(other_dims...);
            }
        }
    
    //конструктор с размерами и значением
    template<typename... Args>
    MultiGrid(size_type first_dim, Args... other_dims, const T& t) :
        data{new MultiGrid<T, Dimension - 1>[first_dim]}, first_size{first_dim} {
        for (size_type i = 0; i < first_size; ++i) {
            data[i] = MultiGrid<T, Dimension - 1>(other_dims..., t);
        }
    }
    
    //конструктор копирования
    MultiGrid(const MultiGrid<T, Dimension>& other) :
        data{new MultiGrid<T, Dimension - 1>[other.first_size]}, 
        first_size{other.first_size} {
        for (size_type i = 0; i < first_size; ++i)
            data[i] = other.data[i];
    }
    
    //конструктор перемещения
    MultiGrid(MultiGrid<T, Dimension>&& other) noexcept :
    data{other.data}, first_size{other.first_size} {
        other.data = nullptr;
        other.first_size = 0;
    }
    
    //оператор присваивания копированием
    MultiGrid<T, Dimension>& operator=(const MultiGrid<T, Dimension>& other) {
        if (&other == this) {
            return *this;
        }
        MultiGrid<T, Dimension> tmp{other};
        std::swap(data, tmp.data);
        std::swap(first_size, tmp.first_size);
        return *this;
    }
    
    //оператор присваивания перемещением
    MultiGrid<T, Dimension>& operator=(MultiGrid<T, Dimension>&& other) noexcept {
        if (&other == this) {
            return *this;
        }
        delete[] data;
        first_size = other.first_size;
        data = other.data;
        other.first_size = 0;
        other.data = nullptr;
        return *this;
    }
    
    //деструктор
    ~MultiGrid() {
        delete[] data;
    }
    

    //индексация
    
    //оператор квадратных скобок
    MultiGrid<T, Dimension - 1>& operator[](size_type first_idx) {
        if (first_idx >= first_size)
            throw std::out_of_range("Index out of range");
        return data[first_idx];
    }
    const MultiGrid<T, Dimension - 1>& operator[](size_type first_idx) const {
        if (first_idx >= first_size)
            throw std::out_of_range("Index out of range");
        return data[first_idx];
    }

    //оператор круглых скобок
    template<typename... Indices>
    const T& operator()(size_type first, Indices... rest) const {
        if (first >= first_size) throw std::out_of_range("Index out of range");
        return data[first](rest...);
    }
    
    template<typename... Indices>
    T& operator()(size_type first, Indices... rest) {
        if (first >= first_size) throw std::out_of_range("Index out of range");
        return data[first](rest...);
    }
};



int main() {
    Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());

    using gsize_t = Grid<float>::size_type;

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
    for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
    assert(0.0f == g[y_idx][x_idx]);

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
    for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
    g[y_idx][x_idx] = 1.0f;

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
    for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
    assert(1.0f == g(y_idx, x_idx));





    MultiGrid<float,3> const g3(2, 3, 4, 1.0f);
    assert(1.0f == g3(1, 1, 1));

    MultiGrid<float,2> g2(2, 5, 2.0f);
    assert(2.0f == g2(1, 1));

    g2 = g3[1];
    assert(1.0f == g2(1, 1));
}





