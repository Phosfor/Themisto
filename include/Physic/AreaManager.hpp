#ifndef _AREA_MANAGER_HPP_
#define _AREA_MANAGER_HPP_

#include <boost/serialization/singleton.hpp>
#include <ClanLib/core.h>
#include <Box2D/Box2D.h>

#include "Physic/Body.hpp"

#define areaManager (AreaManager::get_mutable_instance())
#define areaManagerConst (AreaManager::get_const_instance())

class AreaManagerQueryCallback: public b2QueryCallback
{
    public:

        bool ReportFixture (b2Fixture *fixture);
};

struct CellInfo
{
    b2Body* OccupyingBody;
    Impact* WindImpact;
};

struct CellRegion
{
    int lowerX;
    int lowerY;
    int upperX;
    int upperY;
};

class AreaManager : public
boost::serialization::singleton<AreaManager>
{
    private:
        b2World *mWorld;
        float mSellSize;
        CellInfo*** mThirdMatrix;
        CellInfo*** mSecondMatrix;
        CellInfo*** mFirstMatrix;
        b2AABB* mArea;
        float mSecondMatrixBegin;
        float mThirdMatrixBegin;
        int mMatrixWidth;
        int mMatrixHeight;

        CellInfo*** createMatrix(int width, int height);
        void disposeMatrix(CellInfo*** matrix);
        CellInfo*** getMatrix(int x, int y);
        void activeRegionMoved(const b2AABB* newLocation);
        void resetMatrix(CellInfo*** matrix);
        void scanArea(b2AABB* region);

    public:
        CL_Signal_v1<CellRegion> mRegionUpdated;
        CL_Signal_v1<const CellInfo*> mCellReset;

        float getCellSize();
        b2AABB getArea();
        CellRegion getCellRegion();

        void setWindImpact(Impact* impact, int x, int y);
        Impact* getWindImpact(int x, int y);

        b2Body* getCellBody(int x, int y);
        void reportNewBodyLocation(const b2AABB *oldLocation, const b2AABB *newLocation, b2Body* body);

        void init(b2World *world, float sellSize=10);
        ~AreaManager();

};

#endif
