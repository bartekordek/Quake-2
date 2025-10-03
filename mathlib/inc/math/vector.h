#ifndef __math_vector_h__
#define __math_vector_h__

#include "../../shared/inc/shared/config.h"

typedef float	vec_t;
typedef vec_t	vec3_t[3];
typedef vec_t	vec5_t[5];

EXTERNC vec_t	VectorNormalize(vec3_t v);  // returns vector length
EXTERNC vec_t	VectorNormalize2(vec3_t v, vec3_t out);
EXTERNC void	CrossProduct(vec3_t v1, vec3_t v2, vec3_t cross);

// just in case you do't want to use the macros
EXTERNC vec_t	_DotProduct(vec3_t v1, vec3_t v2);
EXTERNC void	_VectorSubtract(vec3_t veca, vec3_t vecb, vec3_t out);
EXTERNC void	_VectorAdd(vec3_t veca, vec3_t vecb, vec3_t out);
EXTERNC void	_VectorCopy(vec3_t in, vec3_t out);

EXTERNC void	ClearBounds(vec3_t mins, vec3_t maxs);
EXTERNC void	AddPointToBounds(vec3_t v, vec3_t mins, vec3_t maxs);
EXTERNC int		VectorCompare(vec3_t v1, vec3_t v2);
EXTERNC vec_t	VectorLength(vec3_t v);
EXTERNC void	VectorInverse(vec3_t v);
EXTERNC void	VectorScale(vec3_t in, vec_t scale, vec3_t out);
EXTERNC int		Q_log2(int val);

EXTERNC void	R_ConcatRotations(float in1[3][3], float in2[3][3], float out[3][3]);
EXTERNC void	R_ConcatTransforms(float in1[3][4], float in2[3][4], float out[3][4]);

EXTERNC void	AngleVectors(vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
EXTERNC int		BoxOnPlaneSide(vec3_t emins, vec3_t emaxs, struct cplane_s *plane);
EXTERNC float	anglemod(float a);
EXTERNC float	LerpAngle(float a1, float a2, float frac);
EXTERNC void	ProjectPointOnPlane(vec3_t dst, const vec3_t p, const vec3_t normal);
EXTERNC void	PerpendicularVector(vec3_t dst, const vec3_t src);
EXTERNC void	RotatePointAroundVector(vec3_t dst, const vec3_t dir, const vec3_t point, float degrees);

#endif	// __math_vector_h__