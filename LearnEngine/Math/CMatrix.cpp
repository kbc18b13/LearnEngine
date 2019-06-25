#include "CMatrix.h"
namespace LearnEngine {

const CMatrix CMatrix::Identity(1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1);

CMatrix::CMatrix(float _00, float _01, float _02, float _03,
				 float _10, float _11, float _12, float _13,
				 float _20, float _21, float _22, float _23,
				 float _30, float _31, float _32, float _33)
	:mat{ { _00, _01, _02, _03},
          { _10, _11, _12, _13},
          { _20, _21, _22, _23},
          { _30, _31, _32, _33} } {};

void CMatrix::Mul4x4(const CMatrix& mat) {

}

}