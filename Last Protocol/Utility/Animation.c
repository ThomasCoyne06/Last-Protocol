#include "Animation.h"

AnimationData CreateAnimation(sfSprite* _sprite, unsigned _nbFrame, unsigned _frameRate, sfIntRect _firstFrame, sfBool _isLooping)
{
	AnimationData newAnimation = { 0 };

	newAnimation.sprite = _sprite;
	newAnimation.firstFrame = _firstFrame;
	newAnimation.nbFrame = _nbFrame;
	newAnimation.frameRate = _frameRate;
	newAnimation.isLooping = _isLooping;

	return newAnimation;
}

void UpdateAnimation(AnimationData* _animation, float _dt)
{
	// Update the frame
	_animation->timer += _dt;
	while (_animation->timer > 1.f / _animation->frameRate)
	{
		_animation->timer -= 1.f / _animation->frameRate;
		_animation->frame++;

		// End of the animation
		if (_animation->frame == _animation->nbFrame)
		{
			// Animation is looping ? V2
			//_animation->frame = _animation->isLooping ? 0 : _animation->nbFrame - 1;

			// Animation is looping ? V1
			if (_animation->isLooping)
			{
				_animation->frame = 0;
			}
			else
			{
				_animation->frame--;
				_animation->isFinished = sfTrue;
			}
		}
	}

	// Select a frame
	unsigned frameWidth = _animation->firstFrame.width;
	unsigned frameHeight = _animation->firstFrame.height;
	unsigned frameLeft = _animation->firstFrame.left;
	unsigned frameTop = _animation->firstFrame.top;

	sfIntRect area = { frameLeft + _animation->frame * frameWidth, frameTop, frameWidth, frameHeight };
	sfSprite_setTextureRect(_animation->sprite, area);
}

void UpdateAnimationVerti(AnimationData* _animation, float _dt)
{
	// Update the frame
	_animation->timer += _dt;
	while (_animation->timer > 1.f / _animation->frameRate)
	{
		_animation->timer -= 1.f / _animation->frameRate;
		_animation->frame++;

		// End of the animation
		if (_animation->frame == _animation->nbFrame)
		{
			// Animation is looping ? V2
			//_animation->frame = _animation->isLooping ? 0 : _animation->nbFrame - 1;

			// Animation is looping ? V1
			if (_animation->isLooping)
			{
				_animation->frame = 0;
			}
			else
			{
				_animation->frame--;
				_animation->isFinished = sfTrue;
			}
		}
	}

	// Select a frame
	unsigned frameWidth = _animation->firstFrame.width;
	unsigned frameHeight = _animation->firstFrame.height;
	unsigned frameLeft = _animation->firstFrame.left;
	unsigned frameTop = _animation->firstFrame.top;

	sfIntRect area = { frameLeft, frameTop + _animation->frame * frameHeight, frameWidth, frameHeight };
	sfSprite_setTextureRect(_animation->sprite, area);
}

void UpdateReverseAnimation(AnimationData* _animation, float _dt)
{
	// Update the frame
	_animation->timer -= _dt;
	while (_animation->timer < 0)
	{
		_animation->timer += 1.f / _animation->frameRate;
	//_animation->timer += _dt;
	//while (_animation->timer > 1.f / _animation->frameRate)
	//{
	//	_animation->timer -= 1.f / _animation->frameRate;
		_animation->frame--;
		

		// End of the animation
		if (_animation->frame <= 0)
		{
			// Animation is looping ? V2
			//_animation->frame = _animation->isLooping ? 0 : _animation->nbFrame - 1;

			// Animation is looping ? V1
			if (_animation->isLooping)
			{
				_animation->frame = _animation->nbFrame;
			}
			else
			{
				
				_animation->frame = 0;
				_animation->isFinished = sfTrue;
			}
		}
		
	}

	// Select a frame
	unsigned frameWidth = _animation->firstFrame.width;
	unsigned frameHeight = _animation->firstFrame.height;
	unsigned frameLeft = _animation->firstFrame.left;
	unsigned frameTop = _animation->firstFrame.top;

	sfIntRect area = { frameLeft + _animation->frame * frameWidth, frameTop, frameWidth, frameHeight };
	sfSprite_setTextureRect(_animation->sprite, area);
}

void UpdateSizeAnimation(AnimationData* _animation, float _dt, float _percent)
{
	// Update the frame
	_animation->timer += _dt;
	while (_animation->timer > 1.f / _animation->frameRate)
	{
		_animation->timer -= 1.f / _animation->frameRate;
		_animation->frame++;

		// End of the animation
		if (_animation->frame == _animation->nbFrame)
		{
			// Animation is looping ? V2
			//_animation->frame = _animation->isLooping ? 0 : _animation->nbFrame - 1;

			// Animation is looping ? V1
			if (_animation->isLooping)
			{
				_animation->frame = 0;
			}
			else
			{
				_animation->frame--;
				_animation->isFinished = sfTrue;
			}
		}
	}

	// Select a frame
	unsigned frameWidth = _animation->firstFrame.width;
	unsigned frameHeight = _animation->firstFrame.height;
	unsigned frameLeft = _animation->firstFrame.left;
	unsigned frameTop = _animation->firstFrame.top;

	sfIntRect area = { frameLeft + (_animation->frame+1-_percent )* frameWidth, frameTop, frameWidth * _percent, frameHeight};
	sfSprite_setTextureRect(_animation->sprite, area);
}

void DrawAnimation(const AnimationData* _animation, sfRenderWindow* _window)
{
	sfRenderWindow_drawSprite(_window, _animation->sprite, NULL);
}

void LoadAnimation(AnimationData* _animation)
{
	_animation->frame = 0;
	_animation->timer = 0.f;
	_animation->isFinished = sfFalse;
}

sfBool AnimationIsFinished(const AnimationData* _animation)
{
	return _animation->isFinished;
}
void SetAnimationFramerate(AnimationData* _animation, int _newFramerate)
{
	_animation->frameRate = _newFramerate;
}
void SetAnimationFrame(AnimationData* _animation, int _newFrame)
{
	_animation->frame = _newFrame;
}

int GetAnimationNBFrame(AnimationData* _animation)
{
	return _animation->nbFrame;
}