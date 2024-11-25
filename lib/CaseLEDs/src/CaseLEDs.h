#ifndef CASE_LEDS_H_
#define CASE_LEDS_H_

#include <cstring>
#include <cstdio>
#include <stdint.h>

#include "pico/time.h"

#define CLED_COUNT 2
#define CLED_MAX_BRIGHTNESS 100
#define CLED_MAX_LEVEL 0xFFFF

typedef enum
{
	CLED_STATE_LED_RIGHT = (1 << 0),
	CLED_STATE_LED_LEFT = (1 << 1),
} CLEDStateMask;

typedef enum
{
	CLED_ANIM_NONE,
	CLED_ANIM_OFF,
	CLED_ANIM_SOLID,
	CLED_ANIM_BLINK,
	CLED_ANIM_CYCLE,
	CLED_ANIM_FADE,
	CLED_ANIM_BLINK_CUSTOM,
} CLEDAnimationType;

const CLEDAnimationType CLED_ANIMATION_TYPES[] =
{
	CLED_ANIM_NONE,
	CLED_ANIM_OFF,
	CLED_ANIM_SOLID,
	CLED_ANIM_BLINK,
	CLED_ANIM_CYCLE,
	CLED_ANIM_FADE,
	CLED_ANIM_BLINK_CUSTOM,
};

typedef enum
{
	CLED_SPEED_OFF        = 0,
	CLED_SPEED_PLAID      = 10,
	CLED_SPEED_LUDICROUS  = 20,
	CLED_SPEED_RIDICULOUS = 50,
	CLED_SPEED_FASTER     = 100,
	CLED_SPEED_FAST       = 250,
	CLED_SPEED_NORMAL     = 500,
	CLED_SPEED_SLOW       = 1000,
} CLEDAnimationSpeed;

const CLEDAnimationSpeed CLED_ANIMATION_SPEEDS[] =
{
	CLED_SPEED_OFF,
	CLED_SPEED_PLAID,
	CLED_SPEED_LUDICROUS,
	CLED_SPEED_RIDICULOUS,
	CLED_SPEED_FASTER,
	CLED_SPEED_FAST,
	CLED_SPEED_NORMAL,
	CLED_SPEED_SLOW,
};

struct CLEDAnimationState
{
	uint8_t state = 0;
	CLEDAnimationType animation;
	CLEDAnimationSpeed speed;
	uint32_t speedOn = 0;
	uint32_t speedOff = 0;
};

class CaseLEDs
{
	public:
		virtual void setup() = 0;
		virtual void display() = 0;
		void animate(CLEDAnimationState animationState);

	protected:
		void parseState(uint8_t state)
		{
			memcpy(lastCLEDState, currentCLEDState, sizeof(currentCLEDState));
			for (int i = 0; i < CLED_COUNT*10; i++)
				currentCLEDState[i] = (state & (1 << i)) == (1 << i);
		}

		inline void reset()
		{
			memset(lastCLEDState, 0, sizeof(lastCLEDState));
			memset(currentCLEDState, 0, sizeof(currentCLEDState));
			nextAnimationTime = get_absolute_time();
			brightness = CLED_MAX_BRIGHTNESS;
			fadeIn = false;
		}

		inline void handleBlink(CLEDAnimationSpeed speed)
		{
			for (int i = 0; i < CLED_COUNT*10; i++)
			{
				if (lastCLEDState[i])
					currentCLEDState[i] = false;
			}
			nextAnimationTime = make_timeout_time_ms(speed);
		}

		inline void handleBlinkCustom(uint32_t speed, uint32_t speedOff)
		{
			uint32_t nextSpeed = 0;
			for (int i = 0; i < CLED_COUNT*10; i++)
			{
				if (speed > 0 && speedOff == 0) {
					if (lastCLEDState[i]) {
						currentCLEDState[i] = true;
						nextSpeed = speed;
					}
				} else {
					if (lastCLEDState[i]) {
						currentCLEDState[i] = false;
						nextSpeed = speed;
					} else {
						currentCLEDState[i] = true;
						nextSpeed = speedOff;
					}
				}
			}
			nextAnimationTime = make_timeout_time_ms(nextSpeed);
		}

		inline void handleCycle(CLEDAnimationSpeed speed)
		{
			for (int i = 0; i < CLED_COUNT*10; i++)
			{
				if (lastCLEDState[i] != 0)
				{
					memset(currentCLEDState, 0, sizeof(currentCLEDState));
					for (int j = 0; j < CLED_COUNT; j++)
					{
						if (lastCLEDState[j])
						{
							currentCLEDState[(j + 1) % CLED_COUNT] = true;
							break;
						}
					}
					break;
				}
			}
			nextAnimationTime = make_timeout_time_ms(speed);
		}

		inline void handleFade(CLEDAnimationSpeed speed)
		{
			if (fadeIn)
			{
				brightness += 5;
				if (brightness >= CLED_MAX_BRIGHTNESS)
				{
					brightness = 100;
					fadeIn = false;
				}
			}
			else
			{
				brightness -= 5;
				if (brightness <= 5)
				{
					brightness = 5;
					fadeIn = true;
				}
			}

			nextAnimationTime = make_timeout_time_ms(CLED_SPEED_RIDICULOUS);
			//nextAnimationTime = make_timeout_time_ms(speed);
		}

		uint16_t ledLevels[CLED_COUNT*10] = {};
		absolute_time_t nextAnimationTime;
		CLEDAnimationType selectedAnimation = CLED_ANIM_NONE;
		uint32_t brightness = CLED_MAX_BRIGHTNESS;
		bool lastCLEDState[CLED_COUNT*10] = { };
		bool currentCLEDState[CLED_COUNT*10] = { };
		bool fadeIn = false;
};

#endif
