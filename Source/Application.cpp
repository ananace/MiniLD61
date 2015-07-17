#include "Application.hpp"
#include "State/IntroState.hpp"
#include "Util/Font.hpp"
#include "Util/Profiling.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <sstream>

const float TICK_LENGTH = 1.f / 60.f;

Application::Application(int argc, char** argv) :
	mWindow()
{
}

Application::~Application()
{
}

void Application::run()
{
	mWindow.create(sf::VideoMode(800, 600), "MiniLD 61");
	mState.changeState<IntroState>();

	sf::Clock frameTimer;
	sf::Event ev;
	sf::Text profilingText("", sf::getDefaultFont(), 10U);

	float dt, extraDT = 0;
	float totalTime = 0;

	{ // Prime the profiling tree
		PROFILE("Frame");
		{ PROFILE("Tick"); }
		{ PROFILE("Update"); }
		{ PROFILE("Draw"); }
	} Profiler::resetBlocks();

	auto root = Profiler::getRoot()->getChild("Frame");

	while (mWindow.isOpen())
	{ Profiler::startBlock("Frame");
		dt = frameTimer.restart().asSeconds();
		extraDT += dt;

		while (mWindow.pollEvent(ev))
		{ PROFILE("Events");
			mState.handle_event(ev);

			if (ev.type == sf::Event::Closed)
				mWindow.close();
		}

		while (extraDT >= TICK_LENGTH)
		{ PROFILE("Tick");
			mState.fixed_update(TICK_LENGTH);

			extraDT -= TICK_LENGTH;
		}

		PROFILE_CALL("Update", mState.variadic_update(dt));

		mWindow.clear();

		PROFILE_CALL("Draw", mState.draw(mWindow));
		mWindow.draw(profilingText);

		mWindow.display();

		Profiler::endBlock(); // Frame

		if (root->getTotalTime() >= 1)
		{
			std::ostringstream oss;
			oss << *root << std::endl;
			profilingText.setString(oss.str());

			Profiler::resetBlocks();
		}
	}
}

