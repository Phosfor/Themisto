#ifndef _WORLD_OBJECT_HPP_
#define _WORLD_OBJECT_HPP_

class Object
{
    private:
        float x, y;
        std::string mName, mType;

    public:
        virtual std::string getName() { return mName; }
        virtual void setName(const std::string &name) { mName = name; }

        virtual std::string getType() { return mType; }
        virtual void setType(const std::string &type) { mType = type; }

        virtual void update() = 0;
};

#endif /* _WORLD_OBJECT_HPP_ */
