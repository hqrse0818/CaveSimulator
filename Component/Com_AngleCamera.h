#pragma once
#include "../Component/Com_Camera.h"
class Com_AngleCamera :
    public Com_Camera
{
private:
    float fCurrentAngle = 180.0f;
    float fDistance = 10.0f;
    float fHeight = 10.0f;
    float fMaxHeight = 85.0f;
    float fMinHeihgt = 10.0f;
    float fMaxDistance = 100.0f;
public:
    float GetAngle()
    {
        return fCurrentAngle;
    }
    void SetAngle(float _val)
    {
        fCurrentAngle = _val;
    }
    void SetDistance(float _val)
    {
        fDistance = _val;
        
        if (_val < 1)
        {
            fDistance = 1;
        }
        if (_val > fMaxDistance)
        {
            fDistance = fMaxDistance;
        }
    }


    float GetHeight()
    {
        return fHeight;
    }

    void SetHeight(float _val)
    {
        fHeight = _val;
        if (fHeight > fMaxHeight)
        {
            fHeight = fMaxHeight;
        }
        else if (fHeight < fMinHeihgt)
        {
            fHeight = fMinHeihgt;
        }
    }

    float GetDistance()
    {
        return fDistance;
    }

    void SetHeightRange(float _min, float _max)
    {
        fMinHeihgt = _min;
        fMaxHeight = _max;
    }

    void SetMaxDistance(float _value)
    {
        fMaxDistance = _value;
    }

    void Update();
};