#include "Light.h"
#include "../main.h"
#include "../Utility.h"
#include "../System/Time.h"

DirectX::SimpleMath::Matrix DirectionalLight::WorldIdentity()
{
    WorldMatrix = DirectX::SimpleMath::Matrix::Identity;
    WorldMatrix = WorldMatrix.Transpose();
    return WorldMatrix;
}

DirectX::SimpleMath::Matrix DirectionalLight::CalcWorldMatrix()
{
    WorldMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(mPosition);
    WorldMatrix = WorldMatrix.Transpose();
    return WorldMatrix;
}


DirectX::SimpleMath::Matrix DirectionalLight::CalcView()
{
    DirectX::SimpleMath::Vector3 Dir = DirectX::SimpleMath::Vector3(
        Direction.x + mPosition.x, Direction.y+ mPosition.y, Direction.z + mPosition.z);

    ViewMatrix = DirectX::XMMatrixLookAtLH(mPosition, Dir, DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f));
    ViewMatrix = ViewMatrix.Transpose();
    return ViewMatrix;
}

DirectX::SimpleMath::Matrix DirectionalLight::CalcProjection(float fovx, float fovy, float clipnear, float clipfar)
{
    ProjectionMatrix = DirectX::XMMatrixOrthographicLH(fovx,fovy, clipnear, clipfar);
    ProjectionMatrix = ProjectionMatrix.Transpose();

    return ProjectionMatrix;
}

CustomPointLight::CustomPointLight()
{
    pLight = new PointLight();
}

void CustomPointLight::SetColor(float r, float g, float b, float a)
{
    pLight->Diffuse.x = r;
    pLight->Diffuse.y = g;
    pLight->Diffuse.z = b;
    pLight->Diffuse.w = a;
}

void CustomPointLight::InitPointLight()
{
    pLight->Position.w = mInitRange;
    float posx = cosf(static_cast<float>(Euler_To_Radian(mAngleY))) * sinf(static_cast<float>(Euler_To_Radian(mAngleXZ))) * mLength;
    float posz = cosf(static_cast<float>(Euler_To_Radian(mAngleY))) * cosf(static_cast<float>(Euler_To_Radian(mAngleXZ))) * mLength;
    float posy = cosf(static_cast<float>(Euler_To_Radian(mAngleY))) * mLength;
    pLight->Position.x = posx + AxisPos.x;
    pLight->Position.y = posy + AxisPos.y;
    pLight->Position.z = posz + AxisPos.z;
}

void CustomPointLight::UpdatePointLight()
{
    if (bReturn)
    {
        pLight->Position.w -= mGrowSpeed * Time->GetDeltaTime();
        if (pLight->Position.w <= mMinRange)
        {
            pLight->Position.w = mMinRange;
            bReturn = !bReturn;
        }
    }
    else
    {
        pLight->Position.w += mGrowSpeed * Time->GetDeltaTime();
        if (pLight->Position.w >= mMaxRange)
        {
            pLight->Position.w = mMaxRange;
            bReturn = !bReturn;
        }
    }

    mAngleY += mAngleSpeedY/* * Time->GetDeltaTime();*/;
    if (mAngleY >= 360)
    {
        mAngleY = 0;
    }
    mAngleXZ += mAngleSpeedXZ/* * Time->GetDeltaTime()*/;
    if (mAngleXZ >= 360)
    {
        mAngleXZ = 0;
    }

    float posx = cosf(static_cast<float>(Euler_To_Radian(mAngleY))) * sinf(static_cast<float>(Euler_To_Radian(mAngleXZ))) * mLength;
    float posz = cosf(static_cast<float>(Euler_To_Radian(mAngleY))) * cosf(static_cast<float>(Euler_To_Radian(mAngleXZ))) * mLength;
    float posy = cosf(static_cast<float>(Euler_To_Radian(mAngleY))) * mLength;
    pLight->Position.x = posx + AxisPos.x;
    pLight->Position.y = posy + AxisPos.y;
    pLight->Position.z = posz + AxisPos.z;
}
