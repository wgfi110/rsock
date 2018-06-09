//
// Created by System Administrator on 6/1/18.
//

#include <plog/Log.h>

template<typename T>
Singleton<T>::Singleton() {
    static_assert(std::is_base_of<ICloseable, T>::value, "Template parameter must implement ICloseable");
}

template<typename T>
Singleton<T>::~Singleton() {
}

template<typename T>
T *Singleton<T>::GetInstance() {
    if (!sInstance) {
        std::lock_guard<std::mutex> lk(sMutex);
        if (!sInstance) {
            if (sDestroyed) {
                LOGE << "already destroyed";
                assert(0);
            }
            sInstance = new T();
        }
    }
    return sInstance;
}

template<typename T>
int Singleton<T>::DestroyInstance() {
    int nret = 0;
    if (sInstance) {
        std::lock_guard<std::mutex> lk(sMutex);
        if (sInstance) {
            sDestroyed = true;
            auto *closeable = dynamic_cast<ICloseable *>(sInstance);
            nret = closeable->Close();
            delete sInstance;
            sInstance = nullptr;
        }
    }
    return nret;
}
