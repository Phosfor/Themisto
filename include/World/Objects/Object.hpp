#ifndef _WORLD_OBJECT_HPP_
#define _WORLD_OBJECT_HPP_

class Object
{
    private:
        float x, y;
        std::string mName;

    public:
        virtual std::string getName() { return mName; }
        virtual void setName(const std::string &name) { mName = name; }
        virtual void update() = 0;
};

#endif /* _WORLD_OBJECT_HPP_ */
