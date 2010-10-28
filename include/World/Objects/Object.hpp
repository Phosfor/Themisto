#ifndef _WORLD_OBJECT_HPP_
#define _WORLD_OBJECT_HPP_

class Object
{
    private:
        float x, y;

    public:
        virtual void update() = 0;
};

#endif /* _WORLD_OBJECT_HPP_ */
