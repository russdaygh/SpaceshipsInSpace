#pragma once

//System Includes


//Local Includes


//Using Statements


namespace RustyLib
{
	namespace Framework
	{
		namespace ScreenManagement
		{
			class RLScreen;

			class RLScreenManager
			{
			private:
				RLScreen *m_currentScreen;
				bool bIsInitialised;

				RLScreenManager(void );
				~RLScreenManager(void );

				void operator=(const RLScreenManager&);

				void Initialise(void );

			public:
				static RLScreenManager& Instance(void );

				//Screen Manipulation Methods
				void ChangeScreen(RLScreen *new_screen);
				//Uses const pointer because it may be null
				RLScreen const* GetCurrentScreen(void );

				//Tick methods
				void Update(const float time_step);
				void Render(void );
			};

			typedef RLScreenManager ScreenManager;
		}
	}
}