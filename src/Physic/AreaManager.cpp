#include "Physic/AreaManager.hpp"



void AreaManager::init(b2World *world, float cellSize)
{
    
    mCellSize = cellSize;
    mWorld = world;
    float areaWidth = ScreenResolutionX;
    float areaHeight = ScreenResolutionY;
    mArea = new b2AABB;
    mArea->lowerBound.Set(0, 0);
    mArea->upperBound.Set(0,0);
    mSecondMatrixBegin = 0;
    mThirdMatrixBegin = 0;

    mMatrixWidth = floor(areaWidth / mCellSize + 0.5);
    mMatrixHeight = floor(areaHeight / mCellSize + 0.5);

    mSecondMatrix = createMatrix(mMatrixWidth, mMatrixHeight);
    mThirdMatrix  = createMatrix(mMatrixWidth, mMatrixHeight);
    mFirstMatrix = createMatrix(mMatrixWidth, mMatrixHeight);

    scanArea(mArea);

    mRegionUpdated.invoke(getCellRegion());
}

CellRegion AreaManager::getCellRegion()
{
    CellRegion allRegion;
    allRegion.lowerX = 0;
    allRegion.lowerY = 0;
    allRegion.upperX = 3 * mMatrixWidth;
    allRegion.upperY = mMatrixHeight;
    return allRegion;
}

b2AABB AreaManager::getArea()
{
    return *mArea;
}

CellInfo*** AreaManager::createMatrix(int width, int height)
{
    CellInfo*** matrix;
    matrix = new CellInfo**[width];
    for(int i=0; i<width; i++)
    {
        matrix[i] = new CellInfo*[mMatrixHeight];
        for(int j=0; j<height; j++)
        {
            matrix[i][j] = NULL;
        }
    }
    return matrix;
}

void AreaManager::disposeMatrix(CellInfo*** matrix)
{
    for(int i=0; i<mMatrixWidth; i++)
    {
        for(int j=0; j<mMatrixHeight; j++)
        {
            if(matrix[i][j] != NULL)
            {
                delete matrix[i][j];
            }
        }
        delete matrix[i];
    }
    delete matrix;
}

AreaManager::~AreaManager()
{
    disposeMatrix(mSecondMatrix);
    disposeMatrix(mThirdMatrix);
    disposeMatrix(mFirstMatrix);
    delete mArea;
    //delete mRegionUpdated;
}

 CellInfo*** AreaManager::getMatrix(int& x, int& y)
 {
     CellInfo*** matrix = NULL;
     if(mArea->lowerBound.x < x && x < mArea->upperBound.x &&
        mArea->lowerBound.y < y && y < mArea->upperBound.y)
     {
         if(x < mSecondMatrixBegin)
         {
             matrix = mFirstMatrix;
         }
         else if(x < mThirdMatrixBegin)
         {
             matrix = mSecondMatrix;
             x -=mSecondMatrixBegin;
         }
         else  
         {
             matrix = mThirdMatrix;
             x -= mThirdMatrixBegin;
         }
         x = floor(x/mCellSize);
         y = floor(y/mCellSize);
     }
     return matrix;
 }

float AreaManager::getCellSize()
{
    return mCellSize;
}

void AreaManager::setWindImpact(Impact* impact, int _x, int _y)
{
    int x = _x, y = _y;
    CellInfo*** matrix = getMatrix(x, y);
    if(matrix != NULL)
    {
        if(matrix[x][y] == NULL)
        {
            CellInfo* cell = new CellInfo;
            cell->WindImpact = impact;
            cell->OccupyingFixture = NULL;
            matrix[x][y] = cell;
        }
        else
        {
            matrix[x][y]->WindImpact = impact;
        }
    }
}
Impact*  AreaManager::getWindImpact(int _x, int _y)
{
    int x = _x, y = _y;
    CellInfo*** matrix = getMatrix(x, y);
    if(matrix != NULL)
    {
        if(matrix[x][y] != NULL)
        {
            return matrix[x][y]->WindImpact;
        }
    }
    return NULL;
}


b2Fixture* AreaManager::getCellFixture(int _x, int _y)
{
    int x = _x, y = _y;
    CellInfo*** matrix = getMatrix(x, y);
    if(matrix != NULL)
    {
        if(matrix[x][y] != NULL)
        {
            return matrix[x][y]->OccupyingFixture;
        }
    }
    return NULL;
}
void AreaManager::reportNewFixtureLocation(const b2AABB *oldLocation, const b2AABB *newLocation, b2Fixture* body)
{
    CellRegion oldRegion;
    if(oldLocation != NULL)
    {
        // Reset old ocuppy
        float lowerX = oldLocation->lowerBound.x;
        if(lowerX < mArea->lowerBound.x)
        {
            lowerX =  mArea->lowerBound.x;
        }
        oldRegion.lowerX = floor(lowerX-0.5);
        float upperX = oldLocation->upperBound.x;
        if(upperX > mArea->upperBound.x)
        {
            upperX = mArea->upperBound.x;
        }
        oldRegion.upperX = floor(upperX +0.5);
        oldRegion.lowerY = floor(oldLocation->lowerBound.y - 0.5);
        oldRegion.upperY = floor(oldLocation->upperBound.y + 0.5);
        for(int x=oldRegion.lowerX; x<oldRegion.upperX; x++)
        {
            for(int y=oldRegion.lowerY; y<oldRegion.upperY + 0.5; y++)
            {
                int _x = x, _y = y;
                CellInfo*** matrix = getMatrix(_x,_y);
                if(matrix != NULL)
                {
                    if(matrix[_x][_y] != NULL)
                    {
                        matrix[_x][_y]->OccupyingFixture = NULL;
                    }
                }
            }
        }


    }
    CellRegion newRegion;
    if(newLocation != NULL)
    {
        // Set new ocuppy
        float lowerX = newLocation->lowerBound.x;
        if(lowerX < mArea->lowerBound.x)
        {
            lowerX =  mArea->lowerBound.x;
        }

        float upperX = newLocation->upperBound.x;
        if(upperX > mArea->upperBound.x)
        {
            upperX = mArea->upperBound.x;
        }
        newRegion.lowerX = floor(lowerX-0.5);
        newRegion.upperX = floor(newLocation->upperBound.y + 0.5);
        newRegion.lowerY = floor(newLocation->lowerBound.y - 0.5);
        newRegion.upperY = floor(newLocation->upperBound.y + 0.5);
        for(int x=newRegion.lowerX; x<floor(upperX +0.5); ++x)
        {
            for(int y=newRegion.lowerY; y<newRegion.upperY; ++y)
            {
                int _x = x, _y = y;
                CellInfo*** matrix = getMatrix(_x,_y);
                if(matrix != NULL)
                {
                    if(matrix[_x][_y] != NULL)
                    {
                        matrix[_x][_y]->OccupyingFixture = body;
                    }
                    else
                    {
                        CellInfo *cell = new CellInfo;
                        cell->OccupyingFixture = body;
                        cell->WindImpact = NULL;
                        matrix[_x][_y] = cell;
                    }
                }
            }
        }
    }
    if(oldLocation != NULL) mRegionUpdated.invoke(oldRegion);
    if(newLocation != NULL) mRegionUpdated.invoke(newRegion);
}

void AreaManager::activeRegionMoved(const b2AABB* newLocation)
{
    b2Vec2 newCenter = newLocation->GetCenter();
    float regionWidth = mThirdMatrixBegin - mSecondMatrixBegin;
    CellRegion newRegion;
    if(newCenter.x  > mThirdMatrixBegin)
    {
        // First, Second, Third -> Second, Third, First
        mArea->lowerBound.Set(mArea->lowerBound.x + regionWidth, mArea->lowerBound.y);
        mArea->upperBound.Set(mArea->upperBound.x + regionWidth, mArea->upperBound.y);

        mSecondMatrixBegin += regionWidth;
        mThirdMatrixBegin += regionWidth;

        CellInfo*** thirdMatrix = mThirdMatrix;
        mThirdMatrix = mFirstMatrix;
        mFirstMatrix = mSecondMatrix;
        mSecondMatrix = thirdMatrix;

        resetMatrix(mThirdMatrix);
        b2AABB nextRegion;
        nextRegion.lowerBound.Set(mArea->upperBound.x - regionWidth, mArea->lowerBound.y);
        nextRegion.upperBound = mArea->upperBound;
        scanArea(&nextRegion);

        newRegion.lowerX = mMatrixWidth * 2;
        newRegion.lowerY = 0;
        newRegion.upperX = mMatrixWidth * 3;
        newRegion.upperY = mMatrixHeight;
    }
    else if(newCenter.x < mSecondMatrixBegin)
    {
        // First, Second, Third -> Third, First, Second
        mArea->lowerBound.Set(mArea->lowerBound.x - regionWidth, mArea->lowerBound.y);
        mArea->upperBound.Set(mArea->upperBound.x - regionWidth, mArea->upperBound.y);

        mSecondMatrixBegin -= regionWidth;
        mThirdMatrixBegin -= regionWidth;

        CellInfo*** firstMatrix = mFirstMatrix;
        mFirstMatrix = mThirdMatrix;
        mThirdMatrix = mSecondMatrix;
        mSecondMatrix = firstMatrix;

        resetMatrix(mFirstMatrix);
        b2AABB nextRegion;
        nextRegion.lowerBound = mArea->lowerBound;
        nextRegion.upperBound.Set(mArea->lowerBound.x + regionWidth, mArea->upperBound.y);
        scanArea(&nextRegion);

        newRegion.lowerX = 0;
        newRegion.lowerY = 0;
        newRegion.upperX = mMatrixWidth;
        newRegion.upperY = mMatrixHeight;
    }

    mRegionUpdated.invoke(newRegion);
}

void AreaManager::resetMatrix(CellInfo*** matrix)
{
    for(int x=0; x<mMatrixWidth; ++x)
    {
        for(int y=0; y<mMatrixHeight; ++y)
        {
            if(matrix[x][y] != NULL)
            {
                mCellReset.invoke(matrix[x][y]);
                matrix[x][y]->OccupyingFixture = NULL;
                matrix[x][y]->WindImpact = NULL;
            }
        }
    }
}

void AreaManager::scanArea(b2AABB* region)
{
    AreaManagerQueryCallback callback;
    mWorld->QueryAABB(&callback, *region);
}

bool AreaManagerQueryCallback::ReportFixture (b2Fixture *fixture)
{
    areaManager.reportNewFixtureLocation(NULL, &fixture->GetAABB(), fixture);
    return true;
}



