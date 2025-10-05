#ifndef ANIMATION_H
#define ANIMATION_H

#include "Common.h"

typedef struct
{
	sfSprite* sprite;
	sfIntRect firstFrame;
	int frame;
	unsigned nbFrame;
	unsigned frameRate;
	float timer;
	sfBool isLooping;
	sfBool isFinished;
	int nameId;
}AnimationData;

AnimationData CreateAnimation(sfSprite* _sprite, unsigned _nbFrame, unsigned _frameRate, sfIntRect _firstFrame, sfBool _isLooping);
void UpdateReverseAnimation(AnimationData* _animation, float _dt);
void UpdateAnimation(AnimationData* _animation, float _dt);
void UpdateAnimationVerti(AnimationData* _animation, float _dt);
void UpdateSizeAnimation(AnimationData* _animation, float _dt, float _percent);
void DrawAnimation(const AnimationData* _animation, sfRenderWindow* _window);

void LoadAnimation(AnimationData* _animation);
sfBool AnimationIsFinished(const AnimationData* _animation);
void SetAnimationFramerate(AnimationData* _animation, int _newFramerate);
void SetAnimationFrame(AnimationData* _animation, int _newFrame);
int GetAnimationNBFrame(AnimationData* _animation);
#endif

