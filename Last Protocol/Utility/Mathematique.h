#ifndef MATHEMATIQUE_H
#define MATHEMATIQUE_H

#include "Common.h"

#define M_SQRT3 1.732050
#define M_COS_30 0.866025
#define M_COS_45 0.707106
#define M_EPSILON 0.00001

typedef struct
{
	int column;
	int line;
}CellCoordinates;

#pragma region numerical sequence
void GetSequenceReason(const float _tab[], unsigned int _size);
#pragma endregion

#pragma region polynome du second degre
#pragma endregion

#pragma region derivation et variation de fonction
#pragma endregion

#pragma region exponnentiel
#pragma endregion

#pragma region trigonometrie
float ConvertDegToRad(int _deg);
float ConvertRadToDeg(float _rad);
float ConvertDirectionToRad(sfVector2f _direction);
float GetAngle(sfVector2f _p1, sfVector2f _p2);
#pragma endregion

#pragma region percentage
float ConvertValToPercent(float _total, float _val);
#pragma endregion

#pragma region vector
sfVector2f GetVector(sfVector2f _p1, sfVector2f _p2);
float GetDistanceBetweenPoints(sfVector2f _p1, sfVector2f _p2);
float GetVectorLenght(sfVector2f _vector);
sfVector2f GetPointOnVectorWithDistance(sfVector2f _p1, sfVector2f _p2, float _distance);
sfVector2f GetPointWithAngleAndDistance(sfVector2f _startPosititon, float _distance, float _angle);
#pragma endregion

#pragma region dot product
float GetScalarWithVector(sfVector2f _vector1, sfVector2f _vector2);
float GetScalarWithDistance(float _distance1, float _distance2, float _angle);
#pragma endregion

#pragma region clamp
#pragma endregion

#pragma region interpolation
sfVector2f Interpolation(float _time, sfVector2f _startPoint, sfVector2f endPoint);
#pragma endregion

#pragma region geometrie
#pragma endregion

#pragma region GridGetParameter
CellCoordinates GetColumnLineWidthIndex(int _nbColumn, int _id);
int GetIndexWidthColumnLine(int _nbColumn, CellCoordinates _columnLine);
CellCoordinates GetColumnLineWithPosition(sfVector2f _position, sfFloatRect _firstCellRect);
sfVector2f GetPositionWithColumnLine(CellCoordinates _columnLine, sfBool _centerOrigin, sfFloatRect _firstCellRect);
sfFloatRect GetGridHitbox(sfFloatRect _firstCaseHitbox, int _nbColumn, int _nbLine);
#pragma endregion

sfBool ResizeArray(void** _array, unsigned _newSize, unsigned _typeSize);

#endif
