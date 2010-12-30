/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _AREA_MANAGER_HPP_
#define _AREA_MANAGER_HPP_

#include <boost/serialization/singleton.hpp>
#include <ClanLib/core.h>
#include <Box2D/Box2D.h>

#include "Physic/Body.hpp"
#include "Core/Utils.hpp"

class Impact;

class AreaManagerQueryCallback: public b2QueryCallback
{
    public:
        bool ReportFixture (b2Fixture *fixture);
};

struct CellInfo
{
    b2Fixture* OccupyingFixture;
    Impact* WindImpact;
};

struct CellRegion
{
    int lowerX;
    int lowerY;
    int upperX;
    int upperY;
};

class AreaManager : public boost::serialization::singleton<AreaManager>
{
    private:
        b2World *mWorld;
        float mCellSize;
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
        CellInfo*** getMatrix(int& x, int& y);
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

        b2Fixture* getCellFixture(int x, int y);
        void reportNewFixtureLocation(const b2AABB *oldLocation,
                const b2AABB *newLocation, b2Fixture* body);

        void init(b2World *world, float sellSize=10);
        ~AreaManager();
};

inline AreaManager &areaManager() { return AreaManager::get_mutable_instance(); }

#endif /* _AREA_MANAGER_HPP_ */
