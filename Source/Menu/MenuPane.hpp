#pragma once

#include <Util/Easing.hpp>

namespace sf
{
	struct Event;
	class RenderTarget;
}

class MenuPane
{
public:
	enum State
	{
		Opening,   // Coming in from the left
		Open,      // Pane is open in the foreground
		Shading,   // Sliding off to the left
		Shaded,    // In the background on the left
		Unshading, // Sliding back into focus
		Closing,   // Disappearing off to the right
		Closed     // Gone.
	};

	MenuPane();
	virtual ~MenuPane();

	virtual void draw(sf::RenderTarget&);
	virtual void handleEvent(sf::Event&);
	virtual void update(float dt);

	void setState(State s);
	State getState() const;

protected:
	float getOffset() const;
	float getAlpha() const;

private:
	Easer mOffset, mAlpha;
	State mState;
};

