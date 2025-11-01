#include <iostream>


template <typename T>
class shared_ptr {
private:
    struct ControlBlock {
        T* ptr;
        size_t shared_count;
        size_t weak_count;
        ControlBlock(T* p) : ptr(p), shared_count(1), weak_count(0) {}
        
        ~ControlBlock() {
            delete ptr;
        }
        
        void add_shared() {
            if (this)
                ++shared_count;
        }
        
        void add_weak() {
            if (this)
                ++weak_count;
        }
        
        void release_shared() {
            //если нет сильных ссылок, уничтожаем объект
            if (this && ptr && --shared_count == 0) {
                delete ptr;
                ptr = nullptr;
                //если нет слабых ссылок, удаляем control block
                if (weak_count == 0) {
                    delete this;
                }
            }
        }    

        void release_weak() {
            if (this && --weak_count == 0 && shared_count == 0) {
                delete this;
            }
        }
    };

    ControlBlock* cb = nullptr;

    
    shared_ptr(ControlBlock* control_block):
        cb(control_block) {
            cb->add_shared();
        }
public:
    //дефолтный конструктор
    shared_ptr() = default;

    //конструктор из сырого указателя
    shared_ptr(T* ptr):
        cb(ptr ? new ControlBlock(ptr) : nullptr) {}

    //конструктор копирования
    shared_ptr(const shared_ptr& other):
        cb(other.cb) {
                cb->add_shared();
        }

    //конструктор перемещения
    shared_ptr(shared_ptr&& other):
        cb(other.cb) {
            other.cb = nullptr;
        }

    
    //оператор присваивания копированием
    simple_shared_ptr& operator=(const simple_shared_ptr& other) {
        cb->release_shared();
        cb = other.cb;
        cb->add_shared();
        
        return *this;
    }
    
    //оператор присваивания перемещением
    simple_shared_ptr& operator=(simple_shared_ptr&& other) noexcept {
        cb->release_shared();
        cb = other.cb;
        other.cb = nullptr;
        return *this;
    }
   


    //деструктор
    ~shared_ptr() {
        cb->release_shared();
    }


    T& operator*() const {
        return *cb->ptr;
    }

    T* operator->() {
        return cb->ptr;
    }



    template<typename U>
    friend class simple_weak_ptr;
    
    //для доступа к приватному конструктору
    template<typename U, typename... Args>
    friend simple_shared_ptr<U> make_simple_shared(Args&&... args);

};



template <typename T>
class weak_ptr {
private:
    typename shared_ptr<T>::ControlBlock* cb = nullptr;
public:
    weak_ptr() = default;

    weak_ptr(const weak_ptr& other):
        cb(other.cb) {
            cb->add_weak();
        }

    weak_ptr(const shared_ptr<T>& shared):
        cb(shared.cb) {
            cb->add_weak();
        }
    
    
    simple_weak_ptr& operator=(const simple_weak_ptr& other) {
        if (this != &other) {
            cb->release_weak();
            cb = other.cb;
            cb->add_weak();
        }
        return *this;
    }
    
    simple_weak_ptr& operator=(const simple_shared_ptr<T>& shared) {
        cb->release_weak();
        cb = shared.cb;
        cb->add_weak();
        return *this;
    }
    

    ~weak_ptr() {
        cb->release_weak();
        cb = nullptr;
    }

    
    boo expired() const {
        return !cb || cb->shared_count == 0;
    }

    
    shared_ptr<T> lock() const {
        if (cb && cb->shared_count > 0) {
            //используем приватный конструктор из shared
            return shared_ptr<T>(cb);
        }
        return shared_ptr<T>();
    }
};



template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    auto ptr = new ControlBlock<T>{1, T{std::forward<Args>(args)...}};
    return shared_ptr<T>(shared_ptr::make_shared_t(), ptr);
    
}



//для одиночных объектов
template<typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
    auto ptr = new T(std::forward<Args>(args)...);
    auto cb = new typename shared_ptr<T>::ControlBlock(ptr);
    return shared_ptr<T>(cb);
}

//для массивов
template<typename T>
shared_ptr<T[]> make_shared(size_t size) {
    auto ptr = new T[size];
    auto cb = new typename shared_ptr<T[]>::ControlBlock(ptr);
    return shared_ptr<T[]>(cb);
}

