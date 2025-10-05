#include "Mathematique.h"

#pragma region numerical sequence
void GetSequenceReason(const float _tab[], unsigned int _size)
{
	float temp = _tab[1] - _tab[0];
	float reasonR = 0;
	float reasonQ = 0;
	sfBool aritmetique = sfFalse;
	sfBool geometrique = sfFalse;

	for (unsigned int i = 0; i < _size - 1; i++)
	{
		reasonR = _tab[i + 1] - _tab[i];
		reasonQ = _tab[i + 1] / _tab[i];

		aritmetique = reasonR == temp;
		geometrique = _tab[i + 1] == _tab[i] * reasonQ;
	}

	if (aritmetique)
	{
		printf("La suite est aritmetique de raison : %.1f\n", reasonR);
	}
	else
	{
		printf("La suite n est pas aritmetique\n");
	}

	if (geometrique)
	{
		printf("La suite est geometrique de raison : %.1f\n", reasonQ);
	}
	else
	{
		printf("La suite n est pas geometrique\n");
	}
}
#pragma endregion

#pragma region quadratic polynomial
#pragma endregion

#pragma region derivation and variation of function
#pragma endregion

#pragma region exponential
#pragma endregion

#pragma region trigonometry

float ConvertDegToRad(int _deg)
{
	return _deg * (float)M_PI / 180;
}
float ConvertRadToDeg(float _rad)
{
	return _rad * 180 / (float)M_PI;
}

float ConvertDirectionToRad(sfVector2f _direction)
{
	return atan2f(_direction.y, _direction.x);
}
float GetAngle(sfVector2f _p1, sfVector2f _p2)
{
	return atan2f(_p2.y - _p1.y, _p2.x - _p1.x);
}
#pragma endregion

#pragma region percentage
float ConvertValToPercent(float _total, float _val)
{
	return _total / (_total + (_val - _total));
}
#pragma endregion

#pragma region vector
sfVector2f GetVector(sfVector2f _p1, sfVector2f _p2)
{
	return (sfVector2f) { _p2.x - _p1.x, _p2.y - _p1.y };
}
float GetDistanceBetweenPoints(sfVector2f _p1, sfVector2f _p2)
{
	return sqrtf((_p1.x - _p2.x) * (_p1.x - _p2.x) + (_p1.y - _p2.y) * (_p1.y - _p2.y));
}
float GetVectorLenght(sfVector2f _vector)
{
	return sqrtf(_vector.x * _vector.x + _vector.y * _vector.y);
}
sfVector2f GetPointOnVectorWithDistance(sfVector2f _p1, sfVector2f _p2, float _distance)
{
	sfVector2f vector = GetVector(_p1, _p2);
	float vectorSize = GetVectorLenght(vector);
	float percent = ConvertValToPercent(_distance, vectorSize);
	sfVector2f newVector = { percent * vector.x , percent * vector.y };

	return (sfVector2f) { _p1.x + newVector.x, _p1.y + newVector.y };
}

sfVector2f GetPointWithAngleAndDistance(sfVector2f _startPosititon, float _distance, float _angle)
{
	sfVector2f point2;
	point2.x = _startPosititon.x + _distance * cosf(_angle);
	point2.y = _startPosititon.y + _distance * sinf(_angle);

	return point2;
}

#pragma endregion

#pragma region dot product
float GetScalarWithVector(sfVector2f _vector1, sfVector2f _vector2)
{
	return _vector1.x * _vector2.x + _vector1.y * _vector2.y;
}

float GetScalarWithDistance(float _distance1, float _distance2, float _angle)
{
	return _distance1 * _distance2 * (float)cos(_angle);
}
#pragma endregion

#pragma region clamp
#pragma endregion

#pragma region interpolation
sfVector2f Interpolation(float _time, sfVector2f _startPoint, sfVector2f endPoint)
{
	return (sfVector2f)
	{
		(1 - _time)* _startPoint.x + _time * endPoint.x,
			(1 - _time)* _startPoint.y + _time * endPoint.y
	};
}
#pragma endregion

#pragma region geometrie
#pragma endregion

#pragma region GridGetParameter
CellCoordinates GetColumnLineWidthIndex(int _nbColumn, int _id)
{
	int column = _id % _nbColumn;
	int line = _id / _nbColumn;

	return (CellCoordinates) { column, line };
}

int GetIndexWidthColumnLine(int _nbColumn, CellCoordinates _columnLine)
{
	return _columnLine.line * _nbColumn + _columnLine.column;
}

CellCoordinates GetColumnLineWithPosition(sfVector2f _position, sfFloatRect _firstCellRect)
{
	int column = (int)((_position.x - _firstCellRect.left) / _firstCellRect.width);
	int line = (int)((_position.y - _firstCellRect.top) / _firstCellRect.height);

	return (CellCoordinates) { column, line };
}

sfVector2f GetPositionWithColumnLine(CellCoordinates _columnLine, sfBool _centerOrigin, sfFloatRect _firstCellRect)
{
	float x = _firstCellRect.left + _columnLine.column * _firstCellRect.width ;
	float y = _firstCellRect.top + _columnLine.line * _firstCellRect.height  ;

	if (_centerOrigin)
	{
		x += _firstCellRect.width / 2;
		y += _firstCellRect.height / 2;
	}

	return (sfVector2f) { x, y };
}

sfFloatRect GetGridHitbox(sfFloatRect _firstCaseHitbox, int _nbColumn, int _nbLine)
{
	return (sfFloatRect)
	{
		_firstCaseHitbox.left,
			_firstCaseHitbox.top,
			_firstCaseHitbox.width* _nbColumn,
			_firstCaseHitbox.height* _nbLine
	};
}
#pragma endregion

sfBool ResizeArray(void** _array, unsigned _newSize, unsigned _typeSize)
{
	if (_newSize > 0)
	{
		void* newArray = realloc(*_array, _newSize * _typeSize);

		if (newArray != NULL)
		{
			*_array = newArray;
			return sfTrue;
		}
		else
		{
			printf("realloc failed in ResizeArray\n");
			return sfFalse;
		}
	}
	return sfFalse;
}