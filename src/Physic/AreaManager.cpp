#include "Physic/AreaManager.hpp"



void AreaManager::init(b2World *world, float sellSize)
{
    mWorld = world;
    float areaWidth = 1280;
    float areaHeight = 1024;
    mArea = new b2AABB;
    mArea->lowerBound.Set(0, 0);
    mArea->upperBound.Set(0,0);
    mSecondMatrixBegin = 0;
    mThirdMatrixBegin = 0;

    mMatrixWidth = floor(areaWidth / mSellSize + 0.5);
    mMatrixHeight = floor(areaHeight / mSellSize + 0.5);

    mSecondMatrix = createMatrix(mMatrixWidth, mMatrixHeight);
    mThirdMatrix  = createMatrix(mMatrixWidth, mMatrixHeight);
    mFirstMatrix = createMatrix(mMatrixWidth, mMatrixHeight);

    scanArea(mArea);

    mRegionUpdated.invoke(getCellRegion());
    //mRegionUpdated = new CL_Signal_v1<b2AABB*>;

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

 CellInfo*** AreaManager::getMatrix(int x, int y)
 {
     CellInfo*** matrix = NULL;
     if(x < mSecondMatrixBegin)
     {
         matrix = mFirstMatrix;
     }
     else if(x < mThirdMatrixBegin)
     {
         matrix = mSecondMatrix;
     }
     else
     {
         matrix = mThirdMatrix;
     }
     return matrix;
 }

float AreaManager::getCellSize()
{
    return mSellSize;
}

void AreaManager::setWindImpact(Impact* impact, int x, int y)
{
    CellInfo*** matrix = getMatrix(x, y);
    if(matrix[x][y] == NULL)
    {
        CellInfo* cell = new CellInfo;
        cell->WindImpact = impact;
        cell->OccupyingBody = NULL;
        matrix[x][y] = cell;
    }
    else
    {
        matrix[x][y]->WindImpact = impact;
    }
}
Impact*  AreaManager::getWindImpact(int x, int y)
{
    CellInfo*** matrix = getMatrix(x, y);
    if(matrix[x][y] != NULL)
    {
        return matrix[x][y]->WindImpact;
    }
    else
    {
        return NULL;
    }
}


b2Body* AreaManager::getCellBody(int x, int y)
{
    CellInfo*** matrix = getMatrix(x, y);
    if(matrix[x][y] != NULL)
    {
        return matrix[x][y]->OccupyingBody;
    }
    else
    {
        return NULL;
    }
}
void AreaManager::reportNewBodyLocation(const b2AABB *oldLocation, const b2AABB *newLocation, b2Body* body)
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
                CellInfo*** matrix = getMatrix(x,y);
                if(matrix[x][y] != NULL)
                {
                    matrix[x][y]->OccupyingBody = NULL;
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
                CellInfo*** matrix = getMatrix(x,y);
                if(matrix[x][y] != NULL)
                {
                    matrix[x][y]->OccupyingBody = body;
                }
                else
                {
                    CellInfo *cell = new CellInfo;
                    cell->OccupyingBody = body;
                    cell->WindImpact = NULL;
                    matrix[x][y] = cell;
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
                matrix[x][y]->OccupyingBody = NULL;
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
    areaManager.reportNewBodyLocation(NULL, &fixture->GetAABB(), fixture->GetBody());
    return true;
}




