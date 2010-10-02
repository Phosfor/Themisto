#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

// NULL definition
#include <cstddef>

template <typename T>
class ISingleton
{
    private:
        ISingleton(const ISingleton<T> &);
        ISingleton &operator=(const ISingleton<T> &);
        static void destroyInstance()
        {
            if (mInstance != NULL)
                delete mInstance;
            mInstance = 0;
        }

    protected:
        static T *mInstance;

    public:
        ISingleton()
        {
            mInstance = static_cast<T*>(this);
        }

        virtual ~ISingleton()
        {
            if (mInstance != NULL)
                delete mInstance;
            mInstance = 0;
        }

        const static T &get_const_instance()
        {
            if (!mInstance) mInstance = new T();
            return (*mInstance);
        }

        static T &get_mutable_instance()
        {
            if (!mInstance) mInstance = new T();
            return (*mInstance);
        }
};

#endif /* _SINGLETON_HPP_ */
