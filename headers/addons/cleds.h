/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef _CLEDS_H_
#define _CLEDS_H_

#include "BoardConfig.h"
#include <stdint.h>
#include "AnimationStation.hpp"
#include "storagemanager.h"
#include "CaseLEDs.h"
#include "gpaddon.h"
#include "helper.h"

#include "enums.pb.h"

// This needs to be moved to storage if we're going to share between modules
extern NeoPico *neopico;
extern AnimationStation as;

// Player LED Module
#define CLEDName "CLED"

// Player LED Module
class CaseLEDAddon : public GPAddon
{
public:
	virtual bool available();
	virtual void setup();
	virtual void preprocess() {}
	virtual void process();
	virtual std::string name() { return CLEDName; }

protected:
	CLEDAnimationState animationState;

private:
	bool turnOffWhenSuspended;
};

#endif
