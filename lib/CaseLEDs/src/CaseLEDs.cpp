#include "CaseLEDs.h"
#include "pico/stdlib.h"

void CaseLEDs::animate(CLEDAnimationState animationState)
{
	// Reset state and bypass timer check if animation changed
	if (animationState.animation != selectedAnimation)
	{
		reset();
		selectedAnimation = animationState.animation;
	}
	else if (!time_reached(nextAnimationTime))
	{
		return;
	}

	parseState(animationState.state);

	switch (selectedAnimation)
	{
		case CLED_ANIM_BLINK:
			handleBlink(animationState.speed);
			break;

		case CLED_ANIM_CYCLE:
			handleCycle(animationState.speed);
			break;

		case CLED_ANIM_FADE:
			handleFade(animationState.speed);
			break;

		case CLED_ANIM_BLINK_CUSTOM:
			handleBlinkCustom(animationState.speedOn, animationState.speedOff);
			break;

		default:
			break;
	}

	for (int i = 0; i < CLED_COUNT*10; i++)
		ledLevels[i] = CLED_MAX_LEVEL - (currentCLEDState[i] ? (brightness * brightness) : 0);
}
